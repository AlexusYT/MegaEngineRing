//
// Created by alexus on 02.02.24.
//

#include "TestScene.h"

#include <EngineSDK/main/resources/LazyResource.h>

#include "EngineSDK/main/resources/ResourceRequests.h"
#include "EngineSDK/main/resources/shaders/BuiltInProgramRequest.h"
#include "EngineSDK/main/resources/shaders/BuiltInShaderRequest.h"
#include "EngineSDK/renderer/buffers/SSBO.h"
#include "EngineSDK/renderer/shaders/FragmentShader.h"
#include "EngineSDK/renderer/shaders/ShaderProgram.h"
#include "EngineSDK/renderer/shaders/VertexShader.h"
#include "EngineUtils/utils/Logger.h"

using namespace n::sdk::main;
using namespace n::engine::utils;
extern "C" {
void* loadTestScene() { return new TestScene(); }
}

ReportMessagePtr TestScene::preloadScene(const std::shared_ptr<ResourceRequests> &pRequests) {
	pRequests->addRequest(BuiltInProgramRequest::getDefaultProgram());
	pRequests->addRequest(BuiltInFragmentShaderRequest::getDefault());
	pRequests->addRequest(BuiltInVertexShaderRequest::getDefault());


	return nullptr;
}

ReportMessagePtr TestScene::initScene() {
	auto shader = getResourceByRequest(BuiltInProgramRequest::getDefaultProgram());
	auto frag = getResourceByRequest(BuiltInFragmentShaderRequest::getDefault());
	auto vert = getResourceByRequest(BuiltInVertexShaderRequest::getDefault());

	Logger::out("{}", vert->getCompileStatus());
	Logger::out("{}", frag->getCompileStatus());
	Logger::out("{}", shader->getSync<n::sdk::renderer::ShaderProgram>()->getLinkStatus());
	return nullptr;
}