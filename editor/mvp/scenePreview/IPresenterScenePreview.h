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
// Created by alexus on 27.02.25.
//

#ifndef IPRESENTERSCENEPREVIEW_H
#define IPRESENTERSCENEPREVIEW_H
#include "mvp/IPresenter.h"

namespace ked {
class IModelScenePreview;

class IPresenterScenePreview : public IPresenter {
public:
	virtual void renderScene() = 0;

	virtual void renderGeometryBoundingVolumes() = 0;

	virtual void renderSelected(bool pOutline) = 0;

	virtual void init() = 0;

	virtual void onPrimaryMouseKeyPressed() = 0;

	virtual void onSecondaryMouseKeyPressed() = 0;

	virtual void onSceneChanged() = 0;

	virtual void setFocus() = 0;

	[[nodiscard]] virtual const std::shared_ptr<IModelScenePreview> &getModel() const = 0;

	virtual void startMovingGesture() = 0;

	virtual bool cancelCurrentAction() = 0;

	virtual bool applyCurrentAction() = 0;

	virtual bool onCursorPosChanged(double pX, double pY) = 0;
};
} // namespace ked

#endif //IPRESENTERSCENEPREVIEW_H
