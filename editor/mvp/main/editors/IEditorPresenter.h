//
// Created by alexus on 20.01.24.
//

#ifndef IEDITOR_H
#define IEDITOR_H

namespace mer::editor::mvp {

class IEditorPresenter {
public:
	virtual ~IEditorPresenter() = default;

	virtual operator Gtk::Widget&() = 0;

	virtual sigc::connection connectTabHeaderChanged(const sigc::slot<void(const std::string &pName)> &pSlot) = 0;
};

} // namespace mer::editor::mvp

#endif //IEDITOR_H
