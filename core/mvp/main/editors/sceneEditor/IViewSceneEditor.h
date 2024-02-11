//
// Created by alexus on 06.02.24.
//

#ifndef IVIEWSCENEEDITOR_H
#define IVIEWSCENEEDITOR_H
#include <mvp/ThreadDispatcher.h>

namespace MVP_CORE {
class IViewSceneEditor : public ThreadDispatcher {
public:
	virtual ~IViewSceneEditor() = default;

	virtual Gtk::Widget &getMainWidget() = 0;

	virtual sigc::connection connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) = 0;

	virtual void onLoadingStarted() = 0;

	virtual void onLoadingStopped(const engine::utils::ReportMessagePtr &pError) = 0;
};
} // namespace MVP_CORE


#endif //IVIEWSCENEEDITOR_H
