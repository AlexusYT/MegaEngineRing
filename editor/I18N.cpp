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

	trSketchfabSortTypes = {"",
							"likeCount",
							"-likeCount",
							"viewCount",
							"-viewCount",
							"publishedAt",
							"-publishedAt",
							"processedAt",
							"-processedAt"};
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[0], get("SketchfabRelevancy"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[1], get("SketchfabLikeCountAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[2], get("SketchfabLikeCountDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[3], get("SketchfabViewCountAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[4], get("SketchfabViewCountDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[5], get("SketchfabPublishedAtAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[6], get("SketchfabPublishedAtDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[7], get("SketchfabProcessedAtAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[8], get("SketchfabProcessedAtDesc"));

	//by,by-sa, by-nd, by-nc, by-nc-sa, by-nc-nd, cc0, ed, st
	trSketchfabLicenses = {"", "by", "by-sa", "by-nd", "by-nc", "by-nc-sa", "by-nc-nd", "cc0", "ed", "st"};
	trSketchfabLicensesMap = {
		{trSketchfabLicenses[0],
		 {tr("Any License"),
		  //translators: Description for Sketchfab License filter "Any License"
		  tr("AnyLicenseDescription")}},
		{trSketchfabLicenses[1],
		 {tr("Creative Commons: Attribution"),
		  //translators: Description for Sketchfab License filter "Creative Commons: Attribution"
		  tr("Author must be credited. Commercial use is allowed.")}},
		{trSketchfabLicenses[2],
		 {tr("Creative Commons: ShareAlike"),
		  //translators: Description for Sketchfab License filter "Creative Commons: ShareAlike"
		  tr("Author must be credited. Modified versions must have the same license. Commercial use is allowed.")}},
		{trSketchfabLicenses[3],
		 {tr("Creative Commons: NoDerivatives"),
		  //translators: Description for Sketchfab License filter "Creative Commons: NoDerivatives"
		  tr("Author must be credited. Modified versions can not be distributed. Commercial use is allowed.")}},
		{trSketchfabLicenses[4],
		 {tr("Creative Commons: NonCommercial"),
		  //translators: Description for Sketchfab License filter "Creative Commons: NonCommercial"
		  tr("Author must be credited. No commercial use.")}},
		{trSketchfabLicenses[5],
		 {tr("Creative Commons: NonCommercial-ShareAlike"),
		  //translators: Description for Sketchfab License filter "Creative Commons: NonCommercial-ShareAlike"
		  tr("Author must be credited. No commercial use. Modified versions must have the same license.")}},
		{trSketchfabLicenses[6],
		 {tr("Creative Commons: NonCommercial-NoDerivatives"),
		  //translators: Description for Sketchfab License filter "Creative Commons: NonCommercial-NoDerivatives"
		  tr("Author must be credited. No commercial use. Modified versions can not be distributed.")}},
		{trSketchfabLicenses[7],
		 {tr("Creative Commons: Zero"),
		  //translators: Description for Sketchfab License filter "Creative Commons: Zero"
		  tr("Credit is not mandatory. Commercial use is allowed.")}},
		{trSketchfabLicenses[8],
		 {tr("Editorial"),
		  //translators: Description for Sketchfab License filter "Editorial"
		  tr("Use only in connection with events that are newsworthy or of public interest")}},
		{trSketchfabLicenses[9],
		 {tr("Standard"),
		  //translators: Description for Sketchfab License filter "Standard"
		  tr("Under basic restrictions, use worldwide, on all types of media, commercially or not, and in "
			 "all types of derivative works")}}};

	trSketchfabMaterialTypes = {"", "true", "false", "metalness", "specular"};
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[0], get("SketchfabMaterialAny"));
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[1], get("SketchfabMaterialSimple"));
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[2], get("SketchfabMaterialPBROnly"));
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[3], get("SketchfabMaterialMetalness"));
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[4], get("SketchfabMaterialSpecular"));
}
} // namespace mer::editor::mvp