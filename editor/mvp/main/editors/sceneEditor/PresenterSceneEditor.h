// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 06.02.24.
//

#ifndef PRESENTERSCENEEDITOR_H
#define PRESENTERSCENEEDITOR_H
#include "EngineSDK/utils/ModifierKeys.h"
#include "mvp/main/editors/IEditorPresenter.h"

namespace mer::editor::mvp {
class IViewSceneEditor;
class IModelSceneEditor;

class PresenterSceneEditor : public IEditorPresenter {
	std::shared_ptr<IViewSceneEditor> viewSceneEditor;
	std::shared_ptr<IModelSceneEditor> modelSceneEditor;
	sigc::signal<void(const std::string &pName)> tabHeaderChanged;

public:
	PresenterSceneEditor(const std::shared_ptr<IViewSceneEditor> &pViewSceneEditor,
						 const std::shared_ptr<IModelSceneEditor> &pModelSceneEditor);

	operator Gtk::Widget&() override;

	sigc::connection connectTabHeaderChanged(const sigc::slot<void(const std::string &pName)> &pSlot) override {

		return tabHeaderChanged.connect(pSlot);
	}

private:
	void notifyLoadingStarted() const;
	void notifyLoadingStopped(const sdk::utils::ReportMessagePtr &pError) const;

	sdk::utils::ReportMessagePtr loadScene() const;

	static sdk::utils::ModifierKeys convertToModifierKeys(const Gdk::ModifierType &pState);
};
} // namespace mer::editor::mvp


#endif //PRESENTERSCENEEDITOR_H
