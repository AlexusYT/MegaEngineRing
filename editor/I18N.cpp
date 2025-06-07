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

#include "I18N.h"

#include "mvp/editor/settings/Settings.h"

namespace mer::editor::mvp {
void I18n::init() {
	onLanguageChangedConnection = Settings::getGeneral()->onLanguageChanged.connect(
		[](const std::string &pNew, const std::string & /*pOld*/) { switchTo(pNew); });
	trLanguages.emplace_back("system");

	for (auto &entry: std::filesystem::directory_iterator(LOCALE_DIR)) {
		if (!entry.is_directory()) continue;
		std::string filename = entry.path().stem().string();
		std::string langName;
		std::string countryName;
		try {
			//language=regexp
			static std::regex regex(R"(^[a-z]{2,3}_[A-Z]{2,3}$)");
			std::regex_match(filename, regex);
			auto tmpLocale = std::locale(filename + ".UTF-8");

			auto &facet = std::use_facet<std::messages<char>>(tmpLocale);
			auto tmpMessagesCatalog = facet.open("mer", tmpLocale, LOCALE_DIR);
			if (tmpMessagesCatalog < 0) continue;

			//translators: Name of your language. It will be used in the lang settings combobox: LangName (CountryName)
			langName = get("LangName", facet, tmpMessagesCatalog);
			//translators: Name of your country. It will be used in the lang settings combobox: LangName (CountryName)
			countryName = get("CountryName", facet, tmpMessagesCatalog);

			facet.close(tmpMessagesCatalog);
		} catch (...) { continue; }
		trLanguages.emplace_back(filename);
		trLanguagesMap.emplace(filename, std::format("{} ({})", langName, countryName));
	}
	switchTo(Settings::getGeneral()->language);
}

void I18n::updateStrings() {
	if (auto iter = trLanguagesMap.find(trLanguages[0]); iter != trLanguagesMap.end()) iter->second = get("LangSystem");
	else
		trLanguagesMap.emplace(trLanguages[0], get("LangSystem"));
	trAngleDegrees = get("AngleDegreeSymbol");
	trExtensionsMap.emplace("LightExtension", get("LightExtension"));
	trExtensionsMap.emplace("MeshExtension", get("MeshExtension"));

	trSketchfabSortTypes = {"likeCount",   "-likeCount",   "viewCount",	  "-viewCount",
							"publishedAt", "-publishedAt", "processedAt", "-processedAt"};
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[0], get("SketchfabLikeCountAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[1], get("SketchfabLikeCountDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[2], get("SketchfabViewCountAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[3], get("SketchfabViewCountDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[4], get("SketchfabPublishedAtAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[5], get("SketchfabPublishedAtDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[6], get("SketchfabProcessedAtAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[7], get("SketchfabProcessedAtDesc"));
}
} // namespace mer::editor::mvp