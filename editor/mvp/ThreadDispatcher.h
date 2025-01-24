//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 27.01.24.
//

#ifndef THREADDISPATCHER_H
#define THREADDISPATCHER_H
#include <future>

namespace mer::editor::mvp {
class ThreadDispatcher {
public:
	using SlotT = sigc::slot<void(std::promise<void>)>;

private:
	Glib::Dispatcher dispatcher;
	std::vector<std::pair<SlotT, std::promise<void>>> dispatcherQueue;
	std::mutex queueMutex;
	bool queueRunning{};
	std::thread::id mainThread{};

public:
	ThreadDispatcher();

	void notifyDispatcher() { dispatcher.emit(); }

	std::future<void> executeInMainThread(const SlotT &pSlot);
};
} // namespace mer::editor::mvp

#endif //THREADDISPATCHER_H
