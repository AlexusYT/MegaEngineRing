//
// Created by alexus on 21.01.24.
//

#ifndef CPPBODYSTATEMENT_H
#define CPPBODYSTATEMENT_H

namespace mer::editor::project {
class CppBodyStatement {
public:
	virtual ~CppBodyStatement() = default;
	virtual std::string getCode(uint32_t pIndent) = 0;
};
} // namespace mer::editor::project

#endif //CPPBODYSTATEMENT_H
