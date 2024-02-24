//
// Created by alexus on 19.01.24.
//

#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H

namespace mer::editor::mvp {
class IEditorPresenter;
} // namespace mer::editor::mvp

namespace mer::editor::ui {
class ProjectExplorerEntry;

class CenterWindow {
	Gtk::Notebook mainNotebook;

	struct Page {
		int pageNum;
		std::shared_ptr<mvp::IEditorPresenter> widget;
	};

	std::unordered_map<ProjectExplorerEntry* /*entry*/, Page /*pageInfo*/> openedPages;
	std::vector<Gtk::Window> windows;

public:
	CenterWindow();

	operator Gtk::Widget&() { return mainNotebook; }

	void openEntry(ProjectExplorerEntry* pEntry);
};
} // namespace mer::editor::ui


#endif //CENTERWINDOW_H
