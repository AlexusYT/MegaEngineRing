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
// Created by alexus on 17.01.24.
//

#ifndef OBJECTPROPERTIESWINDOW_H
#define OBJECTPROPERTIESWINDOW_H

#include "mvp/IView.h"
#include "mvp/editor/Editor.h"

namespace mer::sdk {
class Scene3D;
class MeshInstance;
class Node;
} // namespace mer::sdk

namespace mer::editor::mvp {
class IPresenterObjectProperties;

class IViewObjectProperties : public IView {
public:
	virtual void setPresenter(IPresenterObjectProperties* pPresenter) = 0;

	virtual void setSelectedNode(sdk::Node* pSelectedNode) = 0;

	virtual void setScene(const std::shared_ptr<sdk::Scene3D> &pScene) = 0;
};

class ViewObjectProperties : public IViewObjectProperties, public EditorTool {
	std::shared_ptr<IWidgetContext> context;
	IPresenterObjectProperties* presenter{};
	sdk::Node* selectedNode{};
	std::shared_ptr<sdk::Scene3D> scene{};

public:
	ViewObjectProperties(const std::shared_ptr<IWidgetContext> &pContext);

	void onUpdate(bool pVisible) override;

	void setPresenter(IPresenterObjectProperties* pPresenter) override { presenter = pPresenter; }

	void setSelectedNode(sdk::Node* pSelectedNode) override { selectedNode = pSelectedNode; }

	void setScene(const std::shared_ptr<sdk::Scene3D> &pScene) override { scene = pScene; }

private:
	void openView() override;

	void closeView() override;

	void drawTransformation();

	void drawMaterial(sdk::MeshInstance* pMeshNode);
};

} // namespace mer::editor::mvp


#endif //OBJECTPROPERTIESWINDOW_H
