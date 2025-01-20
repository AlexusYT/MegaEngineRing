//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 04.02.24.
//

#include "EngineSDK/resources/shaders/ShaderProgramLoader.h"

#include "EngineSDK/resources/shaders/Shader.h"
#include "EngineSDK/resources/shaders/ShaderProgram.h"
#include "EngineUtils/utils/ReportMessage.h"

namespace mer::sdk::main {

std::shared_ptr<IResource> ShaderProgramLoader::createResource() { return std::make_shared<ShaderProgram>(); }

utils::ReportMessagePtr ShaderProgramLoader::load(IResourceLoadExecutor* /*pLoadExecutor*/,
												  std::shared_ptr<std::istream> &pStream,
												  const std::shared_ptr<IResource> &pResource) {
	auto program = std::dynamic_pointer_cast<ShaderProgram>(pResource);
	uint8_t shadersCount;
	pStream->read(reinterpret_cast<char*>(&shadersCount), sizeof(uint8_t));
	for (uint8_t i = 0; i < shadersCount; i++) {
		std::shared_ptr<Shader> shader;
		uint8_t shaderType;
		pStream->read(reinterpret_cast<char*>(&shaderType), sizeof(uint8_t));
		if (shaderType == 0) { //vertex shader
			shader = std::make_shared<VertexShader>();
		} else if (shaderType == 1) { //fragment shader
			shader = std::make_shared<FragmentShader>();
		} else
			continue;
		shader->setSource(readString(pStream));
		program->attachShader(shader);
	}

	return nullptr;
}
} // namespace mer::sdk::main