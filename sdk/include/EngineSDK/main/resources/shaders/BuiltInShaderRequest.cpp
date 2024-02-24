//
// Created by alexus on 31.01.24.
//

#include "BuiltInShaderRequest.h"

#include <EngineSDK/renderer/shaders/FragmentShader.h>
#include <EngineSDK/renderer/shaders/VertexShader.h>
#include <EngineUtils/utils/ReportMessage.h>

namespace mer::sdk::main {

std::shared_ptr<IResource> BuiltInShaderLoader::load(const std::shared_ptr<ResourceRequest> &pRequest,
													 const sdk::utils::ReportMessagePtr &pError,
													 const std::shared_ptr<Resources> & /*pDependencies*/) {
	std::shared_ptr<renderer::Shader> result;
	if (auto fragRequest = std::dynamic_pointer_cast<BuiltInFragmentShaderRequest>(pRequest)) {
		result = std::make_shared<renderer::FragmentShader>();
		//language=glsl
		if (BuiltInFragmentShaderRequest::getDefault() == fragRequest)
			/*result->setSource(R"(
#version 430 core

uniform sampler2D textureIn;

out vec4 colorOut;
in VS_OUT {
	vec2 uv;
	vec4 color;
} gs_in;


void main(){
	colorOut = vec4(vec3(1), 1);
}
)");*/
			result->setSource(R"(#version 330

out vec4 outputColor;

void main() {
  float lerpVal = gl_FragCoord.y / 500.0f;

  outputColor = mix(vec4(1.0f, 0.85f, 0.35f, 1.0f), vec4(0.2f, 0.2f, 0.2f, 1.0f), lerpVal);
})");
	} else if (auto verRequest = std::dynamic_pointer_cast<BuiltInVertexShaderRequest>(pRequest)) {
		result = std::make_shared<renderer::VertexShader>();
		//language=glsl
		if (BuiltInVertexShaderRequest::getDefault() == verRequest)
			result->setSource(R"(#version 330
layout(location = 0) in vec4 position;
uniform mat4 mvp;

void main() {
  gl_Position = position;
})");
		/*result->setSource(R"(
#version 430 core

    layout (location = 0) in vec3 position;
    void main()
    {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    }
)");*/ /*result->setSource(R"(
#version 430 core

layout (location = 0) in vec3 vertexIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec4 colorsIn;
layout (location = 3) in vec3 normalsIn;


layout(std430, binding = 2) buffer ProgramWideSettings {
	mat4 viewProjMatrix;
};

out VS_OUT {
	vec2 uv;
	vec4 color;
} vs_out;


void main(){

	gl_Position = viewProjMatrix * mat4(1) * vec4(vertexIn, 1);
	vs_out.uv = uvIn;
	vs_out.color = colorsIn;

}
)");*/
	} else {
		pError->setStacktrace();
		pError->setTitle("Failed to compile the builtin shader");
		pError->setMessage("Invalid request");
		pError->addInfoLine("Shader name: {}", pRequest->getName());
		return nullptr;
	}
	result->compile();
	if (!result->getCompileStatus()) {
		pError->setStacktrace();
		pError->setTitle("Failed to compile the builtin shader");
		pError->setMessage("Error in shader code detected");
		pError->addInfoLine("Shader name: {}", pRequest->getName());
		std::string log;
		result->getInfoLog(log);
		pError->addInfoLine("Compiler log: {}", log);
		return nullptr;
	}
	return result;
}

} // namespace mer::sdk::main