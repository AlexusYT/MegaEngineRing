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

#ifndef SETTINGS_H
#define SETTINGS_H
#include <typeindex>

#include "categories/GeneralCategory.h"
#include "categories/OtherCategory.h"

namespace mer::editor::mvp {
class Settings {
	inline static std::filesystem::path path;
	inline static std::unordered_map<std::type_index, std::shared_ptr<SettingsCategory>> categories;
	inline static std::vector<std::shared_ptr<SettingsCategory>> categoriesArray;
	inline static bool backupSettingsFile{true};

public:
	static void init();

	static void deinit();

	static void setSettingsPath(const std::filesystem::path &pPath);

	static sdk::ReportMessagePtr load();

	static void loadDefaults();

	static sdk::ReportMessagePtr save();

	static std::shared_ptr<OtherCategory> getOther() { return getCategory<OtherCategory>(); }

	static std::shared_ptr<GeneralCategory> getGeneral() { return getCategory<GeneralCategory>(); }

	static const std::vector<std::shared_ptr<SettingsCategory>> &getCategories() { return categoriesArray; }

private:
	static void addCategory(const std::shared_ptr<SettingsCategory> &pCategory);

	template <typename T>
	static std::shared_ptr<T> getCategory() {
		std::shared_ptr<T> result{};
		if (const auto it = categories.find(typeid(T)); it != categories.end()) {
			result = std::dynamic_pointer_cast<T>(it->second);
		}
		if (!result) throw std::runtime_error("Programmer error: Category not found");
		return result;
	}
};
} // namespace mer::editor::mvp

#endif //SETTINGS_H
