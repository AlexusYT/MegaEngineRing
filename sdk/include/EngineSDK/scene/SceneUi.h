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

#ifndef SCENEUI_H
#define SCENEUI_H
#include <unordered_map>

#include "EngineSDK/render/Initializable.h"
#include "EngineSDK/ui/UiPopup.h"


struct ImGuiIO;
struct ImGuiContext;

namespace mer::sdk {
class UiWindow;
class Window;
} // namespace mer::sdk

namespace mer::sdk {

class SceneUi : public Initializable {
	Window* window{};
	ImGuiContext* imGuiContext{};
	std::unordered_map<std::string, UiWindow*> windows;
	std::unordered_map<std::string, UiPopup*> popups;


public:
	virtual void beforeUiFrame();

	virtual void updateUi();

	virtual void customRender();

	[[nodiscard]] const Window* getWindow() const { return window; }

	void setWindow(Window* pWindow) { window = pWindow; }

	static ImGuiIO &getIo();

	void addUiWindow(UiWindow* pWindow);

	void removeUiWindow(const std::string &pName);

	void addUiPopup(UiPopup* pWindow);

	void removeUiPopup(const std::string &pName);

	virtual void onSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyChanged(int pKey, int pScancode, int pAction, int pMods);

	virtual void onMouseScroll(double pXOffset, double pYOffset);

	virtual void onMouseButton(int pButton, int pAction, int pMods);

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;
};

} // namespace mer::sdk

#endif //SCENEUI_H
