//
// Created by alexus on 20.01.24.
//

#include "CppClass.h"

#include "CppMethod.h"

namespace PROJECT_CORE {
void CppClass::addElement(GeneratorElement* pElement, AccessModifier pModifier) {
	if (auto* method = dynamic_cast<CppMethod*>(pElement)) { method->setClass(this); }
	body.emplace_back(pModifier, pElement);
}
} // namespace PROJECT_CORE