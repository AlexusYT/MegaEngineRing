//
// Created by alexus on 09.02.24.
//

#ifndef CPPEXTERNC_H
#define CPPEXTERNC_H
#include "CppDefinition.h"

namespace PROJECT_CORE {
class CppExternC : public std::enable_shared_from_this<CppExternC> {
	std::list<std::shared_ptr<CppDefinition>> definitions;

	class CppExternCDefinition : public n::core::project::CppDefinition {
		std::shared_ptr<CppExternC> parent;

	public:
		explicit CppExternCDefinition(const std::shared_ptr<CppExternC> &pParent) : parent(pParent) {}

		std::string getCode(const uint32_t pIndent, bool pIsHeader) override {
			return parent->definition(pIndent, pIsHeader);
		}
	};

	CppExternC() = default;

public:
	static std::shared_ptr<CppExternC> create() { return std::shared_ptr<CppExternC>(new CppExternC); }

	void addDefinition(std::shared_ptr<CppDefinition> pElement) { definitions.emplace_back(pElement); }

	std::shared_ptr<CppExternCDefinition> getDefinition() {
		return std::make_shared<CppExternCDefinition>(shared_from_this());
	}

	std::string definition(const uint32_t pIndent, bool pIsHeader) const {
		const std::string tabs(pIndent, '\t');
		std::stringstream ss;
		ss << "extern \"C\"{\n";
		for (const auto &cppDefinition: definitions) {

			ss << tabs << cppDefinition->getCode(pIndent + 1, pIsHeader) << "\n";
		}
		ss << "}\n";
		return ss.str();
	}
};
} // namespace PROJECT_CORE

#endif //CPPEXTERNC_H
