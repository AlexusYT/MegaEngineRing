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
#include "IWidgetContext.h"

namespace mer::editor::mvp {
class Editor;
}

namespace mer::sdk {
class UiWindow;
class SceneUi;
} // namespace mer::sdk

namespace mer::editor::mvp {

class EditorContext : public IWidgetContext {
	EditorTool* tool{};
	Editor* editor{};

	explicit EditorContext(Editor* pEditor) : editor(pEditor) {}

public:
	static std::shared_ptr<IWidgetContext> create(Editor* pEditor) {
		return std::shared_ptr<EditorContext>(new EditorContext(pEditor));
	}

	void addWidget(Gtk::Widget* /*pWidget*/) override {}

	inline void addTool(EditorTool* pWidget) override;

	void removeWidget() override;

	void setTitle(const std::string &pTitle) override;
};

class UiWindowContext : public IWidgetContext {
	sdk::UiWindow* uiWindow{};
	std::shared_ptr<sdk::SceneUi> sceneUi{};

	explicit UiWindowContext(const std::shared_ptr<sdk::SceneUi> &pSceneUi) : sceneUi(pSceneUi) {}

public:
	static std::shared_ptr<IWidgetContext> create(const std::shared_ptr<sdk::SceneUi> &pSceneUi) {
		return std::shared_ptr<UiWindowContext>(new UiWindowContext(pSceneUi));
	}

	void addWidget(Gtk::Widget* /*pWidget*/) override {}

	inline void addWindow(sdk::UiBase* pWidget) override;

	void removeWidget() override;

	void setTitle(const std::string &pTitle) override;
};

} // namespace mer::editor::mvp

#endif //UIWINDOWCONTEXT_H
