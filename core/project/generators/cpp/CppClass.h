//
// Created by alexus on 20.01.24.
//

#ifndef CPPCLASS_H
#define CPPCLASS_H
#include <project/generators/GeneratorElement.h>
#include <project/generators/cpp/AccessModifier.h>

namespace PROJECT_CORE {
class CppClass : public GeneratorElement {
	std::string name;
	std::list<std::string> implementsList;

	std::list<std::pair<AccessModifier, GeneratorElement*>> body;

public:
	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::list<std::string> &getImplementsList() const { return implementsList; }

	void setImplementsList(const std::list<std::string> &pImplementsList) { implementsList = pImplementsList; }

	void addImplement(const std::string &pClassName) { implementsList.emplace_back(pClassName); }

	void addElement(GeneratorElement* pElement, AccessModifier pModifier);

	std::string getDefinition(const uint32_t pIndent) override {
		const std::string tabs(pIndent, '\t');
		using namespace std::string_view_literals;
		std::stringstream ss;
		ss << tabs << "class " << name;
		if (!implementsList.empty()) {
			const auto str =
				std::ranges::fold_left(implementsList | std::views::join_with(", "sv), std::string{}, std::plus{});

			ss << " : " << str;
		}
		ss << " {\n";
		AccessModifier lastModifier = AccessModifier::PRIVATE;
		for (const auto &pair: body) {
			if (lastModifier != pair.first) {
				lastModifier = pair.first;
				ss << tabs << to_string(lastModifier) << ":\n";
			}
			ss << tabs << pair.second->getDefinition(pIndent + 1) << "\n";
		}
		ss << "};";

		return ss.str();
	}

	std::string getDeclaration() override { return "class " + name + ";"; }
};

} // namespace PROJECT_CORE


#endif //CPPCLASS_H
