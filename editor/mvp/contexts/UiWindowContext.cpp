//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 10.02.25.
//

#include "UiWindowContext.h"

#include "EngineSDK/scene/SceneUi.h"
#include "EngineSDK/ui/UiWindow.h"
#include "mvp/editor/Editor.h"

namespace mer::editor::mvp {

void EditorContext::addTool(EditorTool* pWidget) {
	tool = pWidget;
	editor->addTool(pWidget->shared_from_this());
}

void EditorContext::removeWidget() {
	if (!tool) return;
	editor->removeTool(tool);
}

void EditorContext::setTitle(const std::string & /*pTitle*/) {}

} // namespace mer::editor::mvp