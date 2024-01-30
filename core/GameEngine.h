//
// Created by alexus on 05.12.23.
//

#ifndef GAMEENGINE_GAMEENGINE_H
#define GAMEENGINE_GAMEENGINE_H

#include <memory>
#include <mvp/IPresenter.h>

namespace n::core::ui {
class GameEngine {

public:
	static int run(int pArgc, char* pArgv[]);


	static void addWindow(std::shared_ptr<mvp::IPresenter> pWindow);
	static void removeWindow(std::shared_ptr<mvp::IPresenter> pWindow);
};
} // namespace n::core::ui

#endif //GAMEENGINE_GAMEENGINE_H
