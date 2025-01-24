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

#include "MultiPaned.h"

#include <nlohmann/json.hpp>

#include "EngineUtils/utils/UUID.h"
#include "MultiPanedPanel.h"
#include "mvp/ApplicationController.h"
#include "mvp/contexts/MultiPanedContext.h"

namespace mer::editor::ui {


MultiPaned::MultiPaned() {
	signal_destroy().connect(mem_fun(*this, &MultiPaned::onContainerDestroy));

	auto drag = Gtk::GestureDrag::create();
	drag->signal_drag_begin().connect([this](const double pX, const double pY) {
		if (const auto panel = getPanelAt(pX, pY)) panel->beginDrag(pX, pY);
		const auto divider = getDividerAt(pX, pY);
		if (!divider) return;
		const float x = static_cast<float>(pX / get_width());
		const float y = static_cast<float>(pY / get_height());
		divider->beginDrag(x, y);
		draggingDivider = divider;
	});
	drag->signal_drag_update().connect([this, drag](const double pX, const double pY) {
		double startX;
		double startY;
		drag->get_start_point(startX, startY);
		if (const auto panel = getPanelAt(startX + pX, startY + pY)) panel->updateDrag(pX, pY);
		if (!draggingDivider) return;
		const float x = static_cast<float>(pX / get_width());
		const float y = static_cast<float>(pY / get_height());
		draggingDivider->updateDrag(x, y);
		queue_allocate();
	});
	drag->signal_drag_end().connect([this](double /*pX*/, double /*pY*/) { draggingDivider = nullptr; });
	add_controller(drag);
}

MultiPaned::~MultiPaned() {
	panels.clear();
	//if (!gobj()) return;

	//while (Widget* child = get_first_child()) child->unparent();
}

Gtk::Widget* MultiPaned::splitAt(Widget* pParentWidget, const std::shared_ptr<mvp::IPresenter> &pPresenter,
								 const Gtk::Orientation pOrientation, const float pTargetChildSize) {
	if (const auto widget = dynamic_cast<MultiPanedPanel*>(pParentWidget))
		return splitAtImpl(widget, pPresenter, pOrientation, pTargetChildSize).second;
	return nullptr;
}

Gtk::Widget* MultiPaned::setRoot(const std::shared_ptr<mvp::IPresenter> &pPresenter) {
	const auto panel = createPanel(pPresenter);
	if (!panel) return nullptr;
	panels.emplace(*panel->getUuid(), panel);
	panel->insert_at_end(*this);
	return panel.get();
}

void MultiPaned::remove(Widget* pWidget) {
	if (panels.size() <= 1) return;
	auto panel = dynamic_cast<MultiPanedPanel*>(pWidget);
	if (!panel)
		if (const auto panelWidget = dynamic_cast<MultiPanedPanel*>(pWidget->get_parent())) { panel = panelWidget; }
	if (panel) panels.erase(*panel->getUuid());
}

sdk::ReportMessagePtr MultiPaned::importFromJson(const std::shared_ptr<nlohmann::json> &pJson,
														mvp::ApplicationController* pAppController) {

	try {
		std::unordered_map<UUID, std::shared_ptr<MultiPanedPanelDivider>> newDividers;
		for (auto jsonDivider: pJson->at("Dividers")) {
			auto divider = std::make_shared<MultiPanedPanelDivider>();
			sdk::ReportMessagePtr uuidError;
			divider->uuid = UUID::parse(jsonDivider.at("Uuid").get<std::string>(), uuidError);
			if (uuidError) {
				uuidError->setTitle("Failed to parse UUID for divider while importing the layout");
				return uuidError;
			}
			divider->setPos(jsonDivider.at("Pos").get<float>());
			divider->setOrientation(jsonDivider.at("Orientation").get<std::string>() == "Vertical"
										? Gtk::Orientation::VERTICAL
										: Gtk::Orientation::HORIZONTAL);
			newDividers.emplace(*divider->getUuid(), divider);
		}
		std::unordered_map<UUID, std::shared_ptr<MultiPanedPanel>> newPanels;
		for (auto jsonPanel: pJson->at("Panels")) {
			std::string presenterName = jsonPanel.at("Presenter").get<std::string>();
			auto presenter = pAppController->getPresenterByName(presenterName);

			//TODO Use default presenter for this case.
			if (!presenter) {
				auto msg = sdk::ReportMessage::create();
				msg->setTitle("Error while importing the layout");
				msg->setMessage("Presenter with such name does not exist");
				msg->addInfoLine("Missing presenter name: {}", presenterName);
				sdk::Logger::error(msg);
				continue;
			}

			//TODO Use default presenter for this case.
			auto panel = createPanel(presenter);
			if (!panel) {
				auto msg = sdk::ReportMessage::create();
				msg->setTitle("Error while importing the layout");
				msg->setMessage("Presenter failed to create a widget or view");
				msg->addInfoLine("Presenter name: {}", presenterName);
				sdk::Logger::error(msg);
				continue;
			}
			panel->presenter = presenter;
			std::string uuid = jsonPanel.at("Uuid").get<std::string>();
			sdk::ReportMessagePtr uuidError;
			panel->uuid = UUID::parse(uuid, uuidError);
			//TODO Use default presenter for this case.
			if (uuidError) {
				uuidError->setTitle("Failed to parse the UUID for panel while importing the layout");
				uuidError->addInfoLine("Presenter name: {}", presenterName);
				sdk::Logger::error(uuidError);
				continue;
			}
			auto sides = {std::make_pair("Top", MultiPanedSide::TOP), std::make_pair("Bottom", MultiPanedSide::BOTTOM),
						  std::make_pair("Left", MultiPanedSide::LEFT), std::make_pair("Right", MultiPanedSide::RIGHT)};

			auto dividersJson = jsonPanel.at("Dividers");
			for (auto [name, side]: sides) {
				if (auto dividerUuid = dividersJson.at(name).get<std::string>(); dividerUuid != "null") {
					sdk::ReportMessagePtr dividerUuidError;
					auto divUuid = UUID::parse(dividerUuid, dividerUuidError);
					if (dividerUuidError) {
						dividerUuidError->setTitle("Failed to parse the UUID of the divider for panel");
						dividerUuidError->addInfoLine("Presenter name: {}", presenterName);
						dividerUuidError->addInfoLine("Divider location: {}", name);
						sdk::Logger::error(dividerUuidError);
						return dividerUuidError;
					}
					panel->setDivider(side, newDividers.at(*divUuid).get());
				}
			}
			newPanels.emplace(*panel->getUuid(), panel);
		}

		panels.swap(newPanels);
		newPanels.clear();
		dividers.swap(newDividers);
		newDividers.clear();

		for (auto panel: panels | std::views::values) { panel->set_parent(*this); }
		for (auto divider: dividers | std::views::values) { divider->set_parent(*this); }
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to import layout");
		msg->setMessage("Exception occurred");
		return msg;
	}

	return nullptr;
}

std::shared_ptr<nlohmann::json> MultiPaned::exportToJson() const {
	auto json = std::make_shared<nlohmann::json>();
	nlohmann::json panelsDataArray;
	for (const auto &[uuid, panel]: panels) {
		nlohmann::json panelDataObject;
		panelDataObject.emplace("Uuid", uuid.toString());
		panelDataObject.emplace("Presenter", panel->presenter->getTypeName());

		nlohmann::json dividersObject;
		const auto leftDivider = panel->getDivider(MultiPanedSide::LEFT);
		dividersObject.emplace("Left", leftDivider ? leftDivider->getUuid()->toString() : "null");
		const auto rightDivider = panel->getDivider(MultiPanedSide::RIGHT);
		dividersObject.emplace("Right", rightDivider ? rightDivider->getUuid()->toString() : "null");
		const auto topDivider = panel->getDivider(MultiPanedSide::TOP);
		dividersObject.emplace("Top", topDivider ? topDivider->getUuid()->toString() : "null");
		const auto bottomDivider = panel->getDivider(MultiPanedSide::BOTTOM);
		dividersObject.emplace("Bottom", bottomDivider ? bottomDivider->getUuid()->toString() : "null");

		panelDataObject.emplace("Dividers", dividersObject);
		panelsDataArray.emplace_back(panelDataObject);
	}
	json->emplace("Panels", panelsDataArray);
	nlohmann::json dividersDataArray;
	for (const auto &[uuid, divider]: dividers) {
		nlohmann::json dividerDataObject;
		dividerDataObject.emplace("Uuid", uuid.toString());
		dividerDataObject.emplace("Pos", divider->pos);
		dividerDataObject.emplace("Orientation",
								  divider->orientation == Gtk::Orientation::VERTICAL ? "Vertical" : "Horizontal");
		dividersDataArray.emplace_back(dividerDataObject);
	}
	json->emplace("Dividers", dividersDataArray);
	return json;
}

void MultiPaned::onDividerRemoved(const MultiPanedPanelDivider* pDivider) { dividers.erase(*pDivider->getUuid()); }

void MultiPaned::onContainerDestroy() { panels.clear(); }

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

	for (const auto &panel: panels | std::views::values) {
		Gtk::Allocation allocation;

		allocation.set_x(static_cast<int>(panel->getPosX() * widthSelf) + 2);
		allocation.set_y(static_cast<int>(panel->getPosY() * heightSelf) + 2);
		allocation.set_width(static_cast<int>(panel->getWidth() * widthSelf) - 4);
		allocation.set_height(static_cast<int>(panel->getHeight() * heightSelf) - 4);
		panel->size_allocate(allocation, pBaseline);
	}

	for (const auto &divider: dividers | std::views::values) {
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

std::shared_ptr<MultiPanedPanel> MultiPaned::createPanel(const std::shared_ptr<mvp::IPresenter> &pPresenter) {
	const std::shared_ptr<mvp::MultiPanedContext> context = mvp::MultiPanedContext::create(this);
	const auto view = createWidgetSignal(pPresenter.get(), context);
	if (!view) return nullptr;
	pPresenter->addView(view);

	auto widget = context->getWidget();
	if (!widget) {
		pPresenter->removeView(view);
		return nullptr;
	}

	const auto panel = std::make_shared<MultiPanedPanel>(widget);
	panel->view = view;
	panel->presenter = pPresenter;
	panel->name = widget->get_name();
	return panel;
}

std::pair<std::shared_ptr<MultiPanedPanelDivider>, MultiPanedPanel*> MultiPaned::splitAtImpl(
	MultiPanedPanel* pParentWidget, const std::shared_ptr<mvp::IPresenter> &pPresenter,
	const Gtk::Orientation pOrientation, const float pTargetChildSize) {

	const auto panel = createPanel(pPresenter);
	if (!panel) return {};
	panels.emplace(*panel->getUuid(), panel);
	panel->insert_at_end(*this);

	auto divider = pParentWidget->splitAt(panel.get(), pTargetChildSize, pOrientation);
	dividers.emplace(*divider->getUuid(), divider);
	divider->insert_at_end(*this);

	return std::make_pair(divider, panel.get());
}


} // namespace mer::editor::ui