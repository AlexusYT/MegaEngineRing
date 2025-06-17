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

#ifndef UIWINDOWCONTEXT_H
#define UIWINDOWCONTEXT_H

namespace ke {
class SceneUi;
class UiWindow;
class UiBase;
} // namespace ke

namespace ked {
class Editor;
class EditorTool;

class IWidgetContext {
public:
	virtual ~IWidgetContext() = default;

	virtual void add(ke::UiBase* pWidget) = 0;

	virtual void remove() = 0;
};

class EditorContext : public IWidgetContext {
	EditorTool* tool{};
	Editor* editor{};

	explicit EditorContext(Editor* pEditor) : editor(pEditor) {}

public:
	static std::shared_ptr<IWidgetContext> create(Editor* pEditor) {
		return std::shared_ptr<EditorContext>(new EditorContext(pEditor));
	}

	void add(ke::UiBase* pWidget) override;

	void remove() override;
};

class SceneUiContext : public IWidgetContext {
	ke::UiBase* window{};
	ke::SceneUi* scene{};

	explicit SceneUiContext(ke::SceneUi* pScene) : scene(pScene) {}

public:
	static std::shared_ptr<IWidgetContext> create(ke::SceneUi* pScene) {
		return std::shared_ptr<SceneUiContext>(new SceneUiContext(pScene));
	}

	void add(ke::UiBase* pWidget) override;

	void remove() override;
};
} // namespace ked

#endif //UIWINDOWCONTEXT_H
