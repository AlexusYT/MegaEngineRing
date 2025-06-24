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

#ifndef DEFAULTRENDERER_H
#define DEFAULTRENDERER_H
#include <KwasarEngine/buffers/ProgramWideShaderBuffer.h>
#include <KwasarEngine/resources/shaders/builtin/DefaultProgram.h>

#include "Renderer.h"

namespace ke {
class DefaultRenderer : public Renderer {
protected:
	DefaultRenderer();

public:
	static std::shared_ptr<DefaultRenderer> create() { return std::shared_ptr<DefaultRenderer>(new DefaultRenderer()); }

	void render() override {
		DefaultProgram::getInstance()->use();
		if (programBuffer->tryUpdate()) programBuffer->bindBufferBase(0);
		executeMainPass();
	}

protected:
	ReportMessagePtr onInitialize() override {
		DefaultProgram::getInstance()->initialize();
		return Renderer::onInitialize();
	}

	void onUninitialize() override {
		DefaultProgram::getInstance()->uninitialize();
		Renderer::onUninitialize();
	}
};
} // ke

#endif //DEFAULTRENDERER_H
