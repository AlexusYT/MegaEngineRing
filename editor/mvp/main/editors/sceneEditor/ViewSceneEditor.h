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

#include "EngineSDK/ui/UiWindow.h"
#include "IViewSceneEditor.h"
#include "mvp/editor/Editor.h"

namespace mer::editor::mvp {
class IPresenterObjectsTree;
class IPresenterScenePreview;
class SubWindows;
class IPresenterSceneEditor;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {

class ViewSceneEditor : public IViewSceneEditor, public sdk::UiWindow {
	std::shared_ptr<IWidgetContext> context;
	IPresenterSceneEditor* presenter{};
#ifdef USE_OLD_UI
	Gtk::Box mainWidget{Gtk::Orientation::VERTICAL};

	Gtk::GLArea area;
	Gtk::Switch modeSwitch;
	Gtk::ToggleButton gridToggle;
	std::shared_ptr<Gtk::EventControllerMotion> motionController;
	std::shared_ptr<Gtk::GestureClick> gestureClick;
	std::shared_ptr<Gtk::EventControllerKey> keyController;
#endif

public:
	explicit ViewSceneEditor(const std::shared_ptr<IWidgetContext> &pContext);

	void updateUi() override;

	void setPresenter(IPresenterSceneEditor* pPresenter) override { presenter = pPresenter; }

#ifdef USE_OLD_UI
	sigc::connection connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) override;

	sigc::connection connectRealize(const sigc::slot<void()> &pSlot) override;

	sigc::connection connectUnrealize(const sigc::slot<void()> &pSlot) override;

	sigc::connection connectResize(const sigc::slot<void(int pWidth, int pHeight)> &pSlot) override;

	void queueResize() override { area.queue_resize(); }

	sigc::connection connectKeyPressedSignal(
		const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const override;

	sigc::connection connectKeyReleasedSignal(
		const sigc::slot<void(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const override;

	sigc::connection connectCursorMovedSignal(const sigc::slot<void(double pX, double pY)> &pSlot) const override {
		return motionController->signal_motion().connect(pSlot, false);
	}

	sigc::connection connectMouseButtonPressedSignal(
		const sigc::slot<void(unsigned int pButton, double pX, double pY)> &pSlot) const override {
		return gestureClick->signal_pressed().connect(
			[this, pSlot](int /*pNPress*/, const double pX, const double pY) {
				pSlot(gestureClick->get_current_button(), pX, pY);
			},
			false);
	}

	sigc::connection connectMouseButtonReleasedSignal(
		const sigc::slot<void(unsigned int pButton, double pX, double pY)> &pSlot) const override {
		return gestureClick->signal_released().connect(
			[this, pSlot](int /*pNPress*/, const double pX, const double pY) {
				pSlot(gestureClick->get_current_button(), pX, pY);
			},
			false);
	}

	void makeCurrent() override;

	void redraw() override;

	void throwIfError() override;

	void emitResize() override;

	sigc::connection connectSimToggledSignal(const sigc::slot<void()> &pSlot) const override {
		return modeSwitch.property_active().signal_changed().connect(pSlot);
	}

	void setTitle(const std::string &pTitle) override;

	void toggleSimMode(const bool pMode = true) override { modeSwitch.set_active(pMode); }

	bool isSimMode() const override { return modeSwitch.get_active(); }
#endif
	void openView() override;

	void closeView() override;
};

class SceneEditor : public Editor {
	std::shared_ptr<IPresenterScenePreview> scenePreviewPresenter;
	std::shared_ptr<EditorTool> scenePreviewView;
	std::shared_ptr<IPresenterObjectsTree> objTreePresenter;
	std::shared_ptr<EditorTool> objTreeView;

public:
	explicit SceneEditor(const std::string &pName);

	void updateUi() override;

protected:
	const char* getType() const override { return "SceneEditor"; }

	void loadPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize, ImGuiDir pPanelDir) override;

private:
	void addPlane();

	void addGltfModel();
};
} // namespace mer::editor::mvp

#endif //VIEWSCENEEDITOR_H
