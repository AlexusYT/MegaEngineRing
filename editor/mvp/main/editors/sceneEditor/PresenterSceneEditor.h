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

#ifndef PRESENTERSCENEEDITOR_H
#define PRESENTERSCENEEDITOR_H
#include "EngineSDK/scene/objects/ISceneObject.h"
#include "EngineSDK/utils/ModifierKeys.h"
#include "mvp/main/editors/IPresenterSceneEditor.h"

namespace mer::sdk {
class IApplication;
}

namespace mer::editor::mvp {
class ExplorerObject;
class IViewSceneEditor;
class IModelSceneEditor;

class PresenterSceneEditor : public IPresenterSceneEditor {
	std::unordered_map<std::shared_ptr<IViewSceneEditor>, std::vector<sigc::connection>> views;
	std::shared_ptr<IModelSceneEditor> modelSceneEditor;

public:
	explicit PresenterSceneEditor(const std::shared_ptr<IModelSceneEditor> &pModelSceneEditor);

	~PresenterSceneEditor() override;


	void addView(const std::shared_ptr<IView> &) override;

private:
	void onGridToggled(bool pState) override;

	static sdk::ModifierKeys convertToModifierKeys(const Gdk::ModifierType &pState);

	bool onMouseScroll(double pDx, double pDy) override;

private:
	void run() override;

	void stop() override;

	std::string getTypeName() override { return "PresenterSceneEditor"; }
};
} // namespace mer::editor::mvp


#endif //PRESENTERSCENEEDITOR_H
