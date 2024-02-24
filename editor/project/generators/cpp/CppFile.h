//
// Created by alexus on 08.02.24.
//

#ifndef CPPFILE_H
#define CPPFILE_H

namespace PROJECT_CORE {
class GeneratorElement;
class CppDefinition;
class CppDeclaration;

class CppFile {
	std::vector<std::string> includes;
	std::vector<std::shared_ptr<CppDefinition>> definitions;
	std::vector<std::shared_ptr<CppDeclaration>> declarations;

public:
	virtual ~CppFile() = default;

	void addDefinition(std::shared_ptr<CppDefinition> pElement) { definitions.emplace_back(pElement); }

	void addDeclaration(std::shared_ptr<CppDeclaration> pElement) { declarations.emplace_back(pElement); }

	[[nodiscard]] const std::vector<std::shared_ptr<CppDefinition>> &getDefinitions() const { return definitions; }

	[[nodiscard]] const std::vector<std::shared_ptr<CppDeclaration>> &getDeclarations() const { return declarations; }

	void addInclude(const std::string &pInclude, bool pGlobal = true) {
		std::string include;
		if (pGlobal) include = std::format("<{}>\n", pInclude);
		else
			include = std::format("\"{}\"\n", pInclude);
		includes.emplace_back(include);
	}

	[[nodiscard]] const std::vector<std::string> &getIncludes() const { return includes; }

	virtual std::string getExtension() = 0;

	virtual bool isHeader() = 0;

	n::engine::utils::ReportMessagePtr writeFile(const std::filesystem::path &pPath);
};

} // namespace PROJECT_CORE

#endif //CPPFILE_H