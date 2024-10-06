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
#include "EngineSDK/renderer/shaders/FragmentShader.h"
#include "EngineSDK/renderer/shaders/VertexShader.h"

namespace mer::sdk::main {

std::shared_ptr<BuiltInProgramRequest> BuiltInProgramRequest::defaultProgram;

utils::ReportMessagePtr BuiltInProgramRequest::BuiltInProgramLoader::load(IResourceLoadExecutor* /*pLoadExecutor*/,
																		  std::shared_ptr<std::istream> &pStream,
																		  std::shared_ptr<IResource> &pResourceOut) {
	std::shared_ptr<renderer::ShaderProgram> program = std::make_shared<renderer::ShaderProgram>();
	program->setResourceUri(readString(pStream));
	uint8_t shadersCount;
	pStream->read(reinterpret_cast<char*>(&shadersCount), sizeof(uint8_t));
	for (uint8_t i = 0; i < shadersCount; i++) {
		std::shared_ptr<renderer::Shader> shader;
		uint8_t shaderType;
		pStream->read(reinterpret_cast<char*>(&shaderType), sizeof(uint8_t));
		if (shaderType == 0) { //vertex shader
			shader = std::make_shared<renderer::VertexShader>();
		} else if (shaderType == 1) { //fragment shader
			shader = std::make_shared<renderer::FragmentShader>();
		} else
			continue;
		shader->setSource(readString(pStream));
		shader->compile();
		if (!shader->getCompileStatus()) {
			auto msg = utils::ReportMessage::create();
			msg->setTitle("Failed to compile the shader");
			msg->setMessage("Error in shader code detected");
			msg->addInfoLine("Shader type: {}", shaderType == 0 ? "vertex" : "fragment");
			std::string log;
			shader->getInfoLog(log);
			msg->addInfoLine("Compilation log: {}", log);
			return msg;
		}
		program->attachShader(shader);
	}
	program->link();
	if (!program->getLinkStatus()) {

		auto msg = utils::ReportMessage::create();
		msg->setStacktrace();
		msg->setTitle("Failed to link the shader program");
		msg->setMessage("Error in shader code detected");
		std::string log;
		program->getInfoLog(log);
		msg->addInfoLine("Linker log: {}", log);
		return msg;
	}
	pResourceOut = program;
	/*
	auto program = std::make_shared<renderer::ShaderProgram>();
	for (const auto &resource: pDependencies->getResources()) {
		if (auto shader = std::dynamic_pointer_cast<renderer::Shader>(resource.second)) {
			program->attachShader(shader);
		}
	}
	program->link();
	if (!program->getLinkStatus()) {

		auto msg = utils::ReportMessage::create();
		msg->setStacktrace();
		msg->setTitle("Failed to link the builtin shader program");
		msg->setMessage("Error in shader code detected");
		msg->addInfoLine("Shader program name: {}", pRequest->getName());
		std::string log;
		program->getInfoLog(log);
		msg->addInfoLine("Linker log: {}", log);
		return msg;
	}
	pResourceOut = program;*/
	return nullptr;
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