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
// Created by alexus on 20.09.24.
//

#include "MultiPanedPanel.h"

#include "MultiPaned.h"
#include "MultiPanedSide.h"

namespace mer::editor::ui {
MultiPanedPanelDivider::MultiPanedPanelDivider() : ObjectBase("MultiPanedPanelDivider") {
	neighbors.emplace(MultiPanedSide::RIGHT, std::list<MultiPanedPanel*>());
	neighbors.emplace(MultiPanedSide::BOTTOM, std::list<MultiPanedPanel*>());
	neighbors.emplace(MultiPanedSide::LEFT, std::list<MultiPanedPanel*>());
	neighbors.emplace(MultiPanedSide::TOP, std::list<MultiPanedPanel*>());
}

// ReSharper disable once CppInconsistentNaming
GType MultiPanedPanelDivider::get_type() {
	if (gtype) return gtype;
	MultiPanedPanelDivider dummy;
	const auto widget = GTK_WIDGET(dummy.gobj());
	gtype = G_OBJECT_TYPE(widget);
	return gtype;
}

void MultiPanedPanelDivider::beginDrag(const float /*pX*/, const float /*pY*/) {
	if (orientation == Gtk::Orientation::VERTICAL) {
		draggingPos = getPosX();
	} else {
		draggingPos = getPosY();
	}
}

void MultiPanedPanelDivider::updateDrag(const float pX, const float pY) {
	if (orientation == Gtk::Orientation::VERTICAL) {
		setPosX(draggingPos + pX);
	} else {
		setPosY(draggingPos + pY);
	}
}

void MultiPanedPanelDivider::size_allocate_vfunc(int /*pWidth*/, int /*pHeight*/, int /*pBaseline*/) {}

void MultiPanedPanelDivider::measure_vfunc(Gtk::Orientation /*pOrientation*/, int /*pForSize*/, int &pMinimum,
										   int &pNatural, int &pMinimumBaseline, int &pNaturalBaseline) const {
	pMinimumBaseline = -1;
	pNaturalBaseline = -1;

	pMinimum = 3;
	pNatural = 12;
}

bool between(const float pVal, const float p1, const float p2) {
	//
	return pVal > p1 && pVal <= p2;
}
enum class MultiPanedPanel::Corner { NONE = 0, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

MultiPanedPanel::MultiPanedPanel(Widget* pWidget)
	: ObjectBase("MultiPanedPanel"), widget(pWidget), cloningCorner(Corner::NONE) {
	if (!pWidget) return;
	dividers.emplace(MultiPanedSide::RIGHT, nullptr);
	dividers.emplace(MultiPanedSide::BOTTOM, nullptr);
	dividers.emplace(MultiPanedSide::LEFT, nullptr);
	dividers.emplace(MultiPanedSide::TOP, nullptr);
	widget->set_parent(*this);

	const auto motion = Gtk::EventControllerMotion::create();
	motion->signal_motion().connect([this](const double pX, const double pY) {
		const auto allocation = get_allocation();
		const auto width = static_cast<float>(allocation.get_width());
		const auto height = static_cast<float>(allocation.get_height());
		const float x = static_cast<float>(pX);
		const float y = static_cast<float>(pY);

		bool changeCursor{};
		if (constexpr float delta = 20.0f; between(x, 0, delta) && between(y, 0, delta)) {
			cloningCorner = Corner::TOP_LEFT;
			changeCursor = true;
		} else if (between(x, width - delta, width) && between(y, 0, delta)) {
			cloningCorner = Corner::TOP_RIGHT;
			changeCursor = true;
		} else if (between(x, 0, delta) && between(y, height - delta, height)) {
			cloningCorner = Corner::BOTTOM_LEFT;
			changeCursor = true;
		} else if (between(x, width - delta, width) && between(y, height - delta, height)) {
			cloningCorner = Corner::BOTTOM_RIGHT;
			changeCursor = true;
		}
		if (changeCursor) set_cursor("crosshair");
		else
			set_cursor();
	});
	add_controller(motion);
}

// ReSharper disable once CppInconsistentNaming
GType MultiPanedPanel::get_type() {
	if (gtype) return gtype;
	MultiPanedPanel dummy(nullptr);
	const auto widget = GTK_WIDGET(dummy.gobj());
	gtype = G_OBJECT_TYPE(widget);
	return gtype;
}

void MultiPanedPanel::beginDrag(double /*pX*/, double /* pY*/) {
	if (cloningCorner != Corner::NONE) cloningInProgress = true;
}

void MultiPanedPanel::updateDrag(const double pX, const double pY) {
	if (!cloningInProgress) return;
	Gtk::Orientation orientation;
	bool horizontal;
	if (abs(pX) > 20) {
		horizontal = true;
		orientation = Gtk::Orientation::VERTICAL;
	} else if (abs(pY) > 20) {
		horizontal = false;
		orientation = Gtk::Orientation::HORIZONTAL;
	} else
		return;
	const auto multiPaned = dynamic_cast<MultiPaned*>(get_parent());
	if (!multiPaned) return;
	float size{};
	if (cloningCorner == Corner::BOTTOM_RIGHT) {
		size = 1.0f;
	} else if (cloningCorner == Corner::TOP_RIGHT) {
		size = horizontal ? 1.0f : 0.0f;
	} else if (cloningCorner == Corner::BOTTOM_LEFT) {
		size = horizontal ? 0.0f : 1.0f;
	}
	const std::shared_ptr<MultiPanedPanelDivider> divider =
		multiPaned->splitAtImpl(this, presenter, orientation, size).first;

	if (divider) {
		divider->beginDrag(0.0f, 0.0f);
		multiPaned->setDraggingDivider(divider.get());
	}
	cloningInProgress = false;
	cloningCorner = Corner::NONE;
}

std::shared_ptr<MultiPanedPanelDivider> MultiPanedPanel::splitAt(
	MultiPanedPanel* pChildPanel, const float pTargetChildSize, const Gtk::Orientation pOrientation) {
	auto divider = std::make_shared<MultiPanedPanelDivider>();
	divider->setOrientation(pOrientation);
	if (pOrientation == Gtk::Orientation::VERTICAL) {
		divider->setPosY(this->getPosY());
		divider->setLength(this->getHeight());
		if (const auto parentLeftDivider = this->getDivider(MultiPanedSide::LEFT)) {
			divider->setPosX(parentLeftDivider->getPosX() + this->getWidth() * pTargetChildSize);
		} else
			divider->setPosX(this->getWidth() * pTargetChildSize);
		pChildPanel->setDivider(MultiPanedSide::LEFT, divider.get());
		if (const auto parentDivider = this->getDivider(MultiPanedSide::BOTTOM)) {
			pChildPanel->setDivider(MultiPanedSide::BOTTOM, parentDivider);
		}
		if (const auto parentDivider = this->getDivider(MultiPanedSide::RIGHT)) {
			pChildPanel->setDivider(MultiPanedSide::RIGHT, parentDivider);
		}
		if (const auto parentDivider = this->getDivider(MultiPanedSide::TOP)) {
			pChildPanel->setDivider(MultiPanedSide::TOP, parentDivider);
		}
		this->setDivider(MultiPanedSide::RIGHT, divider.get());

	} else {
		divider->setPosX(this->getPosX());
		divider->setLength(this->getWidth());

		if (const auto parentDivider = this->getDivider(MultiPanedSide::TOP)) {
			divider->setPosY(parentDivider->getPosY() + this->getHeight() * pTargetChildSize);
		} else
			divider->setPosY(this->getHeight() * pTargetChildSize);
		pChildPanel->setDivider(MultiPanedSide::TOP, divider.get());
		if (const auto parentDivider = this->getDivider(MultiPanedSide::BOTTOM)) {
			pChildPanel->setDivider(MultiPanedSide::BOTTOM, parentDivider);
		}
		if (const auto parentDivider = this->getDivider(MultiPanedSide::LEFT)) {
			pChildPanel->setDivider(MultiPanedSide::LEFT, parentDivider);
		}
		if (const auto parentDivider = this->getDivider(MultiPanedSide::RIGHT)) {
			pChildPanel->setDivider(MultiPanedSide::RIGHT, parentDivider);
		}
		this->setDivider(MultiPanedSide::BOTTOM, divider.get());
	}


	return divider;
}

float MultiPanedPanel::getWidth() const { return getEndPosX() - getPosX(); }

float MultiPanedPanel::getHeight() const { return getEndPosY() - getPosY(); }

float MultiPanedPanel::getPosX() const {
	const auto leftDivider = getDivider(MultiPanedSide::LEFT);
	return leftDivider ? leftDivider->getPosX() : 0;
}

float MultiPanedPanel::getPosY() const {
	const auto topDivider = getDivider(MultiPanedSide::TOP);
	return topDivider ? topDivider->getPosY() : 0;
}

float MultiPanedPanel::getEndPosX() const {
	const auto rightDivider = getDivider(MultiPanedSide::RIGHT);
	return rightDivider ? rightDivider->getPosX() : 1;
}

float MultiPanedPanel::getEndPosY() const {
	const auto bottomDivider = getDivider(MultiPanedSide::BOTTOM);
	return bottomDivider ? bottomDivider->getPosY() : 1;
}

MultiPanedSide MultiPanedPanel::getOppositeSide(const MultiPanedSide pSide) {
	switch (pSide) {
		case MultiPanedSide::LEFT: return MultiPanedSide::RIGHT;
		case MultiPanedSide::RIGHT: return MultiPanedSide::LEFT;
		case MultiPanedSide::TOP: return MultiPanedSide::BOTTOM;
		case MultiPanedSide::BOTTOM: return MultiPanedSide::TOP;
		default: return MultiPanedSide::NONE;
	}
}

void MultiPanedPanel::size_allocate_vfunc(const int pWidth, const int pHeight, const int pBaseline) {

	Gtk::Allocation allocation;
	allocation.set_width(pWidth);
	allocation.set_height(pHeight);
	widget->size_allocate(allocation, pBaseline);
}

void MultiPanedPanel::measure_vfunc(const Gtk::Orientation pOrientation, const int pForSize, int &pMinimum,
									int &pNatural, int &pMinimumBaseline, int &pNaturalBaseline) const {
	widget->measure(pOrientation, pForSize, pMinimum, pNatural, pMinimumBaseline, pNaturalBaseline);
}

} // namespace mer::editor::ui