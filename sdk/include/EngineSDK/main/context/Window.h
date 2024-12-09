// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 31.01.24.
//

#ifndef WINDOW_H
#define WINDOW_H
#include <EngineUtils/utils/ReportMessageFwd.h>

#include "IContext.h"

typedef struct GLFWwindow GLFWwindow;

namespace mer::sdk::main {

class Window : IContext {
	GLFWwindow* native{};
	std::shared_ptr<Window> sharedWindow;
	int width = 800;
	int height = 600;
	std::string title = "Untitled121";

protected:
	Window();

public:
	~Window() override;

	void show();

	void setVisible(bool pState = true) const;

	bool isVisible() const;

	void setCloseRequest(bool pState) const;

	sdk::utils::ReportMessagePtr setContextVersion(int pMajor, int pMinor) const;

	bool isCloseRequest() const;

	[[nodiscard]] const std::shared_ptr<Window> &getSharedWindow() const { return sharedWindow; }

	void setSharedWindow(const std::shared_ptr<Window> &pSharedWindow) { sharedWindow = pSharedWindow; }

	IContext* getContext() { return this; }

	[[nodiscard]] int getWidth() const { return width; }

	void setWidth(const int pWidth) { width = pWidth; }

	[[nodiscard]] int getHeight() const { return height; }

	void setHeight(const int pHeight) { height = pHeight; }

private:
	virtual void onSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyChanged(int pKey, int pScancode, int pAction, int pMods);

	sdk::utils::ReportMessagePtr init() override;

	void makeCurrent() override;

	void swapBuffers() const override;
};

} // namespace mer::sdk::main
#endif //WINDOW_H
