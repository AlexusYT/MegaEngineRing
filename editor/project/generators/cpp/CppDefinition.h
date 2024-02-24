//
// Created by alexus on 08.02.24.
//

#ifndef CPPDEFINITION_H
#define CPPDEFINITION_H
#include "CppClassMember.h"

namespace mer::editor::project {
class CppDefinition : public CppClassMember {
public:
	virtual ~CppDefinition() = default;

	virtual std::string getCode(uint32_t pIndent, bool pIsHeader) = 0;
};
} // namespace mer::editor::project


#endif //CPPDEFINITION_H
