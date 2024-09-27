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

#include "MultiPaned.h"

#include "MultiPanedPanel.h"
#include "mvp/contexts/MultiPanedContext.h"

namespace mer::editor::ui {


MultiPaned::MultiPaned() {
	signal_destroy().connect(mem_fun(*this, &MultiPaned::onContainerDestroy));

	auto drag = Gtk::GestureDrag::create();
	drag->signal_drag_begin().connect([this](const double pX, const double pY) {
		if (const auto panel = getPanelAt(pX, pY)) panel->beginDrag(pX, pY);
		const auto divider = getDividerAt(pX, pY);
		if (!divider) return;
		const float x = static_cast<float>(pX / allocationSelf.get_width());
		const float y = static_cast<float>(pY / allocationSelf.get_height());
		divider->beginDrag(x, y);
		draggingDivider = divider;
	});
	drag->signal_drag_update().connect([this, drag](const double pX, const double pY) {
		double startX;
		double startY;
		drag->get_start_point(startX, startY);
		if (const auto panel = getPanelAt(startX + pX, startY + pY)) panel->updateDrag(pX, pY);
		if (!draggingDivider) return;
		const float x = static_cast<float>(pX / allocationSelf.get_width());
		const float y = static_cast<float>(pY / allocationSelf.get_height());
		draggingDivider->updateDrag(x, y);
		queue_allocate();
	});
	drag->signal_drag_end().connect([this](double /*pX*/, double /*pY*/) { draggingDivider = nullptr; });
	add_controller(drag);
}

MultiPaned::~MultiPaned() {
	if (!gobj()) return;

	while (Widget* child = get_first_child()) child->unparent();
}

void MultiPaned::onContainerDestroy() {
	while (Widget* child = get_first_child()) child->unparent();
}

void MultiPaned::measure_vfunc(const Gtk::Orientation pOrientation, const int pForSize, int &pMinimum, int &pNatural,
							   int &pMinimumBaseline, int &pNaturalBaseline) const {
	pMinimumBaseline = -1;
	pNaturalBaseline = -1;

	pMinimum = 0;
	pNatural = 0;


	for (const Widget* child = get_first_child(); child; child = child->get_next_sibling()) {
		int childMinimum, childNatural, ignore;
		child->measure(pOrientation, pForSize, childMinimum, childNatural, ignore, ignore);
		pMinimum = std::max(pMinimum, childMinimum);
		pNatural = std::max(pNatural, childNatural);
	}
}

void MultiPaned::size_allocate_vfunc(const int pWidth, const int pHeight, const int pBaseline) {
	const float widthSelf = static_cast<float>(pWidth);
	const float heightSelf = static_cast<float>(pHeight);

	for (const auto &panel: panels) {
		Gtk::Allocation allocation;

		allocation.set_x(static_cast<int>(panel->getPosX() * widthSelf) + 2);
		allocation.set_y(static_cast<int>(panel->getPosY() * heightSelf) + 2);
		allocation.set_width(static_cast<int>(panel->getWidth() * widthSelf) - 4);
		allocation.set_height(static_cast<int>(panel->getHeight() * heightSelf) - 4);
		panel->size_allocate(allocation, pBaseline);
	}

	for (const auto &divider: dividers) {
		Gtk::Allocation allocation;
		allocation.set_x(static_cast<int>(divider->getPosX() * widthSelf) - 2);
		allocation.set_y(static_cast<int>(divider->getPosY() * heightSelf) - 2);
		const auto len = divider->getLength();
		if (divider->getOrientation() == Gtk::Orientation::HORIZONTAL) {
			allocation.set_width(static_cast<int>(len * widthSelf));
			allocation.set_height(5);
		} else {
			allocation.set_width(5);
			allocation.set_height(static_cast<int>(len * heightSelf));
		}
		divider->size_allocate(allocation, pBaseline);
	}
	allocationSelf = get_allocation();
}

Gtk::Widget* MultiPaned::setRoot(const std::shared_ptr<mvp::IPresenter> &pPresenter) {
	auto widget = createWidget(pPresenter);
	if (!widget) return nullptr;

	const auto panel = std::make_shared<MultiPanedPanel>(widget);
	panel->presenter = pPresenter;
	panel->name = widget->get_name();
	panels.push_back(panel);
	panel->insert_at_end(*this);
	return panel.get();
}

MultiPanedPanelDivider* MultiPaned::getDividerAt(const double pX, const double pY) {
	const auto pickedWidget = pick(pX, pY, Gtk::PickFlags::INSENSITIVE | Gtk::PickFlags::NON_TARGETABLE);
	if (!pickedWidget) return nullptr;
	const auto widget = pickedWidget->get_ancestor(MultiPanedPanelDivider::get_type());
	if (!widget) return nullptr;
	const auto divider = dynamic_cast<MultiPanedPanelDivider*>(widget);
	if (!divider) return nullptr;
	return divider;
}

MultiPanedPanel* MultiPaned::getPanelAt(const double pX, const double pY) {
	const auto pickedWidget = pick(pX, pY, Gtk::PickFlags::INSENSITIVE | Gtk::PickFlags::NON_TARGETABLE);
	if (!pickedWidget) return nullptr;
	const auto widget = pickedWidget->get_ancestor(MultiPanedPanel::get_type());
	if (!widget) return nullptr;
	const auto panel = dynamic_cast<MultiPanedPanel*>(widget);
	if (!panel) return nullptr;
	return panel;
}

Gtk::Widget* MultiPaned::createWidget(const std::shared_ptr<mvp::IPresenter> &pPresenter) {
	const std::shared_ptr<mvp::MultiPanedContext> context = mvp::MultiPanedContext::create(this);
	const auto view = createWidgetSignal(pPresenter.get(), context);
	if (!view) return nullptr;
	pPresenter->addView(view);
	return context->getWidget();
}

std::pair<std::shared_ptr<MultiPanedPanelDivider>, MultiPanedPanel*> MultiPaned::splitAtImpl(
	MultiPanedPanel* pParentWidget, const std::shared_ptr<mvp::IPresenter> &pPresenter,
	const Gtk::Orientation pOrientation, const float pTargetChildSize) {

	auto widget = createWidget(pPresenter);
	if (!widget) return {};

	const auto panel = std::make_shared<MultiPanedPanel>(widget);
	panel->presenter = pPresenter;
	panel->name = widget->get_name();
	auto divider = pParentWidget->splitAt(panel.get(), pTargetChildSize, pOrientation);
	dividers.push_back(divider);
	divider->insert_at_end(*this);
	panels.push_back(panel);
	panel->insert_at_end(*this);

	return std::make_pair(divider, panel.get());
}


} // namespace mer::editor::ui