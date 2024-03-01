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
// Created by alexus on 20.01.24.
//

#include "CppClass.h"

#include "CppMethod.h"

namespace mer::editor::project {
/*void CppClass::addElement(GeneratorElement* pElement, AccessModifier pModifier) {
	if (auto* method = dynamic_cast<CppMethod*>(pElement)) { method->setClass(this); }
	body.emplace_back(pModifier, pElement);
}*/

void CppClass::addDefinition(std::shared_ptr<CppDefinition> pElement, AccessModifier pModifier) {
	pElement->setKlass(this);
	definitions.emplace_back(pModifier, pElement);
}

void CppClass::addDeclaration(std::shared_ptr<CppDeclaration> pElement, AccessModifier pModifier) {
	pElement->setKlass(this);
	declarations.emplace_back(pModifier, pElement);
}
} // namespace mer::editor::project