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
// Created by alexus on 27.02.25.
//

#ifndef PRESENTERPREFABPREVIEW_H
#define PRESENTERPREFABPREVIEW_H
#include "mvp/scenePreview/PresenterScenePreview.h"

namespace mer::editor::mvp {
class PresenterPrefabPreview : public PresenterScenePreview {
protected:
	PresenterPrefabPreview(const std::shared_ptr<IViewScenePreview> &pView,
						   const std::shared_ptr<IModelScenePreview> &pModel) : PresenterScenePreview(pView, pModel) {}

public:
	static std::shared_ptr<PresenterPrefabPreview> create(const std::shared_ptr<IViewScenePreview> &pView,
														  const std::shared_ptr<IModelScenePreview> &pModel);

	std::string getTypeName() override { return "PrefabPreview"; }
};
} // namespace mer::editor::mvp

#endif //PRESENTERPREFABPREVIEW_H
