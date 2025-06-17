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

#include "KwasarEngine/scene/SceneUi.h"
#include "KwasarEngine/ui/UiWindow.h"
#include "mvp/editor/Editor.h"

namespace ked {
void EditorContext::add(ke::UiBase* pWidget) {
	tool = dynamic_cast<EditorTool*>(pWidget);
	if (!editor || !tool) return;
	editor->addTool(tool->shared_from_this());
}

void EditorContext::remove() {
	if (!editor || !tool) return;
	editor->removeTool(tool);
}

void SceneUiContext::add(ke::UiBase* pWidget) {
	if (!scene) return;
	// ReSharper disable once CppDFAUnreachableCode
	window = pWidget;
	if (auto widget = dynamic_cast<ke::UiWindow*>(pWidget)) scene->addUiWindow(widget);
	if (auto widget = dynamic_cast<ke::UiPopup*>(pWidget)) scene->addUiPopup(widget);
}

void SceneUiContext::remove() {
	if (!scene || !window) return;
	// ReSharper disable once CppDFAUnreachableCode
	if (auto widget = dynamic_cast<ke::UiWindow*>(window)) scene->removeUiWindow(widget->getName());
	if (auto widget = dynamic_cast<ke::UiPopup*>(window)) scene->removeUiWindow(widget->getName());
}
} // namespace ked