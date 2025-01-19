//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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
