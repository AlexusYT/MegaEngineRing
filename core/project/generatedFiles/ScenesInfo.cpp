//
// Created by alexus on 27.01.24.
//

#include "ScenesInfo.h"

#include <project/Project.h>

namespace PROJECT_CORE {
ScenesInfo::ScenesInfo(const std::shared_ptr<Project> &pProject) : GeneratedFileEntry(pProject) {
	setEntryChildren(Gio::ListStore<ProjectExplorerEntry>::create());
	setName("Сцены");
	auto menuAdd = Gio::Menu::create();
	menuAdd->append("3D сцена", "win.scene.add.scene3d");
	Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();
	menu->append_submenu("Добавить сцену", menuAdd);
	setContextMenu(menu);
}

void ScenesInfo::getActionGroup(const Glib::RefPtr<Gio::SimpleActionGroup> &pActionGroup) {

	pActionGroup->add_action("scene.add.scene3d", [this] {
		auto projectSelf = getProject();
		auto sceneInfo = SceneInfo::create(projectSelf);
		addChildEntry(sceneInfo);
		setDatabaseSaveRequired(true);
		if (auto msg = projectSelf->saveFiles()) {
			projectSelf->errorOccurred(msg);
			return;
		}
		projectSelf->requestRebuildEditorLib();
	});
}

engine::utils::ReportMessagePtr ScenesInfo::onLoadDatabase() {

	const auto database = getProject()->getDatabase();

	if (!database->tableExists("Scenes")) return nullptr;


	try {
		//language=sql
		SQLite::Statement statement(*database, "SELECT * FROM Scenes");
		while (statement.executeStep()) {
			std::string sceneName = statement.getColumn(1);
			engine::utils::ReportMessagePtr msg;
			auto uuid = UUID::parse(statement.getColumn(2), msg);
			if (!uuid) return msg;
			auto sceneInfo = SceneInfo::create(getProject(), sceneName, uuid);
			addChildEntry(sceneInfo);
		}
	} catch (...) {
		auto msg = n::engine::utils::ReportMessage::create();
		msg->setTitle("Failed to parse Scenes info");
		msg->setMessage("Exception occurred while querying the Scenes info");
		return msg;
	}
	return nullptr;
}

engine::utils::ReportMessagePtr ScenesInfo::onSaveDatabase() const {
	auto database = getProject()->getDatabase();

	if (auto msg = createTable()) return msg;
	auto &entryChildren = getEntryChildren();
	if (!entryChildren) return nullptr;

	std::unique_ptr<SQLite::Transaction> transaction;
	try {
		transaction = std::make_unique<SQLite::Transaction>(*database);
	} catch (...) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to save Scenes into database");
		msg->setMessage("Exception occurred while starting transaction");
		return msg;
	}
	try {
		//language=sql
		SQLite::Statement statement(*database, R"(
INSERT INTO Scenes (SceneName, SceneUUID) VALUES (?, ?)
)");
		for (uint32_t i = 0; i < entryChildren->get_n_items(); i++) {
			auto child = std::dynamic_pointer_cast<SceneInfo>(entryChildren->get_item(i));
			statement.bind(1, child->getName());
			statement.bind(2, child->getUuid()->toString());
			statement.executeStep();
			statement.clearBindings();
			statement.reset();
		}
	} catch (...) {
		transaction->rollback();
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to create Scenes table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	transaction->commit();
	return nullptr;
}

engine::utils::ReportMessagePtr ScenesInfo::createTable() const {
	try {
		//language=sql
		getProject()->getDatabase()->exec(R"(DROP TABLE IF EXISTS Scenes;
CREATE TABLE Scenes
(
    SceneId   INTEGER
        CONSTRAINT Scenes_pk
            PRIMARY KEY AUTOINCREMENT,
    SceneName TEXT    NOT NULL,
    SceneUUID TEXT    NOT NULL
);
)");
	} catch (...) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to create Scenes table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	return nullptr;
}
}