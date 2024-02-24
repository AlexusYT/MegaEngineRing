//
// Created by alexus on 20.01.24.
//

#ifndef CPPEDITOR_H
#define CPPEDITOR_H

#include <mvp/main/editors/IEditorPresenter.h>
#include <thread>

namespace mer::editor::mvp {
class CppEditor : public IEditorPresenter {
	std::filesystem::path filePath;
	Gtk::TextView mainTextView;

public:
	explicit CppEditor(const std::filesystem::path &pFilePath) : filePath(pFilePath) {


		/*std::thread thread([](const std::filesystem::path &pFilePath) {}, filePath);
		thread.detach();*/
	}

	operator Gtk::Widget&() override { return mainTextView; }

	sigc::connection connectTabHeaderChanged(const sigc::slot<void(const std::string &pName)> &/*pSlot*/) override {
		return {};
	}
};
} // namespace mer::editor::mvp
#endif //CPPEDITOR_H
