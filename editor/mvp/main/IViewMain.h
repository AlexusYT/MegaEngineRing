//
// Created by alexus on 26.01.24.
//

#ifndef IVIEWMAIN_H
#define IVIEWMAIN_H
#include <mvp/IView.h>

namespace mer::editor::mvp {
class IViewMain : public IView {
public:
	virtual sigc::connection connectReloadCmakeClickedSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectRunClickedSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectKeyPressedSignal(
		const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const = 0;

	virtual void switchLogPage(int pId) const = 0;

	virtual void addLogMessage(int pId, const Glib::ustring &pMessage) = 0;

	virtual void clearLogMessage(int pId) = 0;

	virtual void setWindowTitle(const std::string &pTitle) = 0;

	virtual void addActionGroup(const std::string &pName, const Glib::RefPtr<Gio::SimpleActionGroup> &pActionGroup) = 0;

	virtual void reportError(const sdk::utils::ReportMessagePtr &pError) = 0;
};
} // namespace mer::editor::mvp


#endif //IVIEWMAIN_H
