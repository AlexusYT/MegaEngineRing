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

#ifndef BUILTINPROGRAMREQUEST_H
#define BUILTINPROGRAMREQUEST_H
#include "EngineSDK/renderer/shaders/ShaderProgram.h"
#include "ShaderProgramRequest.h"

namespace mer::sdk::main {
class BuiltInProgramRequest final : public ShaderProgramRequest {

public:
	class BuiltInProgramLoader : public ResourceLoader {
	public:
		utils::ReportMessagePtr load(IResourceLoadExecutor* pLoadExecutor, std::shared_ptr<std::istream> &pStream,
									 std::shared_ptr<IResource> &pResourceOut) override;

	private:
		std::string getFileExtension() override { return "enshader"; }

		std::string readString(const std::shared_ptr<std::istream> &pStream) {
			size_t size = 0;
			pStream->read(reinterpret_cast<std::istream::char_type*>(&size), sizeof(size));
			std::string name(size, 0);
			pStream->read(name.data(), static_cast<long int>(size));
			return name;
		}
	};

	static std::shared_ptr<BuiltInProgramRequest> defaultProgram;

	explicit BuiltInProgramRequest(const std::string &pName)
		: ShaderProgramRequest("__BuiltIn_" + pName + "Program__") {}

public:
	[[nodiscard]] static const std::shared_ptr<BuiltInProgramRequest> &getDefaultProgram();

private:
	[[nodiscard]] std::shared_ptr<ResourceLoader> getLoader() const override {
		return std::make_shared<BuiltInProgramLoader>();
	}
};

} // namespace mer::sdk::main


#endif //BUILTINPROGRAMREQUEST_H
