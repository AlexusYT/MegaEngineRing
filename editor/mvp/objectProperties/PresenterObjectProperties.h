//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 26.09.24.
//

#ifndef PRESENTEROBJECTPROPERTIES_H
#define PRESENTEROBJECTPROPERTIES_H
#include "KwasarEngine/scene/Scene3D.h"
#include "mvp/IPresenter.h"

namespace ke {
class Node;
}

namespace ked {
class IViewObjectProperties;
class IModelObjectProperties;

class IPresenterObjectProperties : public IPresenter {
public:
	virtual void onEditingNodeChanged(ke::Node* pNode) = 0;

	virtual void onSceneChanged(const std::shared_ptr<ke::Scene3D> &pScene) = 0;
};

class PresenterObjectProperties : public IPresenterObjectProperties {
	std::shared_ptr<IModelObjectProperties> model;
	std::shared_ptr<IViewObjectProperties> view;

public:
	explicit PresenterObjectProperties(const std::shared_ptr<IViewObjectProperties> &pView,
									   const std::shared_ptr<IModelObjectProperties> &pModel);

	void run() override;

	void stop() override;

	void onEditingNodeChanged(ke::Node* pNode) override;

	void onSceneChanged(const std::shared_ptr<ke::Scene3D> &pScene) override;

	void addView(const std::shared_ptr<IView> &) override;

	std::string getTypeName() override { return "PresenterObjectProperties"; }
};
} // namespace ked

#endif //PRESENTEROBJECTPROPERTIES_H