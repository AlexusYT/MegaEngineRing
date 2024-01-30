//
// Created by alexus on 20.01.24.
//

#ifndef GENERATORELEMENT_H
#define GENERATORELEMENT_H

namespace PROJECT_CORE {

class GeneratorElement {

public:
	virtual ~GeneratorElement() = default;
	virtual std::string getDefinition(uint32_t pIndent) = 0;
	virtual std::string getDeclaration() = 0;
};

} // namespace PROJECT_CORE

#endif //GENERATORELEMENT_H
