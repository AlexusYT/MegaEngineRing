//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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

#include "ViewGraphicsScriptEditor.h"

namespace mer::editor::mvp {

ViewGraphicsScriptEditor::ViewGraphicsScriptEditor() {
	keyEventController = Gtk::EventControllerKey::create();
	canvas.add_controller(keyEventController);
	mouseMotion = Gtk::EventControllerMotion::create();
	canvas.add_controller(mouseMotion);
	gestureClick = Gtk::GestureClick::create();
	canvas.add_controller(gestureClick);
}

const glm::dvec2 &ViewGraphicsScriptEditor::getCanvasOffset() const { return canvas.getCanvasOffset(); }

size_t ViewGraphicsScriptEditor::addElement(CanvasElement* pElement) { return canvas.addElement(pElement); }

void ViewGraphicsScriptEditor::setSelectedElement(CanvasElement* pElement) { canvas.setSelectedElement(pElement); }

std::optional<size_t> ViewGraphicsScriptEditor::getSelectedElementIndex() { return canvas.getSelectedElementIndex(); }

CanvasElement* ViewGraphicsScriptEditor::getSelectedElement() { return canvas.getSelectedElement(); }

sdk::utils::ReportMessagePtr ViewGraphicsScriptEditor::removeElement(CanvasElement* pElement) {
	return canvas.removeElement(pElement);
}

sigc::connection ViewGraphicsScriptEditor::connectKeyPressedEvent(
	const sigc::slot<bool(uint32_t pKeyVal, uint32_t pKeyCode, Gdk::ModifierType pState)> &pSlot, const bool pAfter) {
	return keyEventController->signal_key_pressed().connect(pSlot, pAfter);
}

sigc::connection ViewGraphicsScriptEditor::connectKeyReleasedEvent(
	const sigc::slot<void(uint32_t pKeyVal, uint32_t pKeyCode, Gdk::ModifierType pState)> &pSlot, const bool pAfter) {
	return keyEventController->signal_key_released().connect(pSlot, pAfter);
}

uint32_t ViewGraphicsScriptEditor::getCurrentDragButton() const { return canvas.getCurrentDragButton(); }

sigc::connection ViewGraphicsScriptEditor::connectDragBegin(const sigc::slot<void(double pX, double pY)> &pSlot,
															const bool pAfter) {
	return canvas.connectDragBegin(pSlot, pAfter);
}

sigc::connection ViewGraphicsScriptEditor::connectDragUpdate(const sigc::slot<void(double pX, double pY)> &pSlot,
															 const bool pAfter) {
	return canvas.connectDragUpdate(pSlot, pAfter);
}

sigc::connection ViewGraphicsScriptEditor::connectDragEnd(const sigc::slot<void(double pX, double pY)> &pSlot,
														  const bool pAfter) {
	return canvas.connectDragEnd(pSlot, pAfter);
}

void ViewGraphicsScriptEditor::redrawCanvas() { canvas.queue_draw(); }

double ViewGraphicsScriptEditor::getCanvasZoom() const { return canvas.getZoom(); }

void ViewGraphicsScriptEditor::openView() {}

void ViewGraphicsScriptEditor::closeView() {}


} // namespace mer::editor::mvp

// mvp