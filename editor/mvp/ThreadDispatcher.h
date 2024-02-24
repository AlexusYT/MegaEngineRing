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

public:
	ThreadDispatcher();

	void notifyDispatcher() { dispatcher.emit(); }

	std::future<void> executeInMainThread(const SlotT &pSlot);
};
} // namespace mer::editor::mvp

#endif //THREADDISPATCHER_H
