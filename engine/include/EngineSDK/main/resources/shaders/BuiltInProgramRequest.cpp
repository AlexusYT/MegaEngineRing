//
// Created by alexus on 04.02.24.
//

#include "BuiltInProgramRequest.h"

#include <EngineSDK/main/resources/Resources.h>
#include <EngineSDK/renderer/shaders/Shader.h>
#include <EngineSDK/renderer/shaders/ShaderProgram.h>
#include <EngineUtils/utils/ReportMessage.h>

#include "BuiltInShaderRequest.h"

namespace n::sdk::main {

std::shared_ptr<BuiltInProgramRequest> BuiltInProgramRequest::defaultProgram;

std::shared_ptr<IResource> BuiltInProgramRequest::BuiltInProgramLoader::load(
	const std::shared_ptr<ResourceRequest> &pRequest, const engine::utils::ReportMessagePtr &pError,
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
} // namespace n::sdk::main