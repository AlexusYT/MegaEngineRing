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
// Created by alexus on 31.01.24.
//

#ifndef BUILTINSHADERREQUEST_H
#define BUILTINSHADERREQUEST_H
#include <functional>

#include "EngineSDK/main/resources/ResourceRequest.h"

namespace mer::sdk::renderer {
class VertexShader;
class FragmentShader;
} // namespace mer::sdk::renderer

namespace mer::sdk::main {
class BuiltInShaderLoader : public ResourceLoader {
public:
	std::shared_ptr<IResource> load(const std::shared_ptr<ResourceRequest> &pRequest,
									const sdk::utils::ReportMessagePtr &pError,
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

} // namespace mer::sdk::main


#endif //BUILTINSHADERREQUEST_H
