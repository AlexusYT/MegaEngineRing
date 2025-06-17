//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 05.06.2025.
//

#ifndef I18N_H
#define I18N_H
#include <regex>
#include <sigc++/connection.h>
#include <unordered_map>

namespace ked {
class I18n {
	inline static sigc::connection onLanguageChangedConnection;

	inline static std::locale currentLocale;
	inline static std::messages<char>::catalog messagesCatalog = -1;

public:
	inline static std::vector<std::string> trLanguages;
	inline static std::unordered_map<std::string, std::string> trLanguagesMap;
	inline static std::unordered_map<std::string, std::string> trExtensionsMap;
	inline static std::vector<std::string> trSketchfabSortTypes;
	inline static std::unordered_map<std::string, std::string> trSketchfabSortTypesMap;
	inline static std::vector<std::string> trSketchfabLicenses;
	inline static std::unordered_map<std::string, std::pair<std::string /*name*/, std::string /*tooltip*/>>
	trSketchfabLicensesMap;
	inline static std::vector<std::string> trSketchfabMaterialTypes;
	inline static std::unordered_map<std::string, std::string> trSketchfabMaterialTypesMap;
	inline static std::string trAngleDegrees;

	static void init();

	static void deinit() { onLanguageChangedConnection.disconnect(); }

	static void switchTo(const std::string &pLocale);

private:
	static void updateStrings();
};
} // namespace ked

#endif //I18N_H
