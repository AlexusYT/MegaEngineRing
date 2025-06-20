//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 31.01.24.
//

#ifndef WINDOW_H
#define WINDOW_H
#include <KwasarEngine/utils/ReportMessageFwd.h>

#include "IContext.h"


struct ImGuiContext;
typedef struct GLFWwindow GLFWwindow;

namespace ke {
class SceneUi;

class Window : IContext {
	GLFWwindow* native{};
	std::shared_ptr<Window> sharedWindow;
	int width = 800;
	int height = 600;
	std::string title = "Untitled121";
	std::vector<std::shared_ptr<SceneUi>> sceneUis;
	ImGuiContext* imGuiContext{};

protected:
	Window();

public:
	~Window() override;

	static auto create() { return std::shared_ptr<Window>(new(std::nothrow) Window()); }

	void show();

	void setVisible(bool pState = true) const;

	bool isVisible() const;

	void setCloseRequest(bool pState) const;

	ke::ReportMessagePtr setContextVersion(int pMajor, int pMinor) const;

	bool isCloseRequest() const;

	void addScene(const std::shared_ptr<SceneUi> &pScene);

	void removeScene(const std::shared_ptr<SceneUi> &pScene);

	void runMainLoop();

	[[nodiscard]] const std::shared_ptr<Window> &getSharedWindow() const { return sharedWindow; }

	void setSharedWindow(const std::shared_ptr<Window> &pSharedWindow) { sharedWindow = pSharedWindow; }

	IContext* getContext() { return this; }

	[[nodiscard]] int getWidth() const { return width; }

	void setWidth(const int pWidth) { width = pWidth; }

	[[nodiscard]] int getHeight() const { return height; }

	void setHeight(const int pHeight) { height = pHeight; }

	[[nodiscard]] GLFWwindow* getNative() const { return native; }

protected:
	ReportMessagePtr init() override;

private:
	virtual void onSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyChanged(int pKey, int pScancode, int pAction, int pMods);

	virtual void onMouseScroll(double pXOffset, double pYOffset);

	virtual void onMouseButton(int pButton, int pAction, int pMods);

	void makeCurrent() override;

	void swapBuffers() const override;
};
} // namespace ke
#endif //WINDOW_H