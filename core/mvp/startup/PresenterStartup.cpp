//
// Created by alexus on 25.01.24.
//

#include "PresenterStartup.h"

#include <GameEngine.h>
#include <Globals.h>
#include <mvp/creatingProject/ModelCreatingProject.h>
#include <mvp/creatingProject/PresenterCreatingProject.h>
#include <mvp/creatingProject/ProjectCreatingWindow.h>
#include <mvp/main/MainWindow.h>
#include <mvp/main/ModelMain.h>
#include <mvp/main/PresenterMain.h>
#include <project/Project.h>

namespace MVP_CORE {
PresenterStartup::PresenterStartup(const std::shared_ptr<IViewStartup> &pView,
								   const std::shared_ptr<IModelStartup> &pModel)
	: view(pView), model(pModel) {
	view->connectNewProjectClickSignal([this] { view->switchMode(1); });
	view->connectCancelClickSignal([this] { view->switchMode(0); });
	view->hideError();
	view->connectNameChangedSignal([this] {
		const auto name = view->getName();
		model->setName(name);
		model->setPath(Globals::getProjectsPath() / name);
		view->setPath(model->getPath().string());
	});
	view->connectPathChangedSignal([this] {
		const std::filesystem::path path = view->getPath();
		model->setPath(path);
		//TODO Add checking for project file
		if (exists(path) && !is_empty(path)) {
			view->showError("Директория не пуста. Продолжение создания проекта может привести к ошибкам.");
		} else {
			view->hideError();
		}
	});
	view->setName(model->getName());
	view->setPath(model->getPath());

	view->connectIconReleasedSignal([this](Gtk::Entry::IconPosition /*pPosition*/) {
		view->showFolderChooserDialog(model->getPath(),
									  sigc::bind([this](const std::shared_ptr<Gio::AsyncResult> &pResult,
														const std::shared_ptr<Gtk::FileDialog> &pDialog) {
										  try {
											  const auto folder = pDialog->select_folder_finish(pResult);
											  view->setPath(folder->get_path());
											  //set_sensitive(true);
										  } catch (const Gtk::DialogError &err) {
											  if (err.code() != Gtk::DialogError::DISMISSED) {
												  using namespace std::string_literals;
												  view->showError("No folder selected. "s + err.what());
											  }
										  } catch (const Glib::Error &err) {
											  using namespace std::string_literals;
											  view->showError("Unexpected exception. "s + err.what());
										  }
									  }));
	});

	view->connectCreateClickSignal([this] {
		const std::filesystem::path &path = model->getPath();
		try {
			if (!exists(path)) create_directories(path);

		} catch (...) {
			const auto msg = engine::utils::ReportMessage::create();
			msg->setTitle("Failed to create directory for project");
			msg->setMessage("Exception occurred");
			engine::utils::Logger::error(msg);
			return;
		}
		auto project = project::Project::create();
		project->setProjectName(model->getName());
		project->setProjectPath(path);
		if (const auto msg = initProject(project)) return engine::utils::Logger::error(msg);
		openProjectCreatingWindow(project);
	});

	view->connectOpenProjectClickSignal([this] {
		view->showFileChooserDialog(model->getPath(),
									sigc::bind([this](const std::shared_ptr<Gio::AsyncResult> &pResult,
													  const std::shared_ptr<Gtk::FileDialog> &pDialog) {
										try {
											const auto folder = pDialog->open_finish(pResult);
											const auto project = project::Project::create();
											const std::filesystem::path path = folder->get_path();
											project->setProjectName(path.stem());
											project->setProjectPath(path.parent_path());
											if (const auto msg = initProject(project))
												return engine::utils::Logger::error(msg);
											if (const auto msg = project->loadProject())
												return engine::utils::Logger::error(msg);
											openMainWindow(project);
										} catch (const Gtk::DialogError &err) {
											if (err.code() != Gtk::DialogError::DISMISSED) {
												using namespace std::string_literals;
												view->showError("No file selected. "s + err.what());
											}
										} catch (const Glib::Error &err) {
											using namespace std::string_literals;
											view->showError("Unexpected exception. "s + err.what());
										}
									}));
	});
}

engine::utils::ReportMessageUPtr PresenterStartup::initProject(const std::shared_ptr<project::Project> &pProject) {

	if (auto msg = pProject->openDatabase()) return msg;

	pProject->initProject();
	return nullptr;
}

void PresenterStartup::openProjectCreatingWindow(const std::shared_ptr<project::Project> &pProject) const {
	auto projectCreatingWindow = std::make_shared<ProjectCreatingWindow>();
	auto modelCreatingProject = std::make_shared<ModelCreatingProject>();
	modelCreatingProject->setProject(pProject);
	const auto presenter = std::make_shared<PresenterCreatingProject>(projectCreatingWindow, modelCreatingProject);

	projectCreatingWindow->set_visible();

	view->addWindow(projectCreatingWindow);
	presenter->runTasks();
	ui::GameEngine::addWindow(presenter);

	view->closeWindow();
}

void PresenterStartup::openMainWindow(const std::shared_ptr<project::Project> &pProject) const {

	engine::utils::ReportMessageUPtr msg;
	auto viewMain = MainWindow::create(pProject, msg);
	if (msg) return engine::utils::Logger::error(msg);
	auto modelMain = std::make_shared<ModelMain>();
	modelMain->setProject(pProject);
	const auto presenter = std::make_shared<PresenterMain>(viewMain, modelMain);

	viewMain->set_visible();

	view->addWindow(viewMain);
	ui::GameEngine::addWindow(presenter);

	view->closeWindow();
}
}
