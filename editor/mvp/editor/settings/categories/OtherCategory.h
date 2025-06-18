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
// Created by alexus on 25.05.25.
//

#ifndef OTHERCATEGORY_H
#define OTHERCATEGORY_H
#include <nlohmann/json_fwd.hpp>

#include "SettingsCategory.h"

namespace ked {
class OtherCategory : public SettingsCategory {
public:
	bool debugEnabled{};

	OtherCategory();

	void save(nlohmann::json &pJson) override;

	void load(const nlohmann::json &pJson) override;

	void loadDefaults() override;

	std::string getName() const override { return trs("OtherCategory"); }

	std::shared_ptr<SettingsCategory> clone() override { return std::make_shared<OtherCategory>(*this); }

	bool check(const std::shared_ptr<SettingsCategory> &pCopy) const override {
		const auto cat = std::dynamic_pointer_cast<OtherCategory>(pCopy);
		if (!cat) return false;
		return this->debugEnabled == cat->debugEnabled;
	}

	void onApply(const std::shared_ptr<SettingsCategory> &pCopy) override {
		const auto cat = std::dynamic_pointer_cast<OtherCategory>(pCopy);
		if (!cat) return;
		this->debugEnabled = cat->debugEnabled;
	}
};
} // namespace ked

#endif //OTHERCATEGORY_H
