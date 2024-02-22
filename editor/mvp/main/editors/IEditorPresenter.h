//
// Created by alexus on 20.01.24.
//

#ifndef IEDITOR_H
#define IEDITOR_H

namespace MVP_CORE {

class IEditorPresenter {
public:
	virtual ~IEditorPresenter() = default;

	virtual operator Gtk::Widget&() = 0;

	virtual sigc::connection connectTabHeaderChanged(const sigc::slot<void(const std::string &pName)> &pSlot) = 0;
};

} // namespace MVP_CORE

#endif //IEDITOR_H
