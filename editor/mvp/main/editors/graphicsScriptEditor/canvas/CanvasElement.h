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

#ifndef CANVASELEMENT_H
#define CANVASELEMENT_H

namespace mer::editor::mvp {
class CanvasWidget;
}

namespace mer::editor::mvp {
class CanvasElement {
	friend class CanvasWidget;
	glm::dvec2 position{};
	glm::dvec2 size{};
	bool hasSize{};
	CanvasWidget* widget{};

public:
	virtual ~CanvasElement() = default;

	[[nodiscard]] const glm::dvec2 &getPosition() const { return position; }

	void setPosition(const glm::dvec2 &pPosition);

	[[nodiscard]] CanvasWidget* getWidget() const { return widget; }

	void setWidget(CanvasWidget* pWidget) {
		widget = pWidget;
		onWidgetChanged(pWidget);
	}

	[[nodiscard]] const glm::dvec2 &getSize() const { return size; }

	[[nodiscard]] bool isHasSize() const { return hasSize; }

	void updateSize(const std::shared_ptr<Cairo::Context> &pContext);

	void draw(const std::shared_ptr<Cairo::Context> &pContext);

	virtual void onRemove() {}

protected:
	virtual void onWidgetChanged(CanvasWidget* pWidget);

	virtual void calculateSize(const std::shared_ptr<Cairo::Context> &pContext, glm::dvec2 &pSizeOut) = 0;

	virtual void onDraw(const std::shared_ptr<Cairo::Context> &pContext) = 0;

	virtual void onPositionChanged(const glm::dvec2 &pNewPos);

	void setSize(const glm::dvec2 &pSize) { size = pSize; }

	[[nodiscard]] bool isSelected() const;

	void setHasSize(const bool pHasSize) { hasSize = pHasSize; }
};

} // namespace mer::editor::mvp

#endif //CANVASELEMENT_H
