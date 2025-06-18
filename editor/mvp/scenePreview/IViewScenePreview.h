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

#ifndef IVIEWSCENEPREVIEW_H
#define IVIEWSCENEPREVIEW_H
#include "mvp/IView.h"

namespace ke {
class OrbitCameraExtension;
}

namespace ked {
class IPresenterScenePreview;

class IViewScenePreview : public IView {
public:
	[[nodiscard]] virtual IPresenterScenePreview* getPresenter() const = 0;

	virtual void setPresenter(IPresenterScenePreview* pPresenter) = 0;

	[[nodiscard]] virtual const std::shared_ptr<ke::OrbitCameraExtension> &getCamera() const = 0;

	[[nodiscard]] virtual const glm::vec2 &getMousePos() const = 0;

	virtual bool isRotate() = 0;

	virtual void focusOnThis() = 0;
};
} // namespace ked

#endif //IVIEWSCENEPREVIEW_H
