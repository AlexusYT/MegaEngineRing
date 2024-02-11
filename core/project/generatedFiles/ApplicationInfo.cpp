//
// Created by alexus on 20.01.24.
//

#include "ApplicationInfo.h"

#include <SQLiteCpp/Database.h>
#include <mvp/main/editors/appInfo/AppInfoEditor.h>
#include <mvp/main/editors/appInfo/ModelAppInfo.h>
#include <mvp/main/editors/appInfo/PresenterAppInfo.h>
#include <project/generators/cpp/CppHeaderFile.h>

#include "project/Project.h"
#include "project/generators/cpp/AccessModifier.h"
#include "project/generators/cpp/CppClass.h"
#include "project/generators/cpp/CppCustomStatement.h"
#include "project/generators/cpp/CppGenerator.h"
#include "project/generators/cpp/CppMethod.h"

namespace PROJECT_CORE {
ApplicationInfo::ApplicationInfo(const std::shared_ptr<Project> &pProject) : GeneratedFileEntry(pProject) {
	setName("Настройки приложения");
	propertiesList = Gio::ListStore<AppInfoProperty>::create();
	addProperty(AppInfoProperty::create("applicationName", pProject->getProjectName()));
	addProperty(AppInfoProperty::create("applicationDisplayName", pProject->getProjectName()));
	addProperty(AppInfoProperty::create("logsDirectory", ""));
	setDatabaseSaveRequired();
}

engine::utils::ReportMessagePtr ApplicationInfo::onLoadDatabase() {
	const auto database = getProject()->getDatabase();

	if (!database->tableExists("ApplicationInfo")) return nullptr;


	try {
		//language=sql
		SQLite::Statement statement(*database, "SELECT * FROM ApplicationInfo");
		while (statement.executeStep()) {
			std::string name = statement.getColumn(1);
			const auto &column = statement.getColumn(2);
			if (auto iter = properties.find(name); iter != properties.end()) {
				iter->second->setValue(column.getString());
			} else {
				addProperty(AppInfoProperty::create(name, column.getString()));
			}
		}
	} catch (...) {
		auto msg = n::engine::utils::ReportMessage::create();
		msg->setTitle("Failed to parse application info");
		msg->setMessage("Exception occurred while querying the application info");
		return msg;
	}

	return nullptr;
}

engine::utils::ReportMessagePtr ApplicationInfo::onSaveDatabase() const {
	const auto database = getProject()->getDatabase();
	if (auto msg = createTable(database.get())) return msg;

	std::unique_ptr<SQLite::Transaction> transaction;
	try {
		transaction = std::make_unique<SQLite::Transaction>(*database);
	} catch (...) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to save application info into database");
		msg->setMessage("Exception occurred while starting transaction");
		return msg;
	}
	try {
		//language=sql
		SQLite::Statement statement(*database, R"(
INSERT INTO ApplicationInfo (PropertyName, PropertyValue) VALUES
(?, ?)
)");
		for (const auto &[name, value]: properties) {
			statement.bind(1, name);
			statement.bind(2, value->getValue());
			statement.executeStep();
			statement.clearBindings();
			statement.reset();
		}
	} catch (...) {
		transaction->rollback();
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to create application info table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	transaction->commit();

	return nullptr;
}

engine::utils::ReportMessagePtr ApplicationInfo::onSaveFile() const {
	if (auto msg = writeFile()) { return msg; }
	return nullptr;
}

engine::utils::ReportMessagePtr ApplicationInfo::createTable(SQLite::Database* pDatabase) {
	try {
		//language=sql
		pDatabase->exec(R"(DROP TABLE IF EXISTS ApplicationInfo;
CREATE TABLE ApplicationInfo
(
    PropertyId    INTEGER
        CONSTRAINT ApplicationInfo_pk
            PRIMARY KEY AUTOINCREMENT,
    PropertyName  TEXT    NOT NULL,
    PropertyValue ANY
);
)");
	} catch (...) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to create application info table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	return nullptr;
}

engine::utils::ReportMessagePtr ApplicationInfo::writeFile() const {
	using namespace std::string_literals;

	auto method = CppMethod::create();
	method->setName("init");
	method->setReturnType("n::engine::utils::ReportMessagePtr");
	method->setIsOverride(true);
	for (const auto &[name, value]: properties) {
		if (name.empty() || value->getValue().empty()) continue;
		std::string methodName = "set"s + static_cast<char>(std::toupper(name[0])) + name.substr(1);
		method->addStatement(CppMethodCall::create(methodName, {std::format("\"{}\"", value->getValue())}));
	}
	method->addStatement(CppCustomStatement::create("return DefaultApplicationSettings::init()"));
	const auto class_ = CppClass::create();
	method->setKlass(class_.get());
	class_->setName("ApplicationSettings");
	class_->addImplement("public n::sdk::main::DefaultApplicationSettings");
	class_->addDeclaration(method->getDeclaration(), AccessModifier::PRIVATE);
	CppHeaderFile headerFile;
	headerFile.addInclude("EngineSDK/main/DefaultApplicationSettings.h");
	headerFile.addDefinition(class_->getDefinition());
	if (auto msg = headerFile.writeFile(getProject()->getProjectPath() / getHeaderPath())) return msg;
	CppSourceFile sourceFile;
	sourceFile.addInclude(getHeaderPath().filename(), false);
	sourceFile.addDefinition(method->getDefinition());
	return sourceFile.writeFile(getProject()->getProjectPath() / getSourcePath());
}

std::shared_ptr<mvp::IEditorPresenter> ApplicationInfo::openEditor() {
	auto modelAppInfo = std::make_shared<mvp::ModelAppInfo>();
	modelAppInfo->setInfo(this);
	auto viewAppInfo = std::make_shared<mvp::AppInfoEditor>();


	return std::make_shared<mvp::PresenterAppInfo>(viewAppInfo, modelAppInfo);
}

void ApplicationInfo::addProperty(std::shared_ptr<AppInfoProperty> pProperty) {
	propertiesList->append(pProperty);
	properties.emplace(pProperty->getName(), pProperty);
}
} // namespace PROJECT_CORE