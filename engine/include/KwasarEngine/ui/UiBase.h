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
// Created by alexus on 14.02.25.
//

#ifndef UIBASE_H
#define UIBASE_H
#include "KwasarEngine/render/IRenderable.h"
#include "imgui.h"

namespace mer::sdk {
class UiBase : public IRenderable {
protected:
	std::string name;
	glm::vec2 position{};
	glm::vec2 pivot{};
	glm::vec2 size{};
	ImGuiWindowFlags windowFlags{};
	unsigned int windowId{};
	bool open{true};

public:
	UiBase(const std::string &pName) : name(pName) {}

	/**
	 * @brief UI update loop. Only here you can use ImGui functions.
	 * @param pVisible if @a true, the tool window is not clipped or collapsed, and you need to render your UI.
	 */
	virtual void onUpdate(bool pVisible) = 0;

	virtual void customRender();

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) {
		name = pName;
		windowId = 0;
	}

	[[nodiscard]] bool isOpen() const { return open; }

	void setOpen(bool pOpen) { open = pOpen; }

	[[nodiscard]] virtual std::string getTitle() const = 0;

	[[nodiscard]] const glm::vec2 &getPosition() const { return position; }

	void setPosition(const glm::vec2 &pPosition) { position = pPosition; }

	[[nodiscard]] const glm::vec2 &getPivot() const { return pivot; }

	void setPivot(const glm::vec2 &pPivot) { pivot = pPivot; }

	[[nodiscard]] const glm::vec2 &getSize() const { return size; }

	void setSize(const glm::vec2 &pSize) { size = pSize; }

	[[nodiscard]] ImGuiWindowFlags getWindowFlags() const { return windowFlags; }

	void setWindowFlags(ImGuiWindowFlags pWindowFlags) { windowFlags = pWindowFlags; }

	[[nodiscard]] unsigned int getId() const { return windowId; }
};
} // namespace mer::sdk

#endif //UIBASE_H