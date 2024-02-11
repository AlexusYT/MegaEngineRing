//
// Created by alexus on 08.01.24.
//

#ifndef ICONTEXT_H
#define ICONTEXT_H

namespace n::sdk::main {
class IContext {
public:
	virtual ~IContext() = default;

	virtual engine::utils::ReportMessagePtr init() = 0;

	virtual void makeCurrent() = 0;

	virtual void swapBuffers() const = 0;
};
} // namespace n::sdk::main


#endif //ICONTEXT_H
