//
// Created by alexus on 21.01.24.
//

#ifndef CPPBODYSTATEMENT_H
#define CPPBODYSTATEMENT_H

namespace PROJECT_CORE {
class CppBodyStatement {
public:
	virtual ~CppBodyStatement() = default;
	virtual std::string getCode(uint32_t pIndent) = 0;
};
} // namespace PROJECT_CORE

#endif //CPPBODYSTATEMENT_H
