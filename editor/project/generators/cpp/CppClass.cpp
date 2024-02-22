//
// Created by alexus on 20.01.24.
//

#include "CppClass.h"

#include "CppMethod.h"

namespace PROJECT_CORE {
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
} // namespace PROJECT_CORE