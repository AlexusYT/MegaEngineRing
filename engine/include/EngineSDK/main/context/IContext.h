//
// Created by alexus on 08.01.24.
//

#ifndef ICONTEXT_H
#define ICONTEXT_H

namespace n::sdk::main {
class IContext {
public:
	virtual ~IContext() = default;
	virtual void makeCurrent() = 0;
};
} // namespace n::sdk::main


#endif //ICONTEXT_H
