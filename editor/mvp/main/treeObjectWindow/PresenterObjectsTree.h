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
// Created by alexus on 25.09.24.
//

#ifndef PRESENTEROBJECTSTREE_H
#define PRESENTEROBJECTSTREE_H
#include <memory>

#include "mvp/PresenterBase.h"

namespace mer::sdk {
class Node;
class Scene3D;
}

namespace mer::editor::mvp {
class IViewObjectsTree;
class IModelObjectsTree;

class IPresenterObjectsTree : public PresenterBase {

public:
	virtual void onSceneChanged(const std::shared_ptr<sdk::Scene3D> &pScene) = 0;

	virtual void onSelectionChanged(const std::vector<sdk::Node*> &pNodes, bool pSelected) = 0;

	virtual void clearSelection() = 0;

	virtual void select(sdk::Node* pNode) = 0;
};

class PresenterObjectsTree : public IPresenterObjectsTree {
	std::shared_ptr<IModelObjectsTree> model;
	std::shared_ptr<IViewObjectsTree> view;

public:
	explicit PresenterObjectsTree(const std::shared_ptr<IViewObjectsTree> &pView,
								  const std::shared_ptr<IModelObjectsTree> &pModel);

	void run() override;

	void stop() override;

	void addView(const std::shared_ptr<IView> &pNewView) override;

	void onSceneChanged(const std::shared_ptr<sdk::Scene3D> &pScene) override;

	void onSelectionChanged(const std::vector<sdk::Node*> &pNodes, bool pSelected) override;

	void clearSelection() override;

	void select(sdk::Node* pNode) override;

	std::string getTypeName() override { return "PresenterObjectsTree"; }
};

} // namespace mer::editor::mvp

#endif //PRESENTEROBJECTSTREE_H
