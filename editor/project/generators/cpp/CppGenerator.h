//
// Created by alexus on 20.01.24.
//

#ifndef CPPGENERATOR_H
#define CPPGENERATOR_H
#include <project/generators/GeneratorElement.h>

namespace PROJECT_CORE {
class CppGenerator {
	std::vector<std::string> includes;
	std::vector<GeneratorElement*> elements;

public:
	[[nodiscard]] const std::vector<std::string> &getIncludes() const { return includes; }

	void setIncludes(const std::vector<std::string> &pIncludes) { includes = pIncludes; }

	void addInclude(const std::string &pInclude) { includes.emplace_back(pInclude); }

	[[nodiscard]] const std::vector<GeneratorElement*> &getElements() const { return elements; }

	void setElements(const std::vector<GeneratorElement*> &pElements) { elements = pElements; }

	void addElement(GeneratorElement* pElement) { elements.emplace_back(pElement); }

	n::engine::utils::ReportMessagePtr saveHeader(const std::filesystem::path &pPath) const {
		create_directories(pPath.parent_path());
		std::ofstream stream;
		try {
			stream.exceptions(std::_S_badbit | std::_S_eofbit | std::_S_failbit);
			stream.open(pPath);
		} catch (...) {

			auto msg = n::engine::utils::ReportMessage::create();
			msg->setTitle("Failed to save header file.");
			msg->setMessage("Exception thrown");
			msg->addInfoLine("Trying to save file at {}", pPath.string());
			return msg;
		}
		//language=cpp
		stream << R"(/*
 * This file is automatically generated. DO NOT EDIT! Your changes will be overwritten by the editor.
 */
)";
		for (const auto &include: includes) { stream << std::format("#include <{}>\n", include); }
		stream << "\n";

		return nullptr;
	}

	n::engine::utils::ReportMessagePtr saveSource(const std::filesystem::path &pPath) const {
		create_directories(pPath.parent_path());
		std::ofstream stream;
		try {
			stream.exceptions(std::_S_badbit | std::_S_eofbit | std::_S_failbit);
			stream.open(pPath);
		} catch (...) {

			auto msg = n::engine::utils::ReportMessage::create();
			msg->setTitle("Failed to save source file.");
			msg->setMessage("Exception thrown");
			msg->addInfoLine("Trying to save file at {}", pPath.string());
			return msg;
		}
		//language=cpp
		stream << R"(/*
 * This file is automatically generated. DO NOT EDIT! Your changes will be overwritten by the editor.
 */
)";

		//for (const auto &element: elements) { stream << element->getForSource(0); }
		return nullptr;
	}
};
} // namespace PROJECT_CORE


#endif //CPPGENERATOR_H