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

#ifndef MULTIPANEDPANEL_H
#define MULTIPANEDPANEL_H

namespace mer::editor::mvp {
class IPresenter;
}

namespace mer::editor::ui {
class MultiPanedPanel;
enum class MultiPanedSide;

class MultiPanedPanelDivider : public Gtk::Widget {
	Gtk::Orientation orientation = Gtk::Orientation::VERTICAL;
	std::map<MultiPanedSide, std::list<MultiPanedPanel*>> neighbors{};
	float posX{};
	float posY{};
	float length{};
	float draggingPos{};
	static inline GType gtype;

public:
	MultiPanedPanelDivider();

	// ReSharper disable once CppInconsistentNaming
	static GType get_type() G_GNUC_CONST;

	void beginDrag(float pX, float pY);

	void updateDrag(float pX, float pY);

	void addPanel(const MultiPanedSide pSide, MultiPanedPanel* pPanel) { neighbors[pSide].push_back(pPanel); }

	void removePanel(const MultiPanedSide pSide, MultiPanedPanel* pPanel) { neighbors[pSide].remove(pPanel); }

	[[nodiscard]] Gtk::Orientation getOrientation() const { return orientation; }

	void setOrientation(const Gtk::Orientation pOrientation) {
		orientation = pOrientation;
		if (orientation == Gtk::Orientation::HORIZONTAL) {
			set_cursor("sb_v_double_arrow");
		} else
			set_cursor("sb_h_double_arrow");
	}

	[[nodiscard]] float getPosX() const { return posX; }

	void setPosX(const float pPosX) { posX = pPosX; }

	[[nodiscard]] float getPosY() const { return posY; }

	void setPosY(const float pPosY) { posY = pPosY; }

	[[nodiscard]] float getLength() const { return length; }

	void setLength(const float pLength) { length = pLength; }

protected:
	Gtk::SizeRequestMode get_request_mode_vfunc() const override { return Gtk::SizeRequestMode::HEIGHT_FOR_WIDTH; }

	void size_allocate_vfunc(int pWidth, int pHeight, int pBaseline) override;

	void measure_vfunc(Gtk::Orientation pOrientation, int pForSize, int &pMinimum, int &pNatural, int &pMinimumBaseline,
					   int &pNaturalBaseline) const override;
};

class MultiPanedPanel : public Gtk::Widget {
	static inline GType gtype;
	enum class Corner;
	std::string name;
	friend class MultiPaned;
	Widget* widget{};
	std::map<MultiPanedSide, MultiPanedPanelDivider*> dividers{};
	Corner cloningCorner{};
	bool cloningInProgress{};
	std::shared_ptr<mvp::IPresenter> presenter;

	MultiPanedPanel() : ObjectBase("MultiPanedPanelDivider") {}

public:
	explicit MultiPanedPanel(Gtk::Widget* pWidget);
	// ReSharper disable once CppInconsistentNaming
	static GType get_type() G_GNUC_CONST;

	void beginDrag(double pX, double pY);

	void updateDrag(double pX, double pY);

	[[nodiscard]] std::map<MultiPanedSide, MultiPanedPanelDivider*> &getDividers() { return dividers; }

	MultiPanedPanelDivider* getDivider(const MultiPanedSide pSide) { return dividers[pSide]; }

	const MultiPanedPanelDivider* getDivider(const MultiPanedSide pSide) const { return dividers.at(pSide); }

	void setDivider(const MultiPanedSide pSide, MultiPanedPanelDivider* pDivider) {
		const auto oppositeSide = getOppositeSide(pSide);
		auto &oldDivider = dividers[pSide];
		if (oldDivider) { oldDivider->removePanel(oppositeSide, this); }
		oldDivider = pDivider;
		pDivider->addPanel(oppositeSide, this);
	}

	std::shared_ptr<MultiPanedPanelDivider> splitAt(MultiPanedPanel* pChildPanel, float pTargetChildSize,
													Gtk::Orientation pOrientation);

	std::string getName() const {
		if (!widget) return "";
		return widget->get_name().raw();
	}

	[[nodiscard]] float getWidth() const;

	[[nodiscard]] float getHeight() const;

	[[nodiscard]] float getPosX() const;

	[[nodiscard]] float getPosY() const;

	[[nodiscard]] float getEndPosX() const;

	[[nodiscard]] float getEndPosY() const;

private:
	static MultiPanedSide getOppositeSide(MultiPanedSide pSide);

protected:
	Gtk::SizeRequestMode get_request_mode_vfunc() const override {
		if (widget) return widget->get_request_mode();
		return Gtk::SizeRequestMode::HEIGHT_FOR_WIDTH;
	}

	void size_allocate_vfunc(int pWidth, int pHeight, int pBaseline) override;

	void measure_vfunc(Gtk::Orientation pOrientation, int pForSize, int &pMinimum, int &pNatural, int &pMinimumBaseline,
					   int &pNaturalBaseline) const override;
};
} // namespace mer::editor::ui

#endif //MULTiPANEDPANEL_H
