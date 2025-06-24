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

#include "CustomEditorRenderer.h"

#include <epoxy/gl.h>
#include <KwasarEngine/buffers/ProgramWideShaderBuffer.h>
#include <KwasarEngine/render/RenderPass.h>
#include <KwasarEngine/resources/shaders/builtin/DefaultProgram.h>
#include <mvp/sceneEditor/SceneOverlayElements.h>

namespace ked {
CustomEditorRenderer::CustomEditorRenderer() {
	addMainPass(std::make_shared<ke::RenderPass>());
	overlay = SceneOverlayElements::create();
}

ke::ReportMessagePtr CustomEditorRenderer::onInitialize() {
	if (const auto msg = overlay->initialize()) { ke::Logger::error(msg); }
	if (auto msg = ke::DefaultProgram::getInstance()->initialize()) { ke::Logger::error(msg); }

	return Renderer::onInitialize();
}

void CustomEditorRenderer::onUninitialize() {
	overlay->uninitialize();
	ke::DefaultProgram::getInstance()->uninitialize();
	Renderer::onUninitialize();
}

void CustomEditorRenderer::render() {
	ke::DefaultProgram::getInstance()->use();
	//camera->propertyDistance = targetZoom;
	programBuffer->bindBufferBase(0);
	programBuffer->setMode(ke::RenderPassMode::REGULAR);
	programBuffer->update();
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	executeRenderPass("__editor_outline__");
	glBindVertexArray(0);
	glStencilMask(0x00);
	overlay->render();
	ke::DefaultProgram::getInstance()->use();
	executeMainPass();
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glDisable(GL_DEPTH_TEST);
	programBuffer->setMode(ke::RenderPassMode::OUTLINE);
	programBuffer->update();
	executeRenderPass("__editor_outline__");
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glDisable(GL_STENCIL_TEST);
	glBindVertexArray(0);
	//if (presenter) presenter->renderGeometryBoundingVolumes();
	glEnable(GL_DEPTH_TEST);
}
} // ked
