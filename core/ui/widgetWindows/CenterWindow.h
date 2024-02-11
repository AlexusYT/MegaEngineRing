//
// Created by alexus on 19.01.24.
//

#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H

namespace n::core::mvp {
class IEditorPresenter;
} // namespace n::core::mvp

namespace UI_CORE {
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
} // namespace UI_CORE


#endif //CENTERWINDOW_H
