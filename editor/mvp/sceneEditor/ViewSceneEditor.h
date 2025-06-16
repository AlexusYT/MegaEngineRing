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
// Created by alexus on 06.02.24.
//

#ifndef VIEWSCENEEDITOR_H
#define VIEWSCENEEDITOR_H

#include "KwasarEngine/ui/UiWindow.h"
#include "mvp/editor/Editor.h"

namespace mer::sdk {
class Scene3D;
}

namespace mer::editor::mvp {
class NodeSelectionHelper;
class IPresenterObjectProperties;
class IPresenterObjectsTree;
class IPresenterScenePreview;

class SceneEditor : public Editor {
	std::shared_ptr<IPresenterScenePreview> scenePreviewPresenter;
	std::shared_ptr<EditorTool> scenePreviewView;
	std::shared_ptr<IPresenterObjectsTree> objTreePresenter;
	std::shared_ptr<EditorTool> objTreeView;
	std::shared_ptr<IPresenterObjectProperties> propertiesPresenter;
	std::shared_ptr<EditorTool> propertiesViewTool;
	std::shared_ptr<NodeSelectionHelper> selection;

	std::shared_ptr<sdk::Scene3D> scene3D;

public:
	explicit SceneEditor(const std::string &pName);

	void updateUi() override;

protected:
	const char* getType() const override { return "SceneEditor"; }

	void loadPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize, ImGuiDir pPanelDir) override;

private:
	void addPlane();

	void addCube();

	void addSphere();

	void addGltfModel(const std::string &pPath);
};
} // namespace mer::editor::mvp

#endif //VIEWSCENEEDITOR_H