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

#ifndef MODELOBJECTSTREE_H
#define MODELOBJECTSTREE_H

namespace mer::sdk {
class Node;
}

namespace mer::editor::mvp {
class IPresenterObjectsTree;
class NodeSelectionHelper;
} // namespace mer::editor::mvp

namespace mer::sdk {
class Scene3D;
}

namespace mer::editor::mvp {
class IModelObjectsTree {
public:
	virtual ~IModelObjectsTree() = default;

	[[nodiscard]] virtual IPresenterObjectsTree* getPresenter() const = 0;

	virtual void setPresenter(IPresenterObjectsTree* pPresenter) = 0;

	virtual void setScene(const std::shared_ptr<sdk::Scene3D> &pScene) = 0;

	[[nodiscard]] virtual const std::shared_ptr<sdk::Scene3D> &getScene() const = 0;

	virtual void clearSelection() = 0;

	virtual void select(sdk::Node* pNode) = 0;
};

class ModelObjectsTree : public IModelObjectsTree {
	std::shared_ptr<sdk::Scene3D> scene;
	NodeSelectionHelper* selection;
	IPresenterObjectsTree* presenter{};

public:
	explicit ModelObjectsTree(NodeSelectionHelper* pSelection);

	[[nodiscard]] IPresenterObjectsTree* getPresenter() const override { return presenter; }

	void setPresenter(IPresenterObjectsTree* pPresenter) override { presenter = pPresenter; }

	void setScene(const std::shared_ptr<sdk::Scene3D> &pNewScene3D) override;

	[[nodiscard]] const std::shared_ptr<sdk::Scene3D> &getScene() const override { return scene; }

	void clearSelection() override;

	void select(sdk::Node* pNode) override;
};


} // namespace mer::editor::mvp

#endif //MODELOBJECTSTREE_H
