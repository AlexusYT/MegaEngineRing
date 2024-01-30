//
// Created by alexus on 25.01.24.
//

#ifndef IVIEWSTARTUP_H
#define IVIEWSTARTUP_H

namespace MVP_CORE {

class IViewStartup {
public:
	virtual ~IViewStartup() = default;

	virtual sigc::connection connectNewProjectClickSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectCancelClickSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectCreateClickSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectOpenProjectClickSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectPathChangedSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectIconReleasedSignal(
		const sigc::slot<void(Gtk::Entry::IconPosition)> &pSlot) const = 0;

	virtual sigc::connection connectNameChangedSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual void setPath(const std::string &pPath) const = 0;

	virtual std::string getPath() const = 0;

	virtual void setName(const std::string &pPath) const = 0;

	virtual std::string getName() const = 0;

	virtual void switchMode(int pMode) = 0;

	virtual void showError(const std::string &pText) const = 0;

	virtual void hideError() const = 0;

	virtual std::shared_ptr<Gtk::FileDialog> showFolderChooserDialog(
		const std::string &pInitialFolder,
		const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
			&pSlot) = 0;

	virtual void showFileChooserDialog(
		const std::string &pInitialFolder,
		const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
			&pSlot) = 0;

	virtual void addWindow(const std::shared_ptr<Gtk::Window> &pWindow) = 0;

	virtual void closeWindow() = 0;
};
} // namespace MVP_CORE


#endif //IVIEWSTARTUP_H
