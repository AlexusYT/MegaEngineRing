//
// Created by alexus on 06.02.24.
//

#ifndef IVIEWSCENEEDITOR_H
#define IVIEWSCENEEDITOR_H
#include <mvp/ThreadDispatcher.h>

namespace mer::editor::mvp {
class IViewSceneEditor : public ThreadDispatcher {
public:
	virtual ~IViewSceneEditor() = default;

	virtual Gtk::Widget &getMainWidget() = 0;

	virtual sigc::connection connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) = 0;

	virtual sigc::connection connectRealize(const sigc::slot<void()> &pSlot) = 0;

	virtual sigc::connection connectUnrealize(const sigc::slot<void()> &pSlot) = 0;

	virtual sigc::connection connectResize(const sigc::slot<void(int pWidth, int pHeight)> &pSlot) = 0;

	virtual void makeCurrent() = 0;

	virtual void redraw() = 0;


	virtual void throwIfError() = 0;

	virtual void onLoadingStarted() = 0;

	virtual void onLoadingStopped(const sdk::utils::ReportMessagePtr &pError) = 0;
};
} // namespace mer::editor::mvp


#endif //IVIEWSCENEEDITOR_H
