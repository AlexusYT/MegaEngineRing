//
// Created by alexus on 22.01.24.
//

#ifndef CPPCUSTOMSTATEMENT_H
#define CPPCUSTOMSTATEMENT_H
#include "CppBodyStatement.h"

namespace mer::editor::project {

class CppCustomStatement : public CppBodyStatement {
	std::string line;

	CppCustomStatement() = default;

	CppCustomStatement(const std::string &pLine) : line(pLine) {}

public:
	static std::shared_ptr<CppCustomStatement> create(const std::string &pLine) {
		return std::shared_ptr<CppCustomStatement>(new CppCustomStatement(pLine));
	}

	static std::shared_ptr<CppCustomStatement> create() {
		return std::shared_ptr<CppCustomStatement>(new CppCustomStatement());
	}

	[[nodiscard]] const std::string &getLine() const { return line; }

	void setLine(const std::string &pLine) { line = pLine; }

	std::string getCode(const uint32_t pIndent) override {
		const std::string tabs(pIndent, '\t');
		using namespace std::string_view_literals;
		std::stringstream ss;
		ss << tabs << line << ";";
		return ss.str();
	}
};
} // namespace mer::editor::project

#endif //CPPCUSTOMSTATEMENT_H
