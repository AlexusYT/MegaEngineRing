//
// Created by alexus on 31.01.24.
//

#ifndef BUILTINSHADERREQUEST_H
#define BUILTINSHADERREQUEST_H
#include <functional>

#include "EngineSDK/main/resources/ResourceRequest.h"

namespace n::sdk::renderer {
class VertexShader;
class FragmentShader;
} // namespace n::sdk::renderer

namespace n::sdk::main {
class BuiltInShaderLoader : public ResourceLoader {
public:
	std::shared_ptr<IResource> load(const std::shared_ptr<ResourceRequest> &pRequest,
									const engine::utils::ReportMessagePtr &pError,
									const std::shared_ptr<Resources> &pDependencies) override;
};

class BuiltInShaderRequest : public RegularRequest {
	std::string name;

protected:
	explicit BuiltInShaderRequest(const std::string &pName) : name(pName) {}

public:
	using ResourceT = renderer::FragmentShader;

	[[nodiscard]] std::shared_ptr<ResourceLoader> getLoader() const override {
		return std::make_shared<BuiltInShaderLoader>();
	}

	[[nodiscard]] std::string getName() const override { return name; }
};

class BuiltInFragmentShaderRequest final : public BuiltInShaderRequest {
	inline static std::shared_ptr<BuiltInFragmentShaderRequest> defaultFragment;

	explicit BuiltInFragmentShaderRequest(const std::string &pName)
		: BuiltInShaderRequest("__BuiltIn_" + pName + "Fragment__") {}

public:
	using ResourceT = renderer::FragmentShader;

	static std::shared_ptr<BuiltInFragmentShaderRequest> getDefault() {
		if (!defaultFragment) { defaultFragment.reset(new BuiltInFragmentShaderRequest("Default")); }
		return defaultFragment;
	}
};

class BuiltInVertexShaderRequest final : public BuiltInShaderRequest {
	inline static std::shared_ptr<BuiltInVertexShaderRequest> defaultVertex;

	explicit BuiltInVertexShaderRequest(const std::string &pName)
		: BuiltInShaderRequest("__BuiltIn_" + pName + "Vertex__") {}

public:
	using ResourceT = renderer::VertexShader;

	static std::shared_ptr<BuiltInVertexShaderRequest> getDefault() {
		if (!defaultVertex) { defaultVertex.reset(new BuiltInVertexShaderRequest("Default")); }
		return defaultVertex;
	}
};

} // namespace n::sdk::main


#endif //BUILTINSHADERREQUEST_H
