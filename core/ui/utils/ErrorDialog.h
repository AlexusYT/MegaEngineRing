//
// Created by alexus on 12.01.24.
//

#ifndef UIUTILS_H
#define UIUTILS_H

class ErrorDialog {
	inline static n::engine::utils::ReportMessageUPtr message;
	Gtk::Window* rootWindow;

	class DetailsDialog : public Gtk::Window {
		Gtk::Box mainBox, footerBox;
		Gtk::ScrolledWindow scrolledWindow;
		Gtk::Label contentLabel;
		Gtk::Button closeBtn, copyBtn;
		n::engine::utils::ReportMessageUPtr message;

	public:
		DetailsDialog(n::engine::utils::ReportMessageUPtr pMessage) : message(std::move(pMessage)) {
			set_destroy_with_parent(true);
			set_title("Name Dialog");
			mainBox.append(scrolledWindow);
			mainBox.append(footerBox);
			mainBox.set_orientation(Gtk::Orientation::VERTICAL);
			scrolledWindow.set_max_content_height(400);
			scrolledWindow.set_child(contentLabel);
			scrolledWindow.set_expand();
			footerBox.append(copyBtn);
			footerBox.append(closeBtn);
			contentLabel.set_text(message->getReport());
			contentLabel.set_selectable();
			contentLabel.select_region(0, 0);
			copyBtn.set_label("Скопировать");
			copyBtn.signal_clicked().connect(
				[this] { contentLabel.get_clipboard()->set_text(contentLabel.get_text()); });
			closeBtn.set_label("Закрыть");
			closeBtn.signal_clicked().connect([this] { this->close(); });
			set_child(mainBox);
			mainBox.set_child_visible(true);
		}
	};

	inline static std::unique_ptr<DetailsDialog> errorDetailsDialog;

public:
	static void showErrorDialog(Gtk::Root* pRoot, n::engine::utils::ReportMessageUPtr &pMessage) {
		message.swap(pMessage);
		n::engine::utils::Logger::error(message);
		Glib::RefPtr<Gtk::AlertDialog> dialog = Gtk::AlertDialog::create();

		dialog->set_message(message->getTitle());
		dialog->set_detail(message->getMessage());
		dialog->set_modal();
		dialog->set_buttons({"Подробнее", "Закрыть"});
		dialog->set_default_button(0);
		dialog->set_cancel_button(1);
		Gtk::Window* rootWindow(dynamic_cast<Gtk::Window*>(pRoot));
		dialog->choose(*rootWindow, sigc::bind(&onDialogButtonsClicked, dialog, rootWindow));
	}

private:
	static void onDialogButtonsClicked(Glib::RefPtr<Gio::AsyncResult> &pResult,
									   const Glib::RefPtr<Gtk::AlertDialog> &pDialog, Gtk::Window* pRootWindow) {
		try {
			switch (const int responseId = pDialog->choose_finish(pResult)) {
				case 0: openDetailsDialog(pRootWindow); break;
				case 1: break;
				default: std::cout << "Unexpected response: " << responseId << std::endl; break;
			}
		} catch (const Gtk::DialogError &err) {
			// Can be thrown by m_pDialog->choose_finish(result).
			std::cout << "DialogError, " << err.what() << std::endl;
		} catch (const Glib::Error &err) { std::cout << "Unexpected exception. " << err.what() << std::endl; }
	}

public:
	static void openDetailsDialog(Gtk::Window* pWindow) {
		errorDetailsDialog = std::make_unique<DetailsDialog>(std::move(message));
		errorDetailsDialog->set_transient_for(*pWindow);
		errorDetailsDialog->set_default_size(800, 600);
		//errorDetailsDialog.set_modal();

		//pWindow->get_application()->add_window(*errorDetailsDialog);
		//errorDetailsDialog->set_hide_on_close();
		errorDetailsDialog->set_visible(true);
		errorDetailsDialog->signal_close_request().connect(
			[] {
				errorDetailsDialog = nullptr;
				return true;
			},
			true);


	}
};


#endif //UIUTILS_H
