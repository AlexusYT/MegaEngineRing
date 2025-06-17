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
// Created by alexus on 09.02.24.
//

#ifndef CPPEXTERNC_H
#define CPPEXTERNC_H
#include "CppDefinition.h"

namespace ked {
class CppExternC : public std::enable_shared_from_this<CppExternC> {
	std::list<std::shared_ptr<CppDefinition>> definitions;

	class CppExternCDefinition : public ked::CppDefinition {
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
} // namespace ked

#endif //CPPEXTERNC_H
