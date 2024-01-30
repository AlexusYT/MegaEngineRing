//
// Created by alexus on 08.01.24.
//

#ifndef IWINDOW_H
#define IWINDOW_H

namespace n::sdk::main {
class IWindow {

public:
	virtual ~IWindow() = default;
	virtual void onRender(double pDeltaTime) = 0;

	virtual glm::ivec2 getWindowSize() = 0;
};
} // namespace n::sdk::main

#endif //IWINDOW_H
