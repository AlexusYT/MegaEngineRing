//
// Created by alexus on 24.01.24.
//

#ifndef IVIEW_H
#define IVIEW_H
#include "ThreadDispatcher.h"

namespace n::core::mvp {
class IView : public ThreadDispatcher {
public:
	virtual ~IView() = default;

	virtual void addWindow(const std::shared_ptr<Gtk::Window> &pWindow) = 0;

	virtual void closeWindow() = 0;
};
} // namespace n::core::mvp


#endif //IVIEW_H
