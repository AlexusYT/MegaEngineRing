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
// Created by alexus on 31.05.2025.
//

#ifndef GENERALCATEGORY_H
#define GENERALCATEGORY_H
#include "EngineSDK/utils/Property.h"
#include "SettingsCategory.h"

namespace mer::editor::mvp {
class GeneralCategory : public SettingsCategory {
public:
	std::string language;
	sigc::signal<void(const decltype(language) &pNew, const decltype(language) &pOld)> onLanguageChanged;
	float fontSize;

	GeneralCategory() { GeneralCategory::loadDefaults(); }

	void save(nlohmann::json &pJson) override;

	void load(const nlohmann::json &pJson) override;

	void loadDefaults() override;

	std::string getName() const override { return trs("GeneralCategory"); }

	std::shared_ptr<SettingsCategory> clone() override { return std::make_shared<GeneralCategory>(*this); }

	bool check(const std::shared_ptr<SettingsCategory> &pCopy) const override {
		const auto cat = std::dynamic_pointer_cast<GeneralCategory>(pCopy);
		if (!cat) return false;
		return this->language == cat->language && this->fontSize == cat->fontSize;
	}

	void onApply(const std::shared_ptr<SettingsCategory> &pCopy) override {
		const auto cat = std::dynamic_pointer_cast<GeneralCategory>(pCopy);
		if (!cat) return;
		if (this->language != cat->language) {
			onLanguageChanged(cat->language, this->language);
			this->language = cat->language;
		}
		this->fontSize = cat->fontSize;
	}
};
} // namespace mer::editor::mvp

#endif //GENERALCATEGORY_H