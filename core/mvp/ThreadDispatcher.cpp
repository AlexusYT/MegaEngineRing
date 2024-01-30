//
// Created by alexus on 27.01.24.
//

#include "ThreadDispatcher.h"

n::core::mvp::ThreadDispatcher::ThreadDispatcher() {
	dispatcher.connect([this] {
		std::lock_guard lock(queueMutex);
		for (const auto &[fst, snd]: dispatcherQueue) { fst(snd); }

		dispatcherQueue.clear();
		queueRunning = false;
	});
}

std::future<void> n::core::mvp::ThreadDispatcher::executeInMainThread(const SlotT &pSlot) {
	std::lock_guard lock(queueMutex);
	std::promise<void> &prom = dispatcherQueue.emplace_back(pSlot, std::promise<void>()).second;
	if (!queueRunning) {
		dispatcher.emit();
		queueRunning = true;
	}
	return prom.get_future();
}