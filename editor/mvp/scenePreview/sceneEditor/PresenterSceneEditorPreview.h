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
// Created by alexus on 12.03.25.
//

#ifndef PRESENTERSCENEEDITORPREVIEW_H
#define PRESENTERSCENEEDITORPREVIEW_H
#include "mvp/scenePreview/PresenterScenePreview.h"

namespace ked {
class PresenterSceneEditorPreview : public PresenterScenePreview {
	PresenterSceneEditorPreview(const std::shared_ptr<IViewScenePreview> &pView,
								const std::shared_ptr<IModelScenePreview> &pModel)
		: PresenterScenePreview(pView, pModel) {}

public:
	static std::shared_ptr<PresenterSceneEditorPreview> create(const std::shared_ptr<IViewScenePreview> &pView,
															   const std::shared_ptr<IModelScenePreview> &pModel);

	std::string getTypeName() override { return "PresenterSceneEditorPreview"; }
};
} // namespace ked

#endif //PRESENTERSCENEEDITORPREVIEW_H
