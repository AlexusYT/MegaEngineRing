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
// Created by alexus on 15.02.25.
//

#ifndef MODELPREFABEDITOR_H
#define MODELPREFABEDITOR_H
#include "IModelPrefabEditor.h"

namespace mer::sdk {
class Prefab;
}

namespace mer::editor::mvp {

class ModelPrefabEditor : public IModelPrefabEditor {
	std::shared_ptr<sdk::Prefab> selectedPrefab;

public:
	[[nodiscard]] const std::shared_ptr<sdk::Prefab> &getSelectedPrefab() const override { return selectedPrefab; }

	void setSelectedPrefab(const std::shared_ptr<sdk::Prefab> &pSelectedPrefab) override {
		selectedPrefab = pSelectedPrefab;
	}
};

} // namespace mer::editor::mvp

#endif //MODELTEXTUREEDITOR_H
