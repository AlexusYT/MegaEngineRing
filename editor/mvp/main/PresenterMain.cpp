//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 26.01.24.
//

#include "PresenterMain.h"

#include <future>
#include <mvp/main/IModelMain.h>
#include <mvp/main/IViewMain.h>
#include <nlohmann/json.hpp>
#include <project/Project.h>

#include "EngineSDK/extensions/ExtensionRegistry.h"
#include "EngineSDK/resources/ResourceType.h"
#include "EngineSDK/resources/materials/MaterialResource.h"
#include "EngineSDK/resources/models/Model3DResource.h"
#include "EngineSDK/resources/textures/TextureResource.h"
#include "Globals.h"
#include "PanedLayoutTab.h"
#include "centerWindow/PresenterCenterWindow.h"
#include "centerWindow/ViewCenterWindow.h"
#include "editors/sceneEditor/ModelSceneEditor.h"
#include "editors/sceneEditor/PresenterSceneEditor.h"
#include "editors/sceneEditor/ViewSceneEditor.h"
#include "editors/sceneEditor/explorerObjects/ExplorerObject.h"
#include "editors/sceneEditor/explorerObjects/SceneExplorerObject.h"
#include "mvp/ApplicationController.h"
#include "mvp/contexts/ApplicationContext.h"
#include "mvp/contexts/MultiPanedContext.h"
#include "mvp/resourceEditor/EditingResourceList.h"
#include "mvp/resourceEditor/ModelResourceEditor.h"
#include "mvp/resourceEditor/PresenterResourceEditor.h"
#include "mvp/resourceEditor/ViewResourceEditor.h"
#include "mvp/resourceSelection/ModelResourceSelection.h"
#include "mvp/resourceSelection/PresenterResourceSelection.h"
#include "mvp/resourceSelection/ViewResourceSelection.h"
#include "objectProperties/ModelObjectProperties.h"
#include "objectProperties/PresenterObjectProperties.h"
#include "objectProperties/ViewObjectProperties.h"
#include "project/LoadedScene.h"
#include "projectExplorer/ModelProjectExplorer.h"
#include "projectExplorer/PresenterProjectExplorer.h"
#include "projectExplorer/ViewProjectExplorer.h"
#include "treeObjectWindow/ModelObjectsTree.h"
#include "treeObjectWindow/PresenterObjectsTree.h"
#include "treeObjectWindow/ViewObjectsTree.h"

namespace mer::editor::mvp {
PresenterMain::PresenterMain(const std::shared_ptr<IViewMain> &pViewMain, const std::shared_ptr<IModelMain> &pModelMain)
	: viewMain(pViewMain), modelMain(pModelMain) {

	pViewMain->setPresenter(this);
	modelMain->setPresenter(this);
	auto project = modelMain->getProject();
	project->connectOnErrorSignal(sigc::mem_fun(*viewMain, &IViewMain::reportError));

	viewMain->setWindowTitle("Game engine editor - " + project->getProjectName());

	modelMain->loadLayoutsFile();

	auto r = viewMain->connectKeyPressedSignal(
		[this, project](const guint /*pKeyVal*/, guint pKeyCode, const Gdk::ModifierType pState) {
			if (pKeyCode == 39 && (pState & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK |
											 Gdk::ModifierType::ALT_MASK)) == Gdk::ModifierType::CONTROL_MASK) {
				std::thread([this, project] {
					if (const auto msg = project->saveProject()) project->errorOccurred(msg);
				}).detach();

				return true;
			}
			sdk::Logger::out("{}", pKeyCode);
			return false;
		});


	viewMain->addActionGroup("win", project->getActionGroups());
}

PresenterMain::~PresenterMain() {}

void PresenterMain::logMessage(int /*pId*/, const std::string & /*pMessage*/) const {
	// TODO viewMain->executeInMainThread([pMessage, this, pId](const std::promise<void> & /*pPromise*/) {
	// 	viewMain->addLogMessage(pId, Glib::Markup::escape_text(pMessage));
	// });
}

void PresenterMain::logError(int /*pId*/, const std::string & /*pMessage*/) const {
	// TODO viewMain->executeInMainThread([pMessage, this, pId](const std::promise<void> & /*pPromise*/) {
	// 	viewMain->addLogMessage(pId, "<span foreground=\"red\">" + Glib::Markup::escape_text(pMessage) + "</span>\n");
	// });
}

void PresenterMain::displayError(const sdk::ReportMessagePtr &pMsg) { sdk::Logger::error(pMsg); }

void PresenterMain::onLayoutLoadFatal() { /*TODO_IMPLEMENT_ME();*/ }

void PresenterMain::onPanedLayoutTabsChanged() { viewMain->setTabs(modelMain->getPanedLayoutTabs()); }

void PresenterMain::onCurrentTabChanged() { viewMain->openTab(modelMain->getCurrentTab()); }

void PresenterMain::run() {
	auto project = modelMain->getProject();
	viewMain->openView();
	{
		auto modelProjectExplorer =
			std::make_shared<ModelProjectExplorer>(modelMain->getProject()->getProjectPath() / "data");
		presenterProjectExplorer = std::make_shared<PresenterProjectExplorer>(modelProjectExplorer);
		getAppController()->run(presenterProjectExplorer);
		presenters.push_back(presenterProjectExplorer);

		loadedScene = std::make_shared<project::LoadedScene>();
		loadedScene->connectErrorOccurred([this](auto pMsg) { displayError(pMsg); });
		loadedScene->setRunDirectory(project->getProjectPath());
		loadedScene->setupResourcesContext(viewMain->getResourcesContext());
		auto modelSceneEditor = std::make_shared<ModelSceneEditor>();
		modelSceneEditor->setLoadedScene(loadedScene);
		auto presenterSceneEditor = std::make_shared<PresenterSceneEditor>(modelSceneEditor);
		getAppController()->run(presenterSceneEditor);
		presenters.push_back(presenterSceneEditor);

		auto modelObjectsTree = std::make_shared<ModelObjectsTree>();
		modelObjectsTree->setLoadedScene(loadedScene);
		auto presenterObjectsTree = std::make_shared<PresenterObjectsTree>(modelObjectsTree);
		getAppController()->run(presenterObjectsTree);
		presenters.push_back(presenterObjectsTree);

		auto modelObjectProperties = std::make_shared<ModelObjectProperties>();
		modelObjectProperties->setLoadedScene(loadedScene);
		auto presenterObjectProperties = std::make_shared<PresenterObjectProperties>(modelObjectProperties);
		getAppController()->run(presenterObjectProperties);
		presenters.push_back(presenterObjectProperties);

		editingResources = std::make_shared<EditingResourceList>();
		editingResources->setupResourcesContext(viewMain->getResourcesContext());

		auto model = std::make_shared<ModelResourceEditor>();
		model->setEditingResources(editingResources);
		model->setPathToDataDir(project->getProjectDataPath());
		auto presenter = std::make_shared<PresenterResourceEditor>(model);
		getAppController()->run(presenter);
		presenters.push_back(presenter);
	}


	//auto viewCenterWindow = std::make_shared<ViewCenterWindow>(getAppController()->getAppContext());
	/*auto viewCenterWindow = std::make_shared<ViewCenterWindow>(PanedContext::create(viewMain->getTopPaned(), true));
	auto modelCenterWindow = std::make_shared<ModelCenterWindow>();
	presenterCenterWindow = std::make_shared<PresenterCenterWindow>(viewCenterWindow, modelCenterWindow);
	getAppController()->run(presenterCenterWindow);

	presenterProjectExplorer->setEntrySelectionChanged([this](ui::ProjectExplorerEntry* pEntry) {
		if (auto tab = dynamic_cast<ITab*>(pEntry)) presenterCenterWindow->openTab(tab);
	});*/
}

void PresenterMain::stop() {

	for (auto presenter: presenters) getAppController()->stop(presenter.get());
	presenters.clear();
	viewMain->closeView();
	loadedScene.reset();
}

std::shared_ptr<IView> PresenterMain::createView(const IPresenter* pPresenter,
												 const std::shared_ptr<MultiPanedContext> &pContext) {

	if (dynamic_cast<const PresenterProjectExplorer*>(pPresenter))
		return std::make_shared<ViewProjectExplorer>(pContext);
	if (dynamic_cast<const PresenterSceneEditor*>(pPresenter)) return std::make_shared<ViewSceneEditor>(pContext);
	if (dynamic_cast<const PresenterObjectsTree*>(pPresenter)) return std::make_shared<ViewObjectsTree>(pContext);
	if (dynamic_cast<const PresenterObjectProperties*>(pPresenter))
		return std::make_shared<ViewObjectProperties>(pContext);
	if (dynamic_cast<const PresenterResourceEditor*>(pPresenter)) return ViewResourceEditor::create(pContext);
	return nullptr;
}

void PresenterMain::readJsonForTab(int32_t pIndex,
								   const sigc::slot<void(const sdk::ReportMessagePtr &pError)> &pCallback) {
	std::thread([this, pIndex, pCallback]() {
		auto tabs = modelMain->getPanedLayoutTabs();
		if (tabs.size() <= static_cast<uint64_t>(pIndex)) {
			//Here we can offer the user to select the file themselves or reload Layouts.json file instead of displaying
			//an error
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Unable to read layout from JSON");
			msg->setMessage("No layout info for specified tab in the Layout.json file");
			msg->addInfoLine("Specified tab index: {}", pIndex);
			msg->addInfoLine("Layout.json file only has {} tabs", tabs.size());
			pCallback(msg);
			return;
		}
		std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
		auto path = std::filesystem::path(tabs.at(static_cast<uint64_t>(pIndex)).getPath());
		if (path.string().starts_with("_builtin_")) {
			auto resourcePath = "/paned-layouts/" + path.filename().string();
			if (!Gio::Resource::get_file_exists_global_nothrow(resourcePath)) {
				auto msg = sdk::ReportMessage::create();
				msg->setTitle("Unable to read layout from JSON");
				msg->setMessage("Specified builtin layout does not exist");
				msg->addInfoLine("Looking for layout at {}", resourcePath);
				pCallback(msg);
			}
			try {
				auto resourceStream = Gio::Resource::lookup_data_global(resourcePath);
				gsize size;
				auto data = static_cast<const char*>(resourceStream->get_data(size));
				*json = nlohmann::json::parse(std::string(data, size));
			} catch (...) {
				auto msg = sdk::ReportMessage::create();
				msg->setTitle("Unable to read layout from JSON");
				msg->setMessage("Exception occurred while reading layout file for tab");
				msg->addInfoLine("Looking for layout at {}", resourcePath);
				pCallback(msg);
			}

		} else {
			std::filesystem::path layoutFilePath;
			layoutFilePath = Globals::getConfigPath() / "paned-layouts" / path;

			try {
				std::ifstream stream = std::ifstream(layoutFilePath);
				stream.exceptions(std::_S_badbit | std::_S_failbit);
				stream >> *json;
			} catch (...) {
				auto msg = sdk::ReportMessage::create();
				msg->setTitle("Unable to read layout from JSON");
				msg->setMessage("Exception occurred while reading layout file for tab");
				msg->addInfoLine("Specified tab index: {}", pIndex);
				msg->addInfoLine("Path to the layout file: {}", layoutFilePath.string());
				pCallback(msg);
				return;
			}
		}


		viewMain->executeInMainThread([pCallback, pIndex, json, this](const std::promise<void> &) {
			auto paned = viewMain->getMultiPanedByIndex(pIndex);
			if (paned) { pCallback(paned->importFromJson(json, getAppController())); }
		});
	}).detach();
}

void PresenterMain::selectResourceForProperty(sdk::PropertyBase* pProperty) {

	auto model = std::make_shared<ModelResourceSelection>();
	model->setPropertyBase(pProperty);
	model->setLoadedScene(loadedScene);
	sdk::ReportMessagePtr msg;
	auto view = ViewResourceSelection::create(ApplicationContext::create(getAppController()->getApp()), msg);
	if (!view) {
		displayError(msg);
		return;
	}
	presenterProjectExplorer->addView(view->getExplorer());
	auto presenter = std::make_shared<PresenterResourceSelection>(model, view);
	getAppController()->run(presenter);
}

void PresenterMain::addExtension(const std::string &pExtensionName) {
	auto obj = loadedScene->getSelectedObject();
	if (auto element = dynamic_cast<SceneExplorerObject*>(obj)) {
		loadedScene->addExtension(element->getObject(), pExtensionName, pExtensionName);
	}
}

void PresenterMain::selectObject(ExplorerObject* pObjectToSelect) { loadedScene->selectObject(pObjectToSelect); }

void PresenterMain::createObject(ExplorerObject* /*pParentObject*/) {
	if (!loadedScene->hasScene()) return;
	loadedScene->addObject();
}

void PresenterMain::removeObject(ExplorerObject* pObjectToRemove) {
	if (auto element = dynamic_cast<SceneExplorerObject*>(pObjectToRemove)) {
		loadedScene->removeObject(element->getObject());
	}
}

void PresenterMain::removeExtension(sdk::Extension* pExtensionToRemove) {
	loadedScene->removeExtension(pExtensionToRemove);
}

void PresenterMain::openFile(const std::filesystem::path &pPathToFile) {
	if (!pPathToFile.has_extension()) return;
	auto ext = pPathToFile.extension().string();
	if (ext == ".enscene")
		if (const auto msg = loadedScene->load(pPathToFile)) { displayError(msg); }
	if (ext == ".enmodel" || ext == ".entex" || ext == ".enmat") {
		auto dataPath = modelMain->getProject()->getProjectDataPath();
		auto resourceUri = relative(pPathToFile, dataPath);
		editingResources->loadResource(resourceUri);
	}
}

void PresenterMain::createResource(const std::filesystem::path &pPathToCreate, const sdk::ResourceType pType) {
	auto pathToCreate = is_directory(pPathToCreate) ? pPathToCreate : pPathToCreate.parent_path();
	std::shared_ptr<sdk::IResource> resource{};
	switch (pType) {

		case sdk::ResourceType::SHADER:
		case sdk::ResourceType::NONE: return;
		case sdk::ResourceType::MODEL:
			resource = std::dynamic_pointer_cast<sdk::IResource>(sdk::Model3DResource::create());
			break;
		case sdk::ResourceType::TEXTURE:
			resource = std::dynamic_pointer_cast<sdk::IResource>(sdk::TextureResource::create());
			break;
		case sdk::ResourceType::MATERIAL:
			resource = std::dynamic_pointer_cast<sdk::IResource>(sdk::MaterialResource::create());
			break;
	}
	std::filesystem::path uri = "/";
	auto root = modelMain->getProject()->getProjectDataPath();
	if (pathToCreate != "/" && pathToCreate != root) { uri /= relative(pathToCreate, root); }
	resource->setResourceUri(uri);
	editingResources->addResource(resource);
}

void PresenterMain::createScene(const std::filesystem::path &pPathToCreate) {
	std::filesystem::path pathToCreate = !is_directory(pPathToCreate) ? pPathToCreate.parent_path() : pPathToCreate;
	int id = 0;
	std::string sceneName;
	while (true) {
		sceneName = std::format("Scene{}.enscene", id++);
		if (!exists(pathToCreate / sceneName)) break;
	}
	if (const auto msg = loadedScene->load(pathToCreate / sceneName)) { displayError(msg); }
}

void PresenterMain::createScript(const std::filesystem::path &pPathToCreate) {
	const auto msg = sdk::ReportMessage::create();
	msg->setTitle("Failed to create script");
	msg->setMessage("Not implemented yet");
	msg->addInfoLine("Path: {}", pPathToCreate.string());
	displayError(msg);
}

void PresenterMain::createDirectory(const std::filesystem::path &pPathToCreate) {
	const auto msg = sdk::ReportMessage::create();
	msg->setTitle("Failed to create directory");
	msg->setMessage("Not implemented yet");
	msg->addInfoLine("Path: {}", pPathToCreate.string());
	displayError(msg);
}

void PresenterMain::renameFile(const std::filesystem::path &pPathToRemove) {

	const auto msg = sdk::ReportMessage::create();
	msg->setTitle("Failed to rename file");
	msg->setMessage("Not implemented yet");
	msg->addInfoLine("File path: {}", pPathToRemove.string());
	displayError(msg);
}

void PresenterMain::deleteFile(const std::filesystem::path &pPathToDelete) {
	using namespace std::filesystem;

	Glib::RefPtr<Gtk::AlertDialog> dialog = Gtk::AlertDialog::create();
	std::error_code error;
	auto type = status(pPathToDelete, error).type();
	if (error) {

		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to delete file");
		msg->setMessage(error.message());
		msg->addInfoLine("File path: {}", pPathToDelete.string());

		displayError(msg);
	}

	std::string name;
	if (type == file_type::directory) {
		name = "directory";
	} else if (type == file_type::regular) {
		name = "file";
	} else if (type == file_type::symlink) {
		name = "symlink";
	} else {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to delete the file");
		msg->setMessage("Trying to delete the file with the unknown type");
		msg->addInfoLine("Type must be directory or file");
		msg->addInfoLine("File path: {}", pPathToDelete.string());

		displayError(msg);
		return;
	}

	dialog->set_message(std::format("Confirm deleting {} {}", name, pPathToDelete.filename().string()));
	dialog->set_detail("This operation cannot be undone");
	dialog->set_modal();
	dialog->set_buttons({"Delete", "Cancel"});
	dialog->set_default_button(1);
	dialog->set_cancel_button(1);
	auto lambda = [dialog, pPathToDelete, this](const std::shared_ptr<Gio::AsyncResult> &pResult) {
		if (const int btn = dialog->choose_finish(pResult); btn == 0) {
			if (std::error_code removeError; !remove_all(pPathToDelete, removeError)) {
				const auto msg = sdk::ReportMessage::create();
				msg->setTitle("Failed to delete file");
				msg->setMessage(removeError.message());
				msg->addInfoLine("File path: {}", pPathToDelete.string());

				displayError(msg);
				return;
			}
			path uri = "/";
			auto root = modelMain->getProject()->getProjectDataPath();
			if (pPathToDelete != "/" && pPathToDelete != root) { uri = relative(pPathToDelete, root); }
			auto ext = pPathToDelete.extension().string();
			if (ext == ".enscene") { loadedScene->unload(); }
			if (ext == ".enmodel" || ext == ".entex" || ext == ".enmat") { editingResources->deleteResource(uri); }
		}
	};
	dialog->choose(*viewMain->getWindow(), lambda);
}

void PresenterMain::showInFiles(const std::filesystem::path &pPathToShow) {
	sdk::Logger::info("Showing the file {}", pPathToShow.string());
	auto launcher = Gtk::FileLauncher::create(Gio::File::create_for_path(pPathToShow));
	launcher->open_containing_folder([launcher, pPathToShow, this](const std::shared_ptr<Gio::AsyncResult> &pResult) {
		try {
			if (launcher->open_containing_folder_finish(pResult)) {
				sdk::Logger::out("File showed");

			} else {

				auto msg = sdk::ReportMessage::create();
				msg->setTitle("Failed to show file in system file manager");
				msg->setMessage("Unknown error");
				msg->addInfoLine("File path: {}", pPathToShow.string());
				displayError(msg);
			}
		} catch (...) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to show file in system file manager");
			msg->setMessage("Exception occurred");
			msg->addInfoLine("File path: {}", pPathToShow.string());
			displayError(msg);
		}
	});
}

void PresenterMain::onViewClosed() { getAppController()->stop(this); }
} // namespace mer::editor::mvp