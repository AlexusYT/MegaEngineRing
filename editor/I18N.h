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
// Created by alexus on 05.06.2025.
//

#ifndef I18N_H
#define I18N_H
#include <regex>
#include <sigc++/connection.h>
#include <unordered_map>

namespace mer::editor::mvp {

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
	inline static std::string trAngleDegrees;

	static void init();

	static void deinit() {
		onLanguageChangedConnection.disconnect();
		auto &facet = std::use_facet<std::messages<char>>(currentLocale);
		if (messagesCatalog >= 0) facet.close(messagesCatalog);
	}

	static void switchTo(const std::string &pLocale) {
		if (pLocale.empty() || pLocale == "system") currentLocale = std::locale("");
		else
			currentLocale = std::locale(pLocale + ".UTF-8");
		auto &facet = std::use_facet<std::messages<char>>(currentLocale);
		if (messagesCatalog >= 0) facet.close(messagesCatalog);
		messagesCatalog = facet.open("mer", currentLocale, LOCALE_DIR);
		updateStrings();
	}

	static std::string get(const std::string &pStr, const std::messages<char> &pFacet, const int pCatalog) {
		return pFacet.get(pCatalog, 0, 0, pStr);
	}

	static std::string get(const std::string &pStr) {
		return get(pStr, std::use_facet<std::messages<char>>(currentLocale), messagesCatalog);
	}


private:
	static void updateStrings();
};

} // namespace mer::editor::mvp

#endif //I18N_H
