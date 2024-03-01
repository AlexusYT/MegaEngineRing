// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 05.12.23.
//

#ifndef GAMEENGINE_GAMEENGINE_H
#define GAMEENGINE_GAMEENGINE_H

#include <memory>
#include <mvp/IPresenter.h>

namespace mer::editor::ui {
class GameEngine {

public:
	static int run(int pArgc, char* pArgv[]);


	static void addWindow(std::shared_ptr<mvp::IPresenter> pWindow);
	static void removeWindow(std::shared_ptr<mvp::IPresenter> pWindow);
};
} // namespace mer::editor::ui

#endif //GAMEENGINE_GAMEENGINE_H
