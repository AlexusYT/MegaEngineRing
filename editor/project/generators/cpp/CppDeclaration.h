//
// Created by alexus on 08.02.24.
//

#ifndef CPPDECLARATION_H
#define CPPDECLARATION_H
#include "CppClassMember.h"

namespace mer::editor::project {
class CppDeclaration : public CppClassMember {
public:
	virtual ~CppDeclaration() = default;
	virtual std::string getCode(uint32_t pIndent, bool pIsHeader) = 0;
};
} // namespace mer::editor::project


#endif //CPPDECLARATION_H
