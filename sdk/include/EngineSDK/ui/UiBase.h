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
#include "EngineSDK/render/IRenderable.h"
#include "imgui.h"

namespace mer::sdk {

class UiBase : public IRenderable {
protected:
	std::string name;
	std::string title;
	glm::vec2 position{};
	glm::vec2 pivot{};
	glm::vec2 size{};
	ImGuiWindowFlags windowFlags{};
	bool dockSpace{};
	unsigned int id{};

public:
	UiBase(const std::string &pName, const std::string &pTitle) : name(pName), title(pTitle) {}

	virtual void updateUi() = 0;

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) {
		name = pName;
		id = 0;
	}

	[[nodiscard]] const std::string &getTitle() const { return title; }

	void setTitle(const std::string &pTitle) { title = pTitle; }

	[[nodiscard]] const glm::vec2 &getPosition() const { return position; }

	void setPosition(const glm::vec2 &pPosition) { position = pPosition; }

	[[nodiscard]] const glm::vec2 &getPivot() const { return pivot; }

	void setPivot(const glm::vec2 &pPivot) { pivot = pPivot; }

	[[nodiscard]] const glm::vec2 &getSize() const { return size; }

	void setSize(const glm::vec2 &pSize) { size = pSize; }

	[[nodiscard]] bool isDockSpace() const { return dockSpace; }

	void setDockSpace(bool pDockSpace = true) { dockSpace = pDockSpace; }

	[[nodiscard]] ImGuiWindowFlags getWindowFlags() const { return windowFlags; }

	void setWindowFlags(ImGuiWindowFlags pWindowFlags) { windowFlags = pWindowFlags; }

	[[nodiscard]] unsigned int getId() const { return id; }
};

} // namespace mer::sdk

#endif //UIBASE_H
