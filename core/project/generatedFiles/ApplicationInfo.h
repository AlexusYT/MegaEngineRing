//
// Created by alexus on 20.01.24.
//

#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

#include "GeneratedFileEntry.h"

namespace n::core::mvp {
class AppInfoEditor;
}

namespace n::core::project {
class Project;
}

namespace SQLite {
class Database;
}

namespace PROJECT_CORE {
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

class ApplicationInfo : public GeneratedFileEntry {
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

	engine::utils::ReportMessageUPtr onLoad() override;

	engine::utils::ReportMessageUPtr onSave() const override;

	static engine::utils::ReportMessageUPtr createTable(SQLite::Database* pDatabase);

	engine::utils::ReportMessageUPtr writeFile() const;

	std::shared_ptr<mvp::IEditorPresenter> openEditor() override;

	[[nodiscard]] const Glib::RefPtr<Gio::ListStore<AppInfoProperty>> &getPropertiesList() const {
		return propertiesList;
	}

	std::filesystem::path getPath() const override { return "source/ApplicationSettings.h"; }

private:
	void addProperty(std::shared_ptr<AppInfoProperty> pProperty);
};
} // namespace PROJECT_CORE

#endif //APPLICATIONINFO_H
