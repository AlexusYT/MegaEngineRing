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

#ifndef IMODELGRAPHICSSCRIPTEDITOR_H
#define IMODELGRAPHICSSCRIPTEDITOR_H

namespace mer::editor::project {
class GraphicsScript;
}

namespace mer::editor::mvp {
class CanvasElement;

class IModelGraphicsScriptEditor {
public:
	virtual ~IModelGraphicsScriptEditor() = default;
	[[nodiscard]] virtual project::GraphicsScript* getScript() const = 0;

	virtual void setScript(project::GraphicsScript* pScript) = 0;

	[[nodiscard]] virtual CanvasElement* getHoveredElement() const = 0;

	virtual void setHoveredElement(CanvasElement* pHoveredElement) = 0;

	[[nodiscard]] virtual CanvasElement* getSelectedElement() const = 0;

	virtual void setSelectedElement(CanvasElement* pSelectedElement) = 0;

	virtual sigc::connection connectSelectedElementChanged(
		const sigc::slot<void(CanvasElement* pNewSelectedElement)> &pSlot) = 0;

	[[nodiscard]] virtual CanvasElement* getMovingElement() const = 0;

	virtual void setMovingElement(CanvasElement* pMovingElement) = 0;
};
} // namespace mer::editor::mvp


#endif //IMODELGRAPHICSSCRIPTEDITOR_H
