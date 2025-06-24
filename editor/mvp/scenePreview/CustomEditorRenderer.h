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
// Created by alexus on 18.06.2025.
//

#ifndef CUSTOMEDITORRENDERER_H
#define CUSTOMEDITORRENDERER_H
#include <KwasarEngine/render/Renderer.h>

namespace ke {
class ProgramWideShaderBuffer;
}

namespace ked {
class SceneOverlayElements;

class CustomEditorRenderer : public ke::Renderer {
	CustomEditorRenderer();

	std::shared_ptr<SceneOverlayElements> overlay;

protected:
	ke::ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

public:
	static std::shared_ptr<CustomEditorRenderer> create() {
		return std::shared_ptr<CustomEditorRenderer>(new CustomEditorRenderer());
	}

	void render() override;
};
} // ked

#endif //CUSTOMEDITORRENDERER_H
