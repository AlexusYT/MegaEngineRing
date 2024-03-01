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
// Created by alexus on 04.02.24.
//

#include "BuiltInProgramRequest.h"

#include <EngineSDK/main/resources/Resources.h>
#include <EngineSDK/renderer/shaders/Shader.h>
#include <EngineSDK/renderer/shaders/ShaderProgram.h>
#include <EngineUtils/utils/ReportMessage.h>

#include "BuiltInShaderRequest.h"

namespace mer::sdk::main {

std::shared_ptr<BuiltInProgramRequest> BuiltInProgramRequest::defaultProgram;

std::shared_ptr<IResource> BuiltInProgramRequest::BuiltInProgramLoader::load(
	const std::shared_ptr<ResourceRequest> &pRequest, const sdk::utils::ReportMessagePtr &pError,
	const std::shared_ptr<Resources> &pDependencies) {

	auto program = std::make_shared<renderer::ShaderProgram>();
	for (const auto &resource: pDependencies->getResources()) {
		if (auto shader = std::dynamic_pointer_cast<renderer::Shader>(resource.second)) {
			program->attachShader(shader);
		}
	}
	program->link();
	if (!program->getLinkStatus()) {

		pError->setStacktrace();
		pError->setTitle("Failed to link the builtin shader program");
		pError->setMessage("Error in shader code detected");
		pError->addInfoLine("Shader program name: {}", pRequest->getName());
		std::string log;
		program->getInfoLog(log);
		pError->addInfoLine("Linker log: {}", log);
		return nullptr;
	}
	return program;
}

const std::shared_ptr<BuiltInProgramRequest> &BuiltInProgramRequest::getDefaultProgram() {
	if (!defaultProgram) {
		defaultProgram = std::shared_ptr<BuiltInProgramRequest>{new BuiltInProgramRequest("Default")};
		defaultProgram->setRequired(BuiltInVertexShaderRequest::getDefault(),
									BuiltInFragmentShaderRequest::getDefault());
	}
	return defaultProgram;
}
} // namespace mer::sdk::main