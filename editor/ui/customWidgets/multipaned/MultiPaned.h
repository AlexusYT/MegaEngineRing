//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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
#include <nlohmann/json_fwd.hpp>

#include "EngineUtils/utils/UUID.h"
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
	friend MultiPanedPanel;
	friend MultiPanedPanelDivider;
	std::unordered_map<UUID, std::shared_ptr<MultiPanedPanelDivider>> dividers;
	std::unordered_map<UUID, std::shared_ptr<MultiPanedPanel>> panels;
	sigc::slot<void(const Widget* pParentWidget, MultiPanedPanel* pPanel)> createWidgetSlot;
	MultiPanedPanelDivider* draggingDivider{};

	sigc::signal<std::shared_ptr<mvp::IView>(const mvp::IPresenter* pPresenter,
											 const std::shared_ptr<mvp::MultiPanedContext> &pContext)>
		createWidgetSignal;

public:
	MultiPaned();

	~MultiPaned() override;

	Widget* splitAt(Widget* pParentWidget, const std::shared_ptr<mvp::IPresenter> &pPresenter,
					Gtk::Orientation pOrientation, float pTargetChildSize = 0.5f);

	Widget* setRoot(const std::shared_ptr<mvp::IPresenter> &pPresenter);

	void remove(Widget* pWidget);

	sigc::connection connectCreateWidgetSignal(
		const sigc::slot<std::shared_ptr<mvp::IView>(const mvp::IPresenter* pPresenter,
													 const std::shared_ptr<mvp::MultiPanedContext> &pContext)> &pSlot) {
		return createWidgetSignal.connect(pSlot);
	}

	sdk::ReportMessagePtr importFromJson(const std::shared_ptr<nlohmann::json> &pJson,
												mvp::ApplicationController* pAppController);

	std::shared_ptr<nlohmann::json> exportToJson() const;

private:
	void onDividerRemoved(const MultiPanedPanelDivider* pDivider);

	void onContainerDestroy();

	Gtk::SizeRequestMode get_request_mode_vfunc() const override { return Gtk::SizeRequestMode::HEIGHT_FOR_WIDTH; }

	void measure_vfunc(Gtk::Orientation pOrientation, int pForSize, int &pMinimum, int &pNatural, int &pMinimumBaseline,
					   int &pNaturalBaseline) const override;

	void size_allocate_vfunc(int pWidth, int pHeight, int pBaseline) override;

	MultiPanedPanelDivider* getDividerAt(double pX, double pY);

	MultiPanedPanel* getPanelAt(double pX, double pY);

	std::shared_ptr<MultiPanedPanel> createPanel(const std::shared_ptr<mvp::IPresenter> &pPresenter);

	std::pair<std::shared_ptr<MultiPanedPanelDivider>, MultiPanedPanel*> splitAtImpl(
		MultiPanedPanel* pParentWidget, const std::shared_ptr<mvp::IPresenter> &pPresenter,
		Gtk::Orientation pOrientation, float pTargetChildSize);

	void setDraggingDivider(MultiPanedPanelDivider* pDraggingDivider) { draggingDivider = pDraggingDivider; }
};
} // namespace mer::editor::ui


#endif //MULTIPANED_H
