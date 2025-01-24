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

#ifndef MODELGRAPHICSSCRIPTEDITOR_H
#define MODELGRAPHICSSCRIPTEDITOR_H

#include <sigc++/signal.h>

#include "IModelGraphicsScriptEditor.h"
#include "canvas/CanvasElement.h"

namespace mer::editor::project {
class GraphicsScript;
}

namespace mer::editor::mvp {


class ModelGraphicsScriptEditor : public IModelGraphicsScriptEditor {

	project::GraphicsScript* script{};

	CanvasElement* hoveredElement{};
	CanvasElement* selectedElement{};
	sigc::signal<void(CanvasElement* pNewSelectedElement)> onSelectedElementChanged;
	CanvasElement* movingElement{};

public:
	[[nodiscard]] project::GraphicsScript* getScript() const override { return script; }

	void setScript(project::GraphicsScript* pScript) override { script = pScript; }

	[[nodiscard]] CanvasElement* getHoveredElement() const override { return hoveredElement; }

	void setHoveredElement(CanvasElement* pHoveredElement) override { hoveredElement = pHoveredElement; }

	[[nodiscard]] CanvasElement* getSelectedElement() const override { return selectedElement; }

	void setSelectedElement(CanvasElement* pSelectedElement) override {
		if (selectedElement == pSelectedElement) return;
		selectedElement = pSelectedElement;
		onSelectedElementChanged(pSelectedElement);
	}

	sigc::connection connectSelectedElementChanged(
		const sigc::slot<void(CanvasElement* pNewSelectedElement)> &pSlot) override {
		return onSelectedElementChanged.connect(pSlot);
	}

	[[nodiscard]] CanvasElement* getMovingElement() const override { return movingElement; }

	void setMovingElement(CanvasElement* pMovingElement) override { movingElement = pMovingElement; }
};
} // namespace mer::editor::mvp


#endif //MODELGRAPHICSSCRIPTEDITOR_H
