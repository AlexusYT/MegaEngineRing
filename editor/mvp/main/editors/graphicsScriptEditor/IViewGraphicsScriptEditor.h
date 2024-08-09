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

#ifndef IVIEWGRAPHICSSCRIPTEDITOR_H
#define IVIEWGRAPHICSSCRIPTEDITOR_H

#include "mvp/ThreadDispatcher.h"

namespace mer::editor::mvp {
class CanvasElement;

class IViewGraphicsScriptEditor : public ThreadDispatcher {
public:
	virtual ~IViewGraphicsScriptEditor() = default;

	virtual Gtk::Widget &getMainWidget() = 0;

	[[nodiscard]] virtual const glm::dvec2 &getCanvasOffset() const = 0;

	virtual size_t addElement(CanvasElement* pElement) = 0;

	virtual void setSelectedElement(CanvasElement* pElement) = 0;

	virtual std::optional<size_t> getSelectedElementIndex() = 0;

	virtual CanvasElement* getSelectedElement() = 0;

	virtual sdk::utils::ReportMessagePtr removeElement(CanvasElement* pElement) = 0;

	virtual sigc::connection connectKeyPressedEvent(
		const sigc::slot<bool(uint32_t pKeyVal, uint32_t pKeyCode, Gdk::ModifierType pState)> &pSlot, bool pAfter) = 0;

	virtual sigc::connection connectKeyReleasedEvent(
		const sigc::slot<void(uint32_t pKeyVal, uint32_t pKeyCode, Gdk::ModifierType pState)> &pSlot, bool pAfter) = 0;

	virtual sigc::connection connectMouseMovedEvent(const sigc::slot<void(double pX, double pY)> &pSlot,
													bool pAfter) = 0;

	virtual sigc::connection connectMouseButtonPressedEvent(
		const sigc::slot<void(int pPressCount, double pX, double pY)> &pSlot, bool pAfter) = 0;

	virtual sigc::connection connectMouseButtonReleasedEvent(
		const sigc::slot<void(int pPressCount, double pX, double pY)> &pSlot, bool pAfter) = 0;

	virtual uint32_t getCurrentDragButton() const = 0;

	virtual sigc::connection connectDragBegin(const sigc::slot<void(double pX, double pY)> &pSlot, bool pAfter) = 0;

	virtual sigc::connection connectDragUpdate(const sigc::slot<void(double pX, double pY)> &pSlot, bool pAfter) = 0;

	virtual sigc::connection connectDragEnd(const sigc::slot<void(double pX, double pY)> &pSlot, bool pAfter) = 0;

	virtual void redrawCanvas() = 0;

	[[nodiscard]] virtual double getCanvasZoom() const = 0;
};

} // namespace mer::editor::mvp

#endif //IVIEWGRAPHICSSCRIPTEDITOR_H
