//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 25.05.25.
//

#include "Settings.h"

#include <nlohmann/json.hpp>

namespace mer::editor::mvp {
void Settings::init() {
	addCategory(std::make_shared<GeneralCategory>());
	addCategory(std::make_shared<OtherCategory>());
}

void Settings::deinit() {
	categories.clear();
	categoriesArray.clear();
}

void Settings::setSettingsPath(const std::filesystem::path &pPath) { path = pPath; }

sdk::ReportMessagePtr Settings::load() {

	std::ifstream file(path);
	if (!file.is_open()) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to load settings.");
		msg->setMessage("Unable to open settings file");
		msg->addInfoLine("Settings file path: {}", path.string());
		return msg;
	}
	nlohmann::json root;
	try {
		file >> root;
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to load settings.");
		msg->setMessage("Exception occurred while parsing json.");
		msg->addInfoLine("Settings file path: {}", path.string());
		return msg;
	}
	for (const auto &category: categoriesArray) {
		try {
			category->load(root);
		} catch (...) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to load settings.");
			msg->setMessage("Exception occurred while parsing settings values.");
			msg->addInfoLine("Settings file path: {}", path.string());
			msg->addInfoLine("Errored category: {}", category->getName());
			return msg;
		}
	}
	return nullptr;
}

void Settings::loadDefaults() {
	for (const auto &category: categoriesArray) category->loadDefaults();
}

sdk::ReportMessagePtr Settings::save() {

	std::ofstream file;
	auto tmpPath = path;
	if (backupSettingsFile) {
		tmpPath.replace_extension(path.extension().string() + ".tmp");
		if (std::error_code ec; !std::filesystem::remove(tmpPath, ec) && ec) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to save settings.");
			msg->setMessage("Unable to remove previous tmp-file");
			msg->addInfoLine("Settings file path: {}", path.string());
			msg->addInfoLine("Temp file path: {}", tmpPath.string());
			msg->addInfoLine("Error: {}", ec.message());
			return msg;
		}
	}

	file.open(tmpPath);
	if (!file.is_open()) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to save settings.");
		msg->setMessage("Unable to open settings file");
		msg->addInfoLine("Settings file path: {}", tmpPath.string());
		return msg;
	}
	nlohmann::json root;
	for (const auto &category: categoriesArray) {
		try {
			category->save(root);
		} catch (...) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to save settings.");
			msg->setMessage("Exception occurred while parsing settings values to json");
			msg->addInfoLine("Settings file path: {}", tmpPath.string());
			msg->addInfoLine("Errored category: {}", category->getName());
			return msg;
		}
	}
	try {
		file << root;
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to save settings.");
		msg->setMessage("Exception occurred while saving json to file.");
		msg->addInfoLine("Settings file path: {}", tmpPath.string());
		return msg;
	}
	if (backupSettingsFile) {
		if (std::error_code ec; !std::filesystem::remove(path, ec) && ec) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to save settings.");
			msg->setMessage("Unable to remove settings file");
			msg->addInfoLine("Settings file path: {}", path.string());
			msg->addInfoLine("Temp file path: {}", tmpPath.string());
			msg->addInfoLine("Error: {}", ec.message());
			msg->addInfoLine("You can rename temp file {} to settings file {} manually", tmpPath.filename().string(),
							 path.filename().string());
			return msg;
		}
		std::error_code ec;
		std::filesystem::rename(tmpPath, path, ec);
		if (ec) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to save settings.");
			msg->setMessage("Unable to rename temp file to settings file");
			msg->addInfoLine("Settings file path: {}", path.string());
			msg->addInfoLine("Temp file path: {}", tmpPath.string());
			msg->addInfoLine("Error: {}", ec.message());
			msg->addInfoLine("You can rename temp file {} to settings file {} manually", tmpPath.filename().string(),
							 path.filename().string());
			return msg;
		}
	}
	return nullptr;
}

void Settings::addCategory(const std::shared_ptr<SettingsCategory> &pCategory) {
	if (!pCategory) return;
	const auto &typeExpr = *pCategory; // Workaround for clang -Wpotentially-evaluated-expression
	categories.emplace(typeid(typeExpr), pCategory);
	categoriesArray.emplace_back(pCategory);
}
} // namespace mer::editor::mvp