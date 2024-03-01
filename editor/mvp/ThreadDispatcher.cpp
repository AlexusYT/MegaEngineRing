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
// Created by alexus on 27.01.24.
//

#include "ThreadDispatcher.h"

mer::editor::mvp::ThreadDispatcher::ThreadDispatcher() {
	dispatcher.connect([this] {
		std::lock_guard lock(queueMutex);
		for (const auto &[fst, snd]: dispatcherQueue) { fst(snd); }

		dispatcherQueue.clear();
		queueRunning = false;
	});
}

std::future<void> mer::editor::mvp::ThreadDispatcher::executeInMainThread(const SlotT &pSlot) {
	std::lock_guard lock(queueMutex);
	std::promise<void> &prom = dispatcherQueue.emplace_back(pSlot, std::promise<void>()).second;
	if (!queueRunning) {
		dispatcher.emit();
		queueRunning = true;
	}
	return prom.get_future();
}