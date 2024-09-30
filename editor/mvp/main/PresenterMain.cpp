// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 26.01.24.
//

#include "PresenterMain.h"

#include <dlfcn.h>
#include <future>
#include <mvp/main/IModelMain.h>
#include <mvp/main/IViewMain.h>
#include <project/Project.h>

#include "Globals.h"
#include "centerWindow/ITab.h"
#include "centerWindow/ModelCenterWindow.h"
#include "centerWindow/PresenterCenterWindow.h"
#include "centerWindow/ViewCenterWindow.h"
#include "editors/sceneEditor/ModelSceneEditor.h"
#include "editors/sceneEditor/PresenterSceneEditor.h"
#include "editors/sceneEditor/ViewSceneEditor.h"
#include "editors/sceneEditor/explorerObjects/ExplorerObject.h"
#include "editors/sceneEditor/explorerObjects/SceneExplorerObject.h"
#include "mvp/ApplicationController.h"
#include "mvp/contexts/MultiPanedContext.h"
#include "objectProperties/ModelObjectProperties.h"
#include "objectProperties/PresenterObjectProperties.h"
#include "objectProperties/ViewObjectProperties.h"
#include "project/LoadedScene.h"
#include "project/Sdk.h"
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
	auto project = modelMain->getProject();
	project->connectOnErrorSignal(sigc::mem_fun(*viewMain, &IViewMain::reportError));
	//TODO move to project
	std::string path = Globals::getSdk() / "lib/MegaEngineSDK.so";
	errno = 0;
	sdk::utils::ReportMessagePtr errorMsg;
	auto sdk = project::Sdk::create(path, errorMsg);
	if (!sdk) {
		project->errorOccurred(errorMsg);
	} else {
		sdk->initExtensionRegistry();
		project->setEditorSdkLib(sdk);
	}
	viewMain->setWindowTitle("Game engine editor - " + project->getProjectName());

	viewMain->getMultiPaned()->connectCreateWidgetSignal(
		[](const IPresenter* pPresenter, const std::shared_ptr<MultiPanedContext> &pContext) -> std::shared_ptr<IView> {
			if (dynamic_cast<const PresenterProjectExplorer*>(pPresenter))
				return std::make_shared<ViewProjectExplorer>(pContext);
			if (dynamic_cast<const PresenterSceneEditor*>(pPresenter))
				return std::make_shared<ViewSceneEditor>(pContext);
			if (dynamic_cast<const PresenterObjectsTree*>(pPresenter))
				return std::make_shared<ViewObjectsTree>(pContext);
			if (dynamic_cast<const PresenterObjectProperties*>(pPresenter))
				return std::make_shared<ViewObjectProperties>(pContext);
			return nullptr;
		});
	auto r = viewMain->connectKeyPressedSignal(
		[this, project](const guint /*pKeyVal*/, guint pKeyCode, const Gdk::ModifierType pState) {
			if (pKeyCode == 39 && (pState & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK |
											 Gdk::ModifierType::ALT_MASK)) == Gdk::ModifierType::CONTROL_MASK) {
				std::thread([this, project] {
					if (const auto msg = project->saveProject()) project->errorOccurred(msg);
				}).detach();

				return true;
			}
			sdk::utils::Logger::out("{}", pKeyCode);
			return false;
		});


	viewMain->addActionGroup("win", project->getActionGroups());
}

PresenterMain::~PresenterMain() {}

void PresenterMain::generateCmakeFiles(const sigc::slot<void(int pExitCode)> &pOnFinish) const {

	std::thread([this, pOnFinish] {
		// TODO viewMain
		// 	->executeInMainThread([this](const std::promise<void> & /*pPromise*/) {
		// 		viewMain->switchLogPage(0);
		// 		viewMain->clearLogMessage(0);
		// 	})
		// 	.wait();
		pOnFinish(modelMain->getProject()->reloadCMake([this](const std::string &pLogLine) { logMessage(0, pLogLine); },
													   [this](const std::string &pLogLine) { logError(0, pLogLine); }));
	}).detach();
}

void PresenterMain::build(const sigc::slot<void(int pExitCode)> &pOnFinish) const {

	std::thread([this, pOnFinish] {
		// TODO viewMain
		// 	->executeInMainThread([this](const std::promise<void> & /*pPromise*/) {
		// 		viewMain->switchLogPage(1);
		// 		viewMain->clearLogMessage(1);
		// 	})
		// 	.wait();
		pOnFinish(modelMain->getProject()->build([this](const std::string &pLogLine) { logMessage(1, pLogLine); },
												 [this](const std::string &pLogLine) { logError(1, pLogLine); }));
	}).detach();
}

void PresenterMain::runProject(const sigc::slot<void(int pExitCode)> &pOnFinish) const {
	std::thread([this, pOnFinish] {
		// TODO viewMain
		// ->executeInMainThread([this](const std::promise<void> & /*pPromise*/) {
		// 	viewMain->switchLogPage(2);
		// 	viewMain->clearLogMessage(2);
		// })
		// .wait();
		pOnFinish(modelMain->getProject()->run([this](const std::string &pLogLine) { logMessage(2, pLogLine); },
											   [this](const std::string &pLogLine) { logError(2, pLogLine); }));
	}).detach();
}

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

void PresenterMain::displayError(const sdk::utils::ReportMessagePtr &pMsg) { sdk::utils::Logger::error(pMsg); }

void PresenterMain::run() {
	auto project = modelMain->getProject();
	viewMain->openView();
	{
		auto paned = viewMain->getMultiPaned();
		auto modelProjectExplorer =
			std::make_shared<ModelProjectExplorer>(modelMain->getProject()->getProjectPath() / "data");
		presenterProjectExplorer = std::make_shared<PresenterProjectExplorer>(modelProjectExplorer);
		getAppController()->run(presenterProjectExplorer);
		paned->addPresenter(presenterProjectExplorer);
		auto rootWidget = paned->setRoot(presenterProjectExplorer);

		loadedScene = std::make_shared<project::LoadedScene>(project->getEditorSdkLib());
		loadedScene->connectErrorOccurred([this](auto pMsg) { displayError(pMsg); });
		loadedScene->setRunDirectory(project->getProjectPath());

		auto modelSceneEditor = std::make_shared<ModelSceneEditor>();
		modelSceneEditor->setLoadedScene(loadedScene);
		auto presenterSceneEditor = std::make_shared<PresenterSceneEditor>(modelSceneEditor);
		getAppController()->run(presenterSceneEditor);
		paned->addPresenter(presenterSceneEditor);
		auto sceneEditor = paned->splitAt(rootWidget, presenterSceneEditor, Gtk::Orientation::VERTICAL, 0.13f);

		auto modelObjectsTree = std::make_shared<ModelObjectsTree>();
		modelObjectsTree->setLoadedScene(loadedScene);
		auto presenterObjectsTree = std::make_shared<PresenterObjectsTree>(modelObjectsTree);
		getAppController()->run(presenterObjectsTree);
		paned->addPresenter(presenterObjectsTree);
		auto objectTree = paned->splitAt(sceneEditor, presenterObjectsTree, Gtk::Orientation::VERTICAL, 0.9f);

		auto modelObjectProperties = std::make_shared<ModelObjectProperties>();
		modelObjectProperties->setLoadedScene(loadedScene);
		auto presenterObjectProperties = std::make_shared<PresenterObjectProperties>(modelObjectProperties);
		getAppController()->run(presenterObjectProperties);
		paned->addPresenter(presenterObjectProperties);
		paned->splitAt(objectTree, presenterObjectProperties, Gtk::Orientation::HORIZONTAL, 0.5f);
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

	getAppController()->stop(presenterProjectExplorer.get());
	viewMain->closeView();
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

void PresenterMain::removeExtension(sdk::main::Extension* pExtensionToRemove) {
	loadedScene->removeExtension(pExtensionToRemove);
}

void PresenterMain::openFile(const std::filesystem::path &pPathToFile) {
	if (!pPathToFile.has_extension()) return;
	if (pPathToFile.extension() == ".enscene")
		if (const auto msg = loadedScene->load(pPathToFile)) { displayError(msg); }
}

void PresenterMain::createResource(const std::filesystem::path &pPathToCreate) {}

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
	const auto msg = sdk::utils::ReportMessage::create();
	msg->setTitle("Failed to create script");
	msg->setMessage("Not implemented yet");
	msg->addInfoLine("Path: {}", pPathToCreate.string());
	displayError(msg);
}

void PresenterMain::createDirectory(const std::filesystem::path &pPathToCreate) {
	const auto msg = sdk::utils::ReportMessage::create();
	msg->setTitle("Failed to create directory");
	msg->setMessage("Not implemented yet");
	msg->addInfoLine("Path: {}", pPathToCreate.string());
	displayError(msg);
}

void PresenterMain::renameFile(const std::filesystem::path &pPathToRemove) {

	const auto msg = sdk::utils::ReportMessage::create();
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

		auto msg = sdk::utils::ReportMessage::create();
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
		auto msg = sdk::utils::ReportMessage::create();
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
				const auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to delete file");
				msg->setMessage(removeError.message());
				msg->addInfoLine("File path: {}", pPathToDelete.string());

				displayError(msg);
				return;
			}
			if (pPathToDelete.extension() == ".enscene") { loadedScene->unload(); }
		}
	};
	dialog->choose(*viewMain->getWindow(), lambda);
}

void PresenterMain::showInFiles(const std::filesystem::path &pPathToShow) {
	sdk::utils::Logger::info("Showing the file {}", pPathToShow.string());
	auto launcher = Gtk::FileLauncher::create(Gio::File::create_for_path(pPathToShow));
	launcher->open_containing_folder([launcher, pPathToShow, this](const std::shared_ptr<Gio::AsyncResult> &pResult) {
		try {
			if (launcher->open_containing_folder_finish(pResult)) {
				sdk::utils::Logger::out("File showed");

			} else {

				auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to show file in system file manager");
				msg->setMessage("Unknown error");
				msg->addInfoLine("File path: {}", pPathToShow.string());
				displayError(msg);
			}
		} catch (...) {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to show file in system file manager");
			msg->setMessage("Exception occurred");
			msg->addInfoLine("File path: {}", pPathToShow.string());
			displayError(msg);
		}
	});
}
} // namespace mer::editor::mvp