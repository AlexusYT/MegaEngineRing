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
// Created by alexus on 12.12.23.
//

#include "Project.h"

#include <ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h>

#include "EngineSDK/extensions/ExtensionRegistry.h"
#include "generatedFiles/ApplicationInfo.h"
#include "scripting/ScriptParser.h"

namespace mer::editor::project {
Project::Project() : projectExplorerEntries(Gio::ListStore<ui::ProjectExplorerEntry>::create()) {
	scriptParser = std::make_shared<ScriptParser>();
}

Project::~Project() {}

sdk::ReportMessagePtr Project::openDatabase() {

	try {
		database = std::make_shared<SQLite::Database>(projectPath / (projectName + ".enproj"),
													  SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to connect to database");
		return msg;
	}
	return nullptr;
}

void Project::initProject() {

	scriptParser->setProject(shared_from_this());

	applicationInfo = ApplicationInfo::create(shared_from_this());
	engineFileEntries = GeneratedFiles::create(shared_from_this());
	//engineFileEntries->addChildEntry(applicationInfo);
	projectExplorerEntries->append(engineFileEntries);
}

sdk::ReportMessagePtr Project::loadProject() {

	if (auto msg = engineFileEntries->loadDatabase()) return msg;
	if (auto msg = saveFiles()) return msg;
	return nullptr;
}

sdk::ReportMessagePtr Project::saveProject() {
	sdk::Logger::info("Saving project...");
	try {
		database->backup((database->getFilename() + ".bak").c_str(), SQLite::Database::Save);
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to save project");
		msg->setMessage("Exception occurred while backup");
		return msg;
	}
	if (auto msg = engineFileEntries->saveDatabase()) return msg;
	sdk::Logger::info("Project saved");

	return nullptr;
}

sdk::ReportMessagePtr Project::saveFiles() const {
	if (auto msg = engineFileEntries->saveToCmake()) return msg;
	if (auto msg = engineFileEntries->saveFile()) return msg;
	return nullptr;
}

Glib::RefPtr<Gio::SimpleActionGroup> Project::getActionGroups() const {
	const Glib::RefPtr<Gio::SimpleActionGroup> refActionGroup = Gio::SimpleActionGroup::create();
	return refActionGroup;
}

} // namespace mer::editor::project