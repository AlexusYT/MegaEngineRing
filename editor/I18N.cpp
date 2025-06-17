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

#include "I18N.h"

#include <libintl.h>

#include "mvp/editor/settings/Settings.h"

#ifdef __MINGW32__
#include <windows.h>
#endif

#define LOCALE_DOMAIN "mer"

namespace ked {
void I18n::init() {
	onLanguageChangedConnection = Settings::getGeneral()->onLanguageChanged.connect(
		[](const std::string &pNew, const std::string & /*pOld*/) { switchTo(pNew); });
	trLanguages.emplace_back("system");
#ifdef __MINGW32__
	_configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
#endif
	for (auto &entry: std::filesystem::directory_iterator(LOCALE_DIR)) {
		if (!entry.is_directory()) continue;
		std::string filename = entry.path().stem().string();
		std::string langName;
		std::string countryName;
		try {
			//language=regexp
			static std::regex regex(R"(^[a-z]{2,3}_[A-Z]{2,3}$)");
			if (!std::regex_match(filename, regex)) continue;

			switchTo(filename);
			//translators: Name of your language. It will be used in the lang settings combobox: LangName (CountryName)
			langName = gettext("LangName");
			//translators: Name of your language. It will be used in the lang settings combobox: LangName (CountryName)
			countryName = gettext("CountryName");
		}
		catch (...) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Failed to load locale");
			msg->setMessage("Exception occurred");
			msg->addInfoLine("Locale directory: {}", LOCALE_DIR);
			msg->addInfoLine("Locale name: {}", filename);
			ke::Logger::error(msg);
			continue;
		}
		trLanguages.emplace_back(filename);
		trLanguagesMap.emplace(filename, std::format("{} ({})", langName, countryName));
	}
	if (const auto &locale = Settings::getGeneral()->language; locale.empty() || locale == "system")
		switchTo("");
	else
		switchTo(locale);
	updateStrings();
}


void I18n::switchTo(const std::string &pLocale) {
#ifdef __MINGW32__
	LCID localeId;
	if (pLocale.empty()) localeId = LOCALE_USER_DEFAULT;
	else {
		const std::wstring localeName(pLocale.begin(), pLocale.end());
		localeId = LocaleNameToLCID(localeName.c_str(), LOCALE_ALLOW_NEUTRAL_NAMES);
	}
	SetThreadLocale(localeId);
#else
	if (pLocale.empty())
		std::setlocale(LC_MESSAGES, "");
	else
		std::setlocale(LC_MESSAGES, (pLocale + ".UTF-8").c_str());
#endif

	bindtextdomain(LOCALE_DOMAIN, LOCALE_DIR);
	textdomain(LOCALE_DOMAIN);
	bind_textdomain_codeset(LOCALE_DOMAIN, "UTF-8");
}

void I18n::updateStrings() {
	if (auto iter = trLanguagesMap.find(trLanguages[0]); iter != trLanguagesMap.end()) iter->second = tr("LangSystem");
	else
		trLanguagesMap.emplace(trLanguages[0], tr("LangSystem"));
	trAngleDegrees = tr("AngleDegreeSymbol");
	trExtensionsMap.emplace("LightExtension", tr("LightExtension"));
	trExtensionsMap.emplace("MeshExtension", tr("MeshExtension"));

	trSketchfabSortTypes = {
		"",
		"likeCount",
		"-likeCount",
		"viewCount",
		"-viewCount",
		"publishedAt",
		"-publishedAt",
		"processedAt",
		"-processedAt"
	};
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[0], tr("SketchfabRelevancy"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[1], tr("SketchfabLikeCountAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[2], tr("SketchfabLikeCountDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[3], tr("SketchfabViewCountAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[4], tr("SketchfabViewCountDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[5], tr("SketchfabPublishedAtAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[6], tr("SketchfabPublishedAtDesc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[7], tr("SketchfabProcessedAtAsc"));
	trSketchfabSortTypesMap.emplace(trSketchfabSortTypes[8], tr("SketchfabProcessedAtDesc"));

	//by,by-sa, by-nd, by-nc, by-nc-sa, by-nc-nd, cc0, ed, st
	trSketchfabLicenses = {"", "by", "by-sa", "by-nd", "by-nc", "by-nc-sa", "by-nc-nd", "cc0", "ed", "st"};
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[0], std::make_pair(
									   tr("Any License"),
									   //translators: Description for Sketchfab License filter "Any License"
									   tr("AnyLicenseDescription")
								   ));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[1], std::make_pair(
									   tr("Creative Commons: Attribution"),
									   //translators: Description for Sketchfab License filter "Creative Commons: Attribution"
									   tr("Author must be credited. Commercial use is allowed.")));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[2], std::make_pair(
									   tr("Creative Commons: ShareAlike"),
									   //translators: Description for Sketchfab License filter "Creative Commons: ShareAlike"
									   tr(
										   "Author must be credited. Modified versions must have the same license. Commercial use is allowed.")));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[3], std::make_pair(
									   tr("Creative Commons: NoDerivatives"),
									   //translators: Description for Sketchfab License filter "Creative Commons: NoDerivatives"
									   tr(
										   "Author must be credited. Modified versions can not be distributed. Commercial use is allowed.")));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[4], std::make_pair(
									   tr("Creative Commons: NonCommercial"),
									   //translators: Description for Sketchfab License filter "Creative Commons: NonCommercial"
									   tr("Author must be credited. No commercial use.")));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[5], std::make_pair(
									   tr("Creative Commons: NonCommercial-ShareAlike"),
									   //translators: Description for Sketchfab License filter "Creative Commons: NonCommercial-ShareAlike"
									   tr(
										   "Author must be credited. No commercial use. Modified versions must have the same license.")));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[6], std::make_pair(
									   tr("Creative Commons: NonCommercial-NoDerivatives"),
									   //translators: Description for Sketchfab License filter "Creative Commons: NonCommercial-NoDerivatives"
									   tr(
										   "Author must be credited. No commercial use. Modified versions can not be distributed.")));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[7], std::make_pair(
									   tr("Creative Commons: Zero"),
									   //translators: Description for Sketchfab License filter "Creative Commons: Zero"
									   tr("Credit is not mandatory. Commercial use is allowed.")));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[8], std::make_pair(
									   tr("Editorial"),
									   //translators: Description for Sketchfab License filter "Editorial"
									   tr(
										   "Use only in connection with events that are newsworthy or of public interest")));
	trSketchfabLicensesMap.emplace(trSketchfabLicenses[9], std::make_pair(
									   tr("Standard"),
									   //translators: Description for Sketchfab License filter "Standard"
									   tr(
										   "Under basic restrictions, use worldwide, on all types of media, commercially or not, and in "
										   "all types of derivative works")));

	trSketchfabMaterialTypes = {"", "true", "false", "metalness", "specular"};
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[0], tr("SketchfabMaterialAny"));
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[1], tr("SketchfabMaterialSimple"));
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[2], tr("SketchfabMaterialPBROnly"));
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[3], tr("SketchfabMaterialMetalness"));
	trSketchfabMaterialTypesMap.emplace(trSketchfabMaterialTypes[4], tr("SketchfabMaterialSpecular"));
}
} // namespace ked
