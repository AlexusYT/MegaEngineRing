//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 17.09.24.
//

#ifndef MULTIPANED_H
#define MULTIPANED_H
#include "MultiPanedPanel.h"
#include "mvp/IPresenter.h"

namespace mer::editor::mvp {
class IView;
class MultiPanedContext;
class IPresenter;
} // namespace mer::editor::mvp

namespace mer::editor::ui {
class MultiPanedPanel;
class MultiPanedPanelDivider;

class MultiPaned : public Gtk::Widget {
	friend class MultiPanedPanel;
	std::vector<std::shared_ptr<MultiPanedPanelDivider>> dividers;
	std::vector<std::shared_ptr<MultiPanedPanel>> panels;
	sigc::slot<void(const Widget* pParentWidget, MultiPanedPanel* pPanel)> createWidgetSlot;
	MultiPanedPanelDivider* draggingDivider{};
	Gtk::Allocation allocationSelf;
	std::vector<std::shared_ptr<mvp::IPresenter>> presenters;

	sigc::signal<std::shared_ptr<mvp::IView>(const mvp::IPresenter* pPresenter,
											 const std::shared_ptr<mvp::MultiPanedContext> &pContext)>
		createWidgetSignal;

public:
	MultiPaned();

	~MultiPaned() override;

	// This signal handler is called only if MyContainer is a managed widget.
	void onContainerDestroy();

	Widget* getWidget() { return get_first_child(); }

	Widget* splitAt(Widget* pParentWidget, const std::shared_ptr<mvp::IPresenter> &pPresenter,
					const Gtk::Orientation pOrientation, const float pTargetChildSize = 0.5f) {
		if (auto widget = dynamic_cast<MultiPanedPanel*>(pParentWidget))
			return splitAtImpl(widget, pPresenter, pOrientation, pTargetChildSize).second;
		return nullptr;
	}

	Gtk::SizeRequestMode get_request_mode_vfunc() const override { return Gtk::SizeRequestMode::HEIGHT_FOR_WIDTH; }

	// Discover the total amount of minimum space and natural space needed by
	// this container and its children.
	void measure_vfunc(Gtk::Orientation pOrientation, int pForSize, int &pMinimum, int &pNatural, int &pMinimumBaseline,
					   int &pNaturalBaseline) const override;

	void size_allocate_vfunc(int pWidth, int pHeight, int pBaseline) override;

	sigc::connection connectCreateWidgetSignal(
		const sigc::slot<std::shared_ptr<mvp::IView>(const mvp::IPresenter* pPresenter,
													 const std::shared_ptr<mvp::MultiPanedContext> &pContext)> &pSlot) {
		return createWidgetSignal.connect(pSlot);
	}

	void addPresenter(const std::shared_ptr<mvp::IPresenter> &pPresenter) { presenters.push_back(pPresenter); }

	Widget* setRoot(const std::shared_ptr<mvp::IPresenter> &pPresenter);

	//void remove(const Gtk::Widget &pWidget);

private:
	MultiPanedPanelDivider* getDividerAt(double pX, double pY);

	MultiPanedPanel* getPanelAt(double pX, double pY);

	Gtk::Widget* createWidget(const std::shared_ptr<mvp::IPresenter> &pPresenter);

	std::pair<std::shared_ptr<MultiPanedPanelDivider>, MultiPanedPanel*> splitAtImpl(
		MultiPanedPanel* pParentWidget, const std::shared_ptr<mvp::IPresenter> &pPresenter,
		Gtk::Orientation pOrientation, float pTargetChildSize);

	void setDraggingDivider(MultiPanedPanelDivider* pDraggingDivider) { draggingDivider = pDraggingDivider; }
};
} // namespace mer::editor::ui


#endif //MULTIPANED_H
