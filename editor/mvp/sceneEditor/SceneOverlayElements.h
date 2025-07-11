//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 06.01.25.
//

#ifndef SCENEOVERLAYELEMENTS_H
#define SCENEOVERLAYELEMENTS_H
#include "KwasarEngine/render/IRenderable.h"
#include "KwasarEngine/render/Initializable.h"

namespace ke {
class ShaderProgram;
}

namespace ked {
class SceneOverlayElements : public ke::Initializable, public ke::IRenderable {
	uint32_t dataBuffer{};
	uint32_t indexBuffer{};
	uint32_t vao{};
	std::shared_ptr<ke::ShaderProgram> shader;
	std::vector<uint16_t> indices;
	bool gridVisible{true};

protected:
	SceneOverlayElements();

public:
	static std::shared_ptr<SceneOverlayElements> create();

	void render() override;

	[[nodiscard]] bool isGridVisible() const { return gridVisible; }

	void setGridVisible(bool pGridVisible) { gridVisible = pGridVisible; }

protected:
	void renderGrid() const;

	ke::ReportMessagePtr onInitialize() override;

	void onUninitialize() override;
};
} // namespace ked

#endif //SCENEOVERLAYELEMENTS_H