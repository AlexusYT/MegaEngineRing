//
// Created by alexus on 06.02.24.
//

#include "PresenterSceneEditor.h"

#include <EngineSDK/main/resources/LoadedResources.h>
#include <EngineSDK/main/resources/ResourceRequests.h>
#include <EngineSDK/main/scene/IScene.h>
#include <dlfcn.h>

#include "EngineSDK/utils/KeyboardKey.h"
#include "IModelSceneEditor.h"
#include "IViewSceneEditor.h"
#include "project/Project.h"
#include "project/generatedFiles/SceneInfo.h"

namespace mer::editor::mvp {
std::shared_ptr<sdk::main::ILoadedResources> loadedResources;

PresenterSceneEditor::PresenterSceneEditor(const std::shared_ptr<IViewSceneEditor> &pViewSceneEditor,
										   const std::shared_ptr<IModelSceneEditor> &pModelSceneEditor)
	: viewSceneEditor(pViewSceneEditor), modelSceneEditor(pModelSceneEditor) {

	viewSceneEditor->connectRealize([this] {
		auto project = modelSceneEditor->getProject();
		if (project->getEditorLibLoading()) {
			notifyLoadingStarted();
		} else {
			if (const sdk::utils::ReportMessagePtr msg = project->getEditorLibError()) notifyLoadingStopped(msg);
			else
				notifyLoadingStopped(nullptr);
		}
		project->connectOnEditorLibLoadingSignal(sigc::mem_fun(*this, &PresenterSceneEditor::notifyLoadingStarted));
		project->connectOnEditorLibLoadedSignal(sigc::mem_fun(*this, &PresenterSceneEditor::notifyLoadingStopped));

		viewSceneEditor->connectKeyPressedSignal(
			[this](guint /*pKeyVal*/, guint pKeyCode, const Gdk::ModifierType pState) {
				if (!modelSceneEditor->isInteractive()) return false;
				const auto key = static_cast<sdk::utils::KeyboardKey>(pKeyCode);
				if (const auto scene = modelSceneEditor->getScene())
					scene->onKeyChanged(key, true, convertToModifierKeys(pState));
				return false;
			});

		viewSceneEditor->connectKeyReleasedSignal(
			[this](guint /*pKeyVal*/, guint pKeyCode, const Gdk::ModifierType pState) {
				const auto key = static_cast<sdk::utils::KeyboardKey>(pKeyCode);
				if (key == sdk::utils::KeyboardKey::KEY_F2)
					modelSceneEditor->setInteractive(!modelSceneEditor->isInteractive());
				if (!modelSceneEditor->isInteractive()) return;
				if (const auto scene = modelSceneEditor->getScene())
					scene->onKeyChanged(key, false, convertToModifierKeys(pState));
			});
	});

	viewSceneEditor->connectUnrealize([this] {
		viewSceneEditor->makeCurrent();
		try {
			viewSceneEditor->throwIfError();

			modelSceneEditor->setScene(nullptr);
			loadedResources = nullptr;
		} catch (const Gdk::GLError &gle) {
			std::cerr << "An error occured making the context current during unrealize" << std::endl;
			std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
		}
	});
	viewSceneEditor->connectRender([this](const Glib::RefPtr<Gdk::GLContext> & /*pContext*/) {
		auto scene = modelSceneEditor->getScene().get();
		if (scene) scene->render();

		viewSceneEditor->executeInMainThread(
			[this](const std::promise<void> & /*pPromise*/) { viewSceneEditor->redraw(); });
		return true;
	});
	viewSceneEditor->connectResize([this](const int pWidth, const int pHeight) {
		if (const auto scene = modelSceneEditor->getScene()) scene->resize(pWidth, pHeight);
	});
	viewSceneEditor->connectCursorMovedSignal([this](const double pX, const double pY) {
		if (!modelSceneEditor->isInteractive()) return;
		if (const auto scene = modelSceneEditor->getScene()) { scene->onCursorPosChanged(pX, pY); }
	});
}

PresenterSceneEditor::operator Gtk::Widget&() { return viewSceneEditor->getMainWidget(); }

void PresenterSceneEditor::notifyLoadingStarted() const {
	viewSceneEditor->executeInMainThread(
		[this](const std::promise<void> & /*pPromise*/) { viewSceneEditor->onLoadingStarted(); });
}

void PresenterSceneEditor::notifyLoadingStopped(const sdk::utils::ReportMessagePtr &pError) const {
	sdk::utils::ReportMessagePtr error = !pError ? loadScene() : pError;
	viewSceneEditor->executeInMainThread([this, error](const std::promise<void> & /*pPromise*/) {
		viewSceneEditor->redraw();
		viewSceneEditor->emitResize();
		viewSceneEditor->onLoadingStopped(error);
	});
}

sdk::utils::ReportMessagePtr PresenterSceneEditor::loadScene() const {
	auto sceneInfo = modelSceneEditor->getSceneInfo();
	auto functionName = "load" + sceneInfo->getName();
	auto project = modelSceneEditor->getProject();
	auto func = dlsym(project->getEditorLib(), functionName.c_str());
	if (!func) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to get scene entry point");
		msg->addInfoLine("Function name: {}", functionName);
		msg->addInfoLine("Scene name: {}", sceneInfo->getName());
		msg->addInfoLine("Error: {}", dlerror());
		return msg;
	}
	sdk::main::IScene* (*startFunc)() = reinterpret_cast<sdk::main::IScene* (*) ()>(func);
	std::shared_ptr<sdk::main::IScene> scene;
	sdk::utils::Logger::out("test1");
	viewSceneEditor->makeCurrent();
	sdk::utils::Logger::out("test2");
	try {

		scene = std::shared_ptr<sdk::main::IScene>(startFunc());
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to get scene entry point");
		msg->setMessage("Load function thrown an exception");
		msg->addInfoLine("Function name: {}", functionName);
		msg->addInfoLine("Scene name: {}", sceneInfo->getName());
		return msg;
	}
	if (!scene) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to get scene entry point");
		msg->setMessage("Load function returned nullptr");
		msg->addInfoLine("Function name: {}", functionName);
		msg->addInfoLine("Scene name: {}", sceneInfo->getName());
		return msg;
	}

	auto requests = std::make_shared<sdk::main::ResourceRequests>();
	if (auto msg = scene->preloadScene(requests)) return msg;
	auto sdk = project->getEditorSdkLib();
	void* sym = dlsym(sdk, "_ZN3mer3sdk4main15LoadedResources6createEv");
	loadedResources = reinterpret_cast<std::shared_ptr<sdk::main::ILoadedResources> (*)()>(sym)();
	auto resources = loadedResources->executeRequests(requests, scene);

	scene->setResources(resources);

	if (auto msg = scene->initScene()) return msg;

	modelSceneEditor->setScene(scene);
	return nullptr;
}

sdk::utils::ModifierKeys PresenterSceneEditor::convertToModifierKeys(const Gdk::ModifierType &pState) {
	using namespace sdk::utils;
	uint8_t mods = 0;
	if ((pState & Gdk::ModifierType::SHIFT_MASK) == Gdk::ModifierType::SHIFT_MASK) { mods |= ModifierKeys::MOD_SHIFT; }
	if ((pState & Gdk::ModifierType::CONTROL_MASK) == Gdk::ModifierType::CONTROL_MASK) {
		mods |= ModifierKeys::MOD_CONTROL;
	}
	if ((pState & Gdk::ModifierType::ALT_MASK) == Gdk::ModifierType::ALT_MASK) { mods |= ModifierKeys::MOD_ALT; }
	if ((pState & Gdk::ModifierType::SUPER_MASK) == Gdk::ModifierType::SUPER_MASK) { mods |= ModifierKeys::MOD_SUPER; }
	if ((pState & Gdk::ModifierType::LOCK_MASK) == Gdk::ModifierType::LOCK_MASK) {
		mods |= ModifierKeys::MOD_CAPS_LOCK;
	}
	return ModifierKeys(mods);
}
} // namespace mer::editor::mvp
