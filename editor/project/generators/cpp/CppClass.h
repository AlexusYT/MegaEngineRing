//
// Created by alexus on 20.01.24.
//

#ifndef CPPCLASS_H
#define CPPCLASS_H
#include <project/generators/GeneratorElement.h>
#include <project/generators/cpp/AccessModifier.h>

#include "CppDeclaration.h"
#include "CppDefinition.h"

namespace PROJECT_CORE {
class CppClass : public GeneratorElement, public std::enable_shared_from_this<CppClass> {
	std::string name;
	std::list<std::string> implementsList;

	std::list<std::pair<AccessModifier, GeneratorElement*>> body;
	std::list<std::pair<AccessModifier, std::shared_ptr<CppDefinition>>> definitions;
	std::list<std::pair<AccessModifier, std::shared_ptr<CppDeclaration>>> declarations;

	class CppClassDeclaration : public CppDeclaration {
		std::shared_ptr<CppClass> parent;

	public:
		explicit CppClassDeclaration(const std::shared_ptr<CppClass> &pParent) : parent(pParent) {}

		std::string getCode(const uint32_t /*pIndent*/, bool /*pIsHeader*/) override {
			return "class " + parent->getName() + ";";
		}
	};

	class CppClassDefinition : public CppDefinition {
		std::shared_ptr<CppClass> parent;

	public:
		explicit CppClassDefinition(const std::shared_ptr<CppClass> &pParent) : parent(pParent) {}

		std::string getCode(const uint32_t pIndent, bool pIsHeader) override {
			return parent->definition(pIndent, pIsHeader);
		}
	};

	CppClass() = default;

	explicit CppClass(const std::string &pName) : name(pName) {}

public:
	static std::shared_ptr<CppClass> create() { return std::shared_ptr<CppClass>(new CppClass); }

	static std::shared_ptr<CppClass> create(const std::string &pName) {
		return std::shared_ptr<CppClass>(new CppClass(pName));
	}

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::list<std::string> &getImplementsList() const { return implementsList; }

	void setImplementsList(const std::list<std::string> &pImplementsList) { implementsList = pImplementsList; }

	void addImplement(const std::string &pClassName) { implementsList.emplace_back(pClassName); }

	void addDefinition(std::shared_ptr<CppDefinition> pElement, AccessModifier pModifier);

	void addDeclaration(std::shared_ptr<CppDeclaration> pElement, AccessModifier pModifier);

	std::shared_ptr<CppClassDeclaration> getDeclaration() {
		return std::make_shared<CppClassDeclaration>(shared_from_this());
	}

	std::shared_ptr<CppClassDefinition> getDefinition() {
		return std::make_shared<CppClassDefinition>(shared_from_this());
	}

	std::string definition(const uint32_t pIndent, bool pIsHeader) {
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
		for (const auto &pair: definitions) {
			if (lastModifier != pair.first) {
				lastModifier = pair.first;
				ss << tabs << to_string(lastModifier) << ":\n";
			}
			ss << tabs << pair.second->getCode(pIndent + 1, pIsHeader) << "\n";
		}
		for (const auto &pair: declarations) {
			if (lastModifier != pair.first) {
				lastModifier = pair.first;
				ss << tabs << to_string(lastModifier) << ":\n";
			}
			ss << tabs << pair.second->getCode(pIndent + 1, pIsHeader) << "\n";
		}
		ss << "};";

		return ss.str();
	}
};

} // namespace PROJECT_CORE


#endif //CPPCLASS_H
