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
// Created by alexus on 15.02.25.
//

#ifndef PRESENTERPREFABEDITOR_H
#define PRESENTERPREFABEDITOR_H
#include "IPresenterPrefabEditor.h"

namespace ked {
class IViewPrefabEditor;
}

namespace ked {
class IModelPrefabEditor;
}

namespace ked {
class PresenterPrefabEditor : public IPresenterPrefabEditor {
	std::shared_ptr<IModelPrefabEditor> model;
	std::shared_ptr<IViewPrefabEditor> view;

	PresenterPrefabEditor(const std::shared_ptr<IModelPrefabEditor> &pModel,
						  const std::shared_ptr<IViewPrefabEditor> &pView);

public:
	static std::shared_ptr<PresenterPrefabEditor> create(const std::shared_ptr<IModelPrefabEditor> &pModel,
														 const std::shared_ptr<IViewPrefabEditor> &pView);

	void addMeshesFrom(const std::filesystem::path &pPath) override;

	void setSelectedPrefab(const std::shared_ptr<ke::Prefab> &pPrefab) override;

	void run() override;

	void stop() override;

	std::string getTypeName() override { return "TextureEditor"; }
};
} // namespace ked

#endif //PRESENTERPREFABEEDITOR_H
