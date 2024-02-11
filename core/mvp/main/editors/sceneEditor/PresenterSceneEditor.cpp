//
// Created by alexus on 06.02.24.
//

#include "PresenterSceneEditor.h"

#include <EngineSDK/main/resources/LoadedResources.h>
#include <EngineSDK/main/resources/ResourceRequests.h>
#include <EngineSDK/main/scene/IScene.h>
#include <cxxabi.h>
#include <dlfcn.h>

#include "IModelSceneEditor.h"
#include "IViewSceneEditor.h"
#include "project/Project.h"
#include "project/generatedFiles/SceneInfo.h"

namespace n::core::mvp {
PresenterSceneEditor::PresenterSceneEditor(const std::shared_ptr<IViewSceneEditor> &pViewSceneEditor,
										   const std::shared_ptr<IModelSceneEditor> &pModelSceneEditor)
	: viewSceneEditor(pViewSceneEditor), modelSceneEditor(pModelSceneEditor) {
	auto project = modelSceneEditor->getProject();
	if (project->getEditorLibLoading()) {
		notifyLoadingStarted();
	} else {
		if (const engine::utils::ReportMessagePtr msg = project->getEditorLibError()) notifyLoadingStopped(msg);
		else
			notifyLoadingStopped(nullptr);
	}
	project->connectOnEditorLibLoadingSignal(sigc::mem_fun(*this, &PresenterSceneEditor::notifyLoadingStarted));
	project->connectOnEditorLibLoadedSignal(sigc::mem_fun(*this, &PresenterSceneEditor::notifyLoadingStopped));
	viewSceneEditor->connectRender([](const Glib::RefPtr<Gdk::GLContext> & /*pContext*/) { return true; });
}

PresenterSceneEditor::operator Gtk::Widget&() { return viewSceneEditor->getMainWidget(); }

void PresenterSceneEditor::notifyLoadingStarted() const {
	viewSceneEditor->executeInMainThread(
		[this](const std::promise<void> & /*pPromise*/) { viewSceneEditor->onLoadingStarted(); });
}

void PresenterSceneEditor::notifyLoadingStopped(const engine::utils::ReportMessagePtr &pError) const {
	engine::utils::ReportMessagePtr error = !pError ? loadScene() : pError;

	viewSceneEditor->executeInMainThread(
		[this, error](const std::promise<void> & /*pPromise*/) { viewSceneEditor->onLoadingStopped(error); });
}

engine::utils::ReportMessagePtr PresenterSceneEditor::loadScene() const {
	auto sceneInfo = modelSceneEditor->getSceneInfo();
	auto functionName = "load" + sceneInfo->getName();
	auto project = modelSceneEditor->getProject();
	auto func = dlsym(project->getEditorLib(), functionName.c_str());
	if (!func) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to get scene entry point");
		msg->addInfoLine("Function name: {}", functionName);
		msg->addInfoLine("Scene name: {}", sceneInfo->getName());
		msg->addInfoLine("Error: {}", dlerror());
		return msg;
	}
	sdk::main::IScene* (*startFunc)() = reinterpret_cast<sdk::main::IScene* (*) ()>(func);
	std::shared_ptr<sdk::main::IScene> scene;
	try {

		scene = std::shared_ptr<sdk::main::IScene>(startFunc());
	} catch (...) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to get scene entry point");
		msg->setMessage("Load function thrown an exception");
		msg->addInfoLine("Function name: {}", functionName);
		msg->addInfoLine("Scene name: {}", sceneInfo->getName());
		return msg;
	}
	if (!scene) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to get scene entry point");
		msg->setMessage("Load function returned nullptr");
		msg->addInfoLine("Function name: {}", functionName);
		msg->addInfoLine("Scene name: {}", sceneInfo->getName());
		return msg;
	}
	modelSceneEditor->setScene(scene);

	auto requests = std::make_shared<sdk::main::ResourceRequests>();
	if (auto msg = scene->preloadScene(requests)) return msg;
	auto sdk = project->getEditorSdkLib();
	void* sym = dlsym(sdk, "_ZN1n3sdk4main15LoadedResources6createEv");
	std::shared_ptr<sdk::main::ILoadedResources> loadedResources =
		reinterpret_cast<std::shared_ptr<sdk::main::ILoadedResources> (*)()>(sym)();
	auto resources = loadedResources->executeRequests(requests, scene);

	scene->setResources(resources);

	if (auto msg = scene->initScene()) return msg;
	return nullptr;
}
} // namespace n::core::mvp
