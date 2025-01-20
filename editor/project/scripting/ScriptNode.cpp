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
// Created by alexus on 25.06.24.
//

#include "ScriptNode.h"

#include "EngineSDK/extensions/Extension.h"
#include "mvp/main/editors/graphicsScriptEditor/canvas/CanvasWidget.h"

namespace mer::editor::project {
sdk::utils::ReportMessagePtr ScriptNode::connectToSlot(const std::string &pSelfSlotName, ScriptNodeSlot* pOtherSlot) {
	auto selfSlot = getSlotByName(pSelfSlotName);
	if (!selfSlot) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Slot connection failed");
		msg->setMessage("No slot with such name");
		msg->addInfoLine("Slot name: {}", pSelfSlotName);
		return msg;
	}
	if (!pOtherSlot) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Slot connection failed");
		msg->setMessage("Pointer to other slot is nullptr");
		msg->addInfoLine("Slot name: {}", pSelfSlotName);
		return msg;
	}

	selfSlot->connect(pOtherSlot);
	pOtherSlot->connect(selfSlot);
	auto connection = std::make_shared<ScriptNodeSlotConnection>(selfSlot, pOtherSlot);
	selfSlot->ownedConnection = connection;
	pOtherSlot->usedConnection = connection.get();
	return nullptr;
}

void ScriptNode::removeAllConnections() const {
	for (auto slot: slots) { slot.second->disconnect(); }
}

ScriptNodeSlot* ScriptNode::getSlotByName(const std::string &pSelfSlotName) {
	auto iter = slots.find(pSelfSlotName);
	if (iter == slots.end()) { return nullptr; }

	return iter->second.get();
}

sdk::utils::ReportMessagePtr ScriptNode::addNewSlot(const std::string &pSlotName, const std::string &pMethodName,
													SlotConnectionType pType) {
	auto iter = slots.find(pSlotName);
	if (iter != slots.end()) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to add new slot");
		msg->setMessage("Slot with such name already added to node");
		msg->addInfoLine("Slot name: {}", pSlotName);
		return msg;
	}
	auto slot = std::make_shared<ScriptNodeSlot>();
	slot->setParent(this);
	slot->setName(pSlotName);
	slot->setMethodName(pMethodName);
	slot->setType(pType);
	if (getWidget()) slot->setWidget(getWidget());
	slots.emplace(pSlotName, slot);

	return nullptr;
}

mvp::CanvasElement* ScriptNode::getHoveredElement(const glm::dvec2 &pPos) {

	for (auto slot: slots) {
		auto newCursorPos = pPos - slot.second->getPosition();
		if (auto hoveredElement = slot.second->getHoveredElement(newCursorPos)) { return hoveredElement; }
	}
	auto sizeSelf = getSize();
	if (pPos.x > 0 && pPos.x < sizeSelf.x && pPos.y > 0 && pPos.y < sizeSelf.y) return this;

	return nullptr;
}

void ScriptNode::onDraw(const std::shared_ptr<Cairo::Context> &pContext) {
	pContext->set_font_size(20);
	glm::dvec2 sizeSelf;
	if (!hasSize) {
		calculateSize(pContext, sizeSelf);
		setSize(sizeSelf);
		hasSize = true;
	} else {
		sizeSelf = getSize();
	}

	Cairo::TextExtents extents;
	pContext->get_text_extents(name, extents);
	Cairo::FontExtents fontExtents;
	pContext->get_font_extents(fontExtents);

	pContext->set_source_rgba(0.3f, 0.3f, 0.3f, 1.0f);
	pContext->rectangle(0, 0, sizeSelf.x, fontExtents.height);
	pContext->fill();

	if (isSelected()) pContext->set_source_rgba(0.5f, 0.5f, 0.5f, 1.0f);
	else
		pContext->set_source_rgba(0.4f, 0.4f, 0.4f, 1.0f);
	pContext->rectangle(0, fontExtents.height, sizeSelf.x, sizeSelf.y - fontExtents.height);
	pContext->fill();

	pContext->move_to(5, fontExtents.ascent);
	pContext->set_source_rgba(1.0, 1.0, 1.0, 1.0);
	pContext->text_path(name);
	pContext->fill();


	//double yOffset = fontExtents.height + 15;
	for (auto slot: slots) { slot.second->draw(pContext); }
}

void ScriptNode::calculateSize(const std::shared_ptr<Cairo::Context> &pContext, glm::dvec2 &pSizeOut) {

	pContext->set_font_size(20);
	Cairo::FontExtents fontExtents;
	pContext->get_font_extents(fontExtents);
	Cairo::TextExtents extents;
	pContext->get_text_extents(name, extents);

	int settersCount = 0;
	int callbacksCount = 0;
	double yOffset = fontExtents.height + 15;
	double widthLeft = 0;
	double widthRight = 0;
	for (auto [slotName, slot]: slots) {
		slot->updateSize(pContext);
		auto slotSize = slot->getSize();
		switch (slot->getType()) {
			case SlotConnectionType::SETTER:
				settersCount++;
				widthLeft = std::max(widthLeft, slotSize.x);
				break;
			case SlotConnectionType::CALLBACK:
				callbacksCount++;
				widthRight = std::max(widthRight, slotSize.x);
				break;
		}
	}
	pSizeOut.x = glm::max(widthLeft + widthRight + 10, extents.width + 10);
	pSizeOut.y = glm::max(yOffset + (fontExtents.height + connectionSpacing.y) * settersCount,
						  yOffset + (fontExtents.height + connectionSpacing.y) * callbacksCount, extents.height);

	settersCount = 0;
	callbacksCount = 0;
	glm::dvec2 pos{0};
	for (auto [slotName, slot]: slots) {
		auto slotSize = slot->getSize();
		switch (slot->getType()) {
			case SlotConnectionType::SETTER:
				pos.x = 0;
				pos.y = yOffset + (slotSize.y + connectionSpacing.y) * settersCount;
				settersCount++;
				break;
			case SlotConnectionType::CALLBACK:
				pos.x = pSizeOut.x;
				pos.y = yOffset + (slotSize.y + connectionSpacing.y) * callbacksCount;
				callbacksCount++;
				break;
		}
		slot->setPosition(pos);
	}
}

void ScriptNode::onWidgetChanged(mvp::CanvasWidget* pWidget) {
	for (auto [slotName, slot]: slots) { slot->setWidget(pWidget); }
}

sdk::utils::ReportMessagePtr ScriptNodeSlot::connect(ScriptNodeSlot* pOtherSlot) {
	if (auto msg = checkConnectable(pOtherSlot)) { return msg; }

	connectedTo = pOtherSlot;
	pOtherSlot->connectedTo = this;

	auto connection = std::make_shared<ScriptNodeSlotConnection>(this, pOtherSlot);
	if (auto widgetSelf = getWidget()) {
		connection->setWidget(widgetSelf);
		pOtherSlot->setWidget(widgetSelf);
	}
	ownedConnection = connection;
	pOtherSlot->usedConnection = connection.get();
	return nullptr;
}

sdk::utils::ReportMessagePtr ScriptNodeSlot::exchange(ScriptNodeSlot* pNewOtherSlot) {
	if (auto msg = checkConnectable(pNewOtherSlot)) { return msg; }
	ScriptNodeSlot* prevSlot = connectedTo;
	prevSlot->connectedTo = nullptr;
	prevSlot->usedConnection = nullptr;
	prevSlot->setWidget(nullptr);
	if (auto widgetSelf = getWidget()) { pNewOtherSlot->setWidget(widgetSelf); }

	connectedTo = pNewOtherSlot;
	pNewOtherSlot->connectedTo = this;
	ownedConnection->setOther(pNewOtherSlot);
	pNewOtherSlot->usedConnection = ownedConnection.get();

	return nullptr;
}

sdk::utils::ReportMessagePtr ScriptNodeSlot::checkConnectable(ScriptNodeSlot* pOtherSlot) {

	if (type == SlotConnectionType::CALLBACK) {
		if (pOtherSlot->type == SlotConnectionType::CALLBACK) {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Slot connection failed");
			msg->setMessage("Connecting callback-slot to callback-slot is not permitted");
			msg->addInfoLine("Self slot name: {}", name);
			msg->addInfoLine("Other slot name: {}", pOtherSlot->name);
			return msg;
		}
	}
	if (type == SlotConnectionType::SETTER) {
		if (pOtherSlot->type == SlotConnectionType::SETTER) {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Slot connection failed");
			msg->setMessage("Connecting setter-slot to setter-slot is not permitted");
			msg->addInfoLine("Self slot name: {}", name);
			msg->addInfoLine("Other slot name: {}", pOtherSlot->name);
			return msg;
		}
	}
	return nullptr;
}

void ScriptNodeSlot::disconnect() {
	if (!ownedConnection && !usedConnection) return;
	if (ownedConnection) {
		auto otherSlot = ownedConnection->getOther();
		otherSlot->usedConnection = nullptr;
		otherSlot->connectedTo = nullptr;
		connectedTo = nullptr;
		ownedConnection.reset();
	} else if (usedConnection) {
		usedConnection->getSelf()->disconnect();
	}
}

mvp::CanvasElement* ScriptNodeSlot::getHoveredElement(const glm::dvec2 &pPos) {
	if (ownedConnection)
		if (const auto hoveredElement = ownedConnection->getHoveredElement(this, pPos)) return hoveredElement;
	if (const auto tmp = pPos * pPos; tmp.x + tmp.y - connectionRadius * connectionRadius < 1) return this;
	return nullptr;
}

void ScriptNodeSlot::onDraw(const std::shared_ptr<Cairo::Context> &pContext) {
	Cairo::FontExtents fontExtents;
	pContext->get_font_extents(fontExtents);
	glm::dvec2 textPos{};
	Cairo::TextExtents slotNameExtents;
	pContext->get_text_extents(getName(), slotNameExtents);
	switch (getType()) {

		case SlotConnectionType::SETTER:
			pContext->set_source_rgba(0.2f, 0.8f, 0.2f, 1.0f);
			textPos.x = connectionRadius * 2;
			textPos.y = fontExtents.descent;
			break;
		case SlotConnectionType::CALLBACK:
			pContext->set_source_rgba(0.2f, 0.2f, 1.0f, 1.0f);
			textPos.x = -connectionRadius * 2 - slotNameExtents.width;
			textPos.y = fontExtents.descent;
			break;
	}
	pContext->arc(0, 0, connectionRadius, 0, 2 * M_PI);
	if (getConnectedTo()) pContext->fill();
	else
		pContext->stroke();
	pContext->move_to(textPos.x, textPos.y);

	pContext->set_source_rgba(1.0f, 1.0f, 1.0f, 1.0f);
	pContext->text_path(getName());
	pContext->fill();
	if (ownedConnection) {
		auto first = ownedConnection->getSelf();
		auto second = ownedConnection->getOther();
		if (first && first->isHasSize() && second && second->isHasSize()) { ownedConnection->draw(pContext); }
	}
}

void ScriptNodeSlot::calculateSize(const std::shared_ptr<Cairo::Context> &pContext, glm::dvec2 &pSizeOut) {
	Cairo::TextExtents slotNameExtents;
	pContext->get_text_extents(this->getName(), slotNameExtents);
	Cairo::FontExtents fontExtents;
	pContext->get_font_extents(fontExtents);
	pSizeOut.x = slotNameExtents.width + connectionRadius * 2;
	pSizeOut.y = fontExtents.height;
	if (ownedConnection) ownedConnection->updateSize(pContext);
}

void ScriptNodeSlot::onWidgetChanged(mvp::CanvasWidget* pWidget) {
	if (ownedConnection) ownedConnection->setWidget(pWidget);
}

void ScriptNodeSlotConnection::getAnotherSlotPos(glm::dvec2 &pPosOut) const {
	pPosOut = -self->getPosition() - self->getParent()->getPosition() + other->getParent()->getPosition() +
			  other->getPosition();
}

mvp::CanvasElement* ScriptNodeSlotConnection::getHoveredElement(const ScriptNodeSlot* /*pCallerSlot*/,
																const glm::dvec2 &pPos) {
	glm::dvec2 anotherSlotPos;
	getAnotherSlotPos(anotherSlotPos);

	const auto distance = length(anotherSlotPos);
	const auto dist1 = glm::distance(pPos, anotherSlotPos);
	const auto dist2 = length(pPos);
	if (dist1 + dist2 > distance + 10) return nullptr;


	double test = pPos.x * anotherSlotPos.y - anotherSlotPos.x * pPos.y;
	if (std::abs(test / distance) < 8) { return this; }
	return nullptr;
}

void ScriptNodeSlotConnection::onDraw(const std::shared_ptr<Cairo::Context> &pContext) {

	double lineWidth{};
	if (isSelected()) {
		pContext->set_source_rgb(1.0, 0.4, 0.2);
		lineWidth = pContext->get_line_width();
		pContext->set_line_width(lineWidth + 3.0);
	} else {
		pContext->set_source_rgb(0.9, 0.9, 0.9);
	}
	pContext->move_to(0, 0);
	glm::dvec2 pos;
	getAnotherSlotPos(pos);
	pContext->line_to(pos.x, pos.y);
	pContext->stroke();
	if (isSelected()) pContext->set_line_width(lineWidth);
}

void ScriptNodeSlotConnection::calculateSize(const std::shared_ptr<Cairo::Context> & /*pContext*/,
											 glm::dvec2 & /*pSizeOut*/) {}

void ScriptNodeSlotConnection::queueDraw() const { getWidget()->queue_draw(); }

} // namespace mer::editor::project