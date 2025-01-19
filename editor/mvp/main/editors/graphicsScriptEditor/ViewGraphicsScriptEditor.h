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
// Created by alexus on 09.07.24.
//

#ifndef VIEWGRAPHICSSCRIPTEDITOR_H
#define VIEWGRAPHICSSCRIPTEDITOR_H
#include "IViewGraphicsScriptEditor.h"
#include "canvas/CanvasWidget.h"
#include "mvp/ThreadDispatcher.h"

namespace mer::editor::mvp {

class ViewGraphicsScriptEditor : public IViewGraphicsScriptEditor, public ThreadDispatcher {
	CanvasWidget canvas;
	std::shared_ptr<Gtk::EventControllerKey> keyEventController;
	std::shared_ptr<Gtk::EventControllerMotion> mouseMotion;
	std::shared_ptr<Gtk::GestureClick> gestureClick;

public:
	ViewGraphicsScriptEditor();

	Gtk::Widget &getMainWidget() override { return canvas; }

	[[nodiscard]] const glm::dvec2 &getCanvasOffset() const override;

	size_t addElement(CanvasElement* pElement) override;

	void setSelectedElement(CanvasElement* pElement) override;

	std::optional<size_t> getSelectedElementIndex() override;

	CanvasElement* getSelectedElement() override;

	sdk::utils::ReportMessagePtr removeElement(CanvasElement* pElement) override;

	sigc::connection connectKeyPressedEvent(
		const sigc::slot<bool(uint32_t pKeyVal, uint32_t pKeyCode, Gdk::ModifierType pState)> &pSlot,
		bool pAfter) override;

	sigc::connection connectKeyReleasedEvent(
		const sigc::slot<void(uint32_t pKeyVal, uint32_t pKeyCode, Gdk::ModifierType pState)> &pSlot,
		bool pAfter) override;

	sigc::connection connectMouseMovedEvent(const sigc::slot<void(double pX, double pY)> &pSlot,
											const bool pAfter) override {
		return mouseMotion->signal_motion().connect(pSlot, pAfter);
	}

	sigc::connection connectMouseButtonPressedEvent(
		const sigc::slot<void(int pPressCount, double pX, double pY)> &pSlot, const bool pAfter) override {
		return gestureClick->signal_pressed().connect(pSlot, pAfter);
	}

	sigc::connection connectMouseButtonReleasedEvent(
		const sigc::slot<void(int pPressCount, double pX, double pY)> &pSlot, const bool pAfter) override {
		return gestureClick->signal_released().connect(pSlot, pAfter);
	}

	uint32_t getCurrentDragButton() const override;

	sigc::connection connectDragBegin(const sigc::slot<void(double pX, double pY)> &pSlot, const bool pAfter) override;

	sigc::connection connectDragUpdate(const sigc::slot<void(double pX, double pY)> &pSlot, const bool pAfter) override;

	sigc::connection connectDragEnd(const sigc::slot<void(double pX, double pY)> &pSlot, const bool pAfter) override;

	void redrawCanvas() override;

	[[nodiscard]] double getCanvasZoom() const override;

	void openView() override;

	void closeView() override;
};

} // namespace mer::editor::mvp

// mvp

#endif //VIEWGRAPHICSSCRIPTEDITOR_H
