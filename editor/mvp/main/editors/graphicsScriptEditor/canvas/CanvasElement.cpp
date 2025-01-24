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
// Created by alexus on 14.07.24.
//

#include "CanvasElement.h"

#include "CanvasWidget.h"

namespace mer::editor::mvp {
void CanvasElement::setPosition(const glm::dvec2 &pPosition) {
	if (position == pPosition) return;
	position = pPosition;
	onPositionChanged(pPosition);
	if (widget) widget->queue_draw();
}

void CanvasElement::updateSize(const std::shared_ptr<Cairo::Context> &pContext) {
	if (!isHasSize()) {
		glm::dvec2 elementSize;
		calculateSize(pContext, elementSize);
		setSize(elementSize);
		setHasSize(true);
	}
}

void CanvasElement::draw(const std::shared_ptr<Cairo::Context> &pContext) {

	const auto pos = getPosition();
	pContext->translate(pos.x, pos.y);
	onDraw(pContext);
	pContext->translate(-pos.x, -pos.y);
}

void CanvasElement::onWidgetChanged(CanvasWidget* /*pWidget*/) {}

void CanvasElement::onPositionChanged(const glm::dvec2 & /*pNewPos*/) {}

bool CanvasElement::isSelected() const {

	if (!widget) return false;
	return widget->getSelectedElement() == this;
}
} // namespace mer::editor::mvp