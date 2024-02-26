//
// Created by alexus on 04.02.24.
//

#ifndef BUILTINPROGRAMREQUEST_H
#define BUILTINPROGRAMREQUEST_H
#include "EngineSDK/main/resources/ResourceRequest.h"
#include "EngineSDK/renderer/shaders/ShaderProgram.h"

namespace mer::sdk::main {
class BuiltInProgramRequest final : public RegularRequest {

	class BuiltInProgramLoader : public ResourceLoader {
	public:
		std::shared_ptr<IResource> load(const std::shared_ptr<ResourceRequest> &pRequest,
										const sdk::utils::ReportMessagePtr &pError,
										const std::shared_ptr<Resources> &pDependencies) override;
	};

	static std::shared_ptr<BuiltInProgramRequest> defaultProgram;

	std::string name;

	explicit BuiltInProgramRequest(const std::string &pName) : name("__BuiltIn_" + pName + "Program__") {}

public:
	using ResourceT = renderer::ShaderProgram;

	[[nodiscard]] std::string getName() const override { return name; }

	[[nodiscard]] static const std::shared_ptr<BuiltInProgramRequest> &getDefaultProgram();

private:
	[[nodiscard]] std::shared_ptr<ResourceLoader> getLoader() const override {
		return std::make_shared<BuiltInProgramLoader>();
	}
};

} // namespace mer::sdk::main


#endif //BUILTINPROGRAMREQUEST_H
