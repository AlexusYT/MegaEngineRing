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
// Created by alexus on 20.01.24.
//

#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

#include "GeneratedFileEntry.h"
#include "mvp/main/centerWindow/ITab.h"

namespace mer::editor::mvp {
class AppInfoEditor;
}

namespace mer::editor::project {
class Project;
}

namespace SQLite {
class Database;
}

namespace mer::editor::project {
class AppInfoProperty : public Glib::Object {
	std::string name;
	std::string value;

protected:
	AppInfoProperty(const std::string &pName, const std::string &pValue) : name(pName), value(pValue) {}

public:
	static std::shared_ptr<AppInfoProperty> create(const std::string &pName, const std::string &pValue) {

		return Glib::make_refptr_for_instance(new AppInfoProperty(pName, pValue));
	}

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::string &getValue() const { return value; }

	void setValue(const std::string &pValue) { value = pValue; }
};

class ApplicationInfo : public GeneratedFileEntry, public mvp::ITab {
	friend mvp::AppInfoEditor;


	Glib::RefPtr<Gio::ListStore<AppInfoProperty>> propertiesList;
	std::unordered_map<std::string, std::shared_ptr<AppInfoProperty>> properties;

	/**
	 * @brief The application name. Used in paths.
	 */
	//std::string applicationName;

	/**
	 * @brief The application display name. Used to display the application name to user.
	 */
	//std::string applicationDisplayName;

	/**
	 * @brief The directory where the logs will be stored.
	 */
	//std::string logsDirectory;

protected:
	explicit ApplicationInfo(const std::shared_ptr<Project> &pProject);

public:
	static std::shared_ptr<ApplicationInfo> create(const std::shared_ptr<Project> &pProject) {
		return Glib::make_refptr_for_instance(new ApplicationInfo(pProject));
	}

	sdk::utils::ReportMessagePtr onLoadDatabase() override;

	sdk::utils::ReportMessagePtr onSaveDatabase() const override;

	sdk::utils::ReportMessagePtr onSaveFile() const override;

	static sdk::utils::ReportMessagePtr createTable(SQLite::Database* pDatabase);

	sdk::utils::ReportMessagePtr writeFile() const;

	std::shared_ptr<mvp::IPresenter> createEditorPresenter(
		const std::shared_ptr<mvp::TabPlaceholder> &pPlaceholder) override;

	[[nodiscard]] const Glib::RefPtr<Gio::ListStore<AppInfoProperty>> &getPropertiesList() const {
		return propertiesList;
	}

	std::filesystem::path getHeaderPath() const override { return "source/ApplicationSettings.h"; }

	std::filesystem::path getSourcePath() const override { return "source/ApplicationSettings.cpp"; }

private:
	void addProperty(std::shared_ptr<AppInfoProperty> pProperty);
};
} // namespace mer::editor::project

#endif //APPLICATIONINFO_H
