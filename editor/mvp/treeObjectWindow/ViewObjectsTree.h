//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 10.01.24.
//

#ifndef TREEOBJECTWINDOW_H
#define TREEOBJECTWINDOW_H

#include "mvp/IView.h"
#include "mvp/editor/Editor.h"

namespace ke {
class Scene3D;
class Node;
} // namespace ke

namespace ked {
class IWidgetContext;
class IPresenterObjectsTree;

class IViewObjectsTree : public IView {
public:
	virtual void setSceneToRender(const std::shared_ptr<ke::Scene3D> &pScene) = 0;

	virtual void markSelected(const std::vector<ke::Node*> &pNodes, bool pSelected) = 0;

	virtual void setPresenter(IPresenterObjectsTree* pPresenter) = 0;
};

class ViewObjectsTree : public IViewObjectsTree, public EditorTool {
	std::shared_ptr<IWidgetContext> context;
	std::shared_ptr<ke::Scene3D> scene;
	std::unordered_map<ke::Node*, bool> selectedMap;
	sigc::scoped_connection sceneNodesChangedConnection;
	IPresenterObjectsTree* presenter;
	ke::Node* selectedNodeForContext{};
	ke::Node* hoveredNode{};
	uint32_t contextMenuImguiId{};
	ke::Node* nodeToReparent{};

public:
	ViewObjectsTree(const std::shared_ptr<IWidgetContext> &pContext);

	[[nodiscard]] std::string getTitle() const override { return trs("TreeViewTool"); }

	void openView() override;

	void closeView() override;

	void onUpdate(bool pVisible) override;

private:
	void updateTreeLevel(const std::vector<ke::Node*> &pNodes);

	void setSceneToRender(const std::shared_ptr<ke::Scene3D> &pScene) override;

	void markSelected(const std::vector<ke::Node*> &pNodes, bool pSelected) override;

	void setPresenter(IPresenterObjectsTree* pPresenter) override { presenter = pPresenter; }
};
} // namespace ked

#endif //TREEOBJECTWINDOW_H
