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

#ifndef SETTINGSCATEGORY_H
#define SETTINGSCATEGORY_H
#include <nlohmann/json_fwd.hpp>

namespace mer::editor::mvp {

class SettingsCategory {
	sigc::signal<void()> onSettingsChanged;

public:
	virtual ~SettingsCategory() = default;

	virtual void save(nlohmann::json &pJson) = 0;

	virtual void load(const nlohmann::json &pJson) = 0;

	virtual void loadDefaults() = 0;

	virtual std::string getName() const = 0;

	virtual std::shared_ptr<SettingsCategory> clone() = 0;

	virtual bool check(const std::shared_ptr<SettingsCategory> &pCopy) const = 0;

	virtual void onApply(const std::shared_ptr<SettingsCategory> &pCopy) = 0;

	void apply(const std::shared_ptr<SettingsCategory> &pCopy) {
		onApply(pCopy);
		onSettingsChanged.emit();
	}

	sigc::connection connectChanged(const sigc::slot<void()> &pCallback) {
		return onSettingsChanged.connect(pCallback);
	}
};

} // namespace mer::editor::mvp

#endif //SETTINGSCATEGORY_H
