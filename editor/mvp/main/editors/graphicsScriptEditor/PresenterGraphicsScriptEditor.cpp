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
// Created by alexus on 09.07.24.
//

#include "PresenterGraphicsScriptEditor.h"

#include "IModelGraphicsScriptEditor.h"
#include "IViewGraphicsScriptEditor.h"
#include "project/generatedFiles/GraphicsScript.h"
#include "project/scripting/FakeScriptNode.h"

namespace mer::editor::mvp {
PresenterGraphicsScriptEditor::PresenterGraphicsScriptEditor(
	const std::shared_ptr<IViewGraphicsScriptEditor> &pViewGraphicsScriptEditor,
	const std::shared_ptr<IModelGraphicsScriptEditor> &pModelGraphicsScriptEditor)
	: viewGraphicsScriptEditor(pViewGraphicsScriptEditor), modelGraphicsScriptEditor(pModelGraphicsScriptEditor) {

	const auto script = modelGraphicsScriptEditor->getScript();
	auto fakeNode = std::make_shared<project::FakeScriptNode>();
	script->addNode(fakeNode);
	project::ScriptNodeSlot* fakeSetterSlot = fakeNode->getSlotByName("FakeSetterSlot");
	project::ScriptNodeSlot* fakeCallbackSlot = fakeNode->getSlotByName("FakeCallbackSlot");
	script->connectNodeRemovingSignal(
		[this](project::ScriptNode* pNode) { viewGraphicsScriptEditor->removeElement(pNode); });
	script->connectConnectionRemovingSignal([this](project::ScriptNodeSlotConnection* pConnection) {
		viewGraphicsScriptEditor->removeElement(pConnection);
	});
	int count = 0;
	for (auto node: script->getScriptNodes() | std::views::values) {
		//auto nodeSize = node->getSize();
		node->setPosition({(300) * count, 250 * count});
		viewGraphicsScriptEditor->addElement(node.get());
		count++;
	}

	viewGraphicsScriptEditor->connectKeyPressedEvent(
		[this, fakeSetterSlot, fakeCallbackSlot](const uint32_t pKeyVal, uint32_t /*pKeyCode*/,
												 Gdk::ModifierType /*pState*/) {
			if (pKeyVal == GDK_KEY_Delete) {
				const auto element = modelGraphicsScriptEditor->getSelectedElement();
				if (!element) return false;
				const auto scriptSelf = modelGraphicsScriptEditor->getScript();
				if (const auto connection = dynamic_cast<project::ScriptNodeSlotConnection*>(element)) {
					connection->getSelf()->disconnect();
					viewGraphicsScriptEditor->redrawCanvas();
				} else if (const auto extNode = dynamic_cast<project::ExtensionScriptNode*>(element)) {
					extNode->removeAllConnections();
					viewGraphicsScriptEditor->redrawCanvas();
				} else if (const auto node = dynamic_cast<project::ScriptNode*>(element)) {
					if (const auto msg = scriptSelf->removeNode(node)) { sdk::utils::Logger::error(msg); }
				}
			}
			if (pKeyVal == GDK_KEY_Escape) {
				fakeCallbackSlot->disconnect();
				fakeSetterSlot->disconnect();
			}
			return true;
		},
		true);
	viewGraphicsScriptEditor->connectMouseMovedEvent(
		[this, fakeNode](const double pX, const double pY) {
			const auto scriptSelf = modelGraphicsScriptEditor->getScript();
			const glm::dvec2 cursorPos = glm::dvec2(pX, pY) / viewGraphicsScriptEditor->getCanvasZoom() -
										 viewGraphicsScriptEditor->getCanvasOffset();
			fakeNode->setPosition(cursorPos);
			std::list<CanvasElement*> hoveredElements;
			scriptSelf->getHoveredElements(cursorPos, hoveredElements);
			CanvasElement* element{};
			CanvasElement* connection{};
			for (const auto hoveredElement: hoveredElements) {
				if (const auto node = dynamic_cast<project::ScriptNode*>(element)) {
					if (node == fakeNode.get()) continue;
					element = hoveredElement;

				} else if (const auto nodeSlot = dynamic_cast<project::ScriptNodeSlot*>(hoveredElement)) {
					if (nodeSlot->getParent() == fakeNode.get()) continue;
					element = hoveredElement;
				} else if (const auto nodeSlotConnection =
							   dynamic_cast<project::ScriptNodeSlotConnection*>(hoveredElement)) {
					if (nodeSlotConnection->getSelf()->getParent() == fakeNode.get() ||
						nodeSlotConnection->getOther()->getParent() == fakeNode.get())
						continue;
					connection = nodeSlotConnection;
				} else {

					element = hoveredElement;
				}
			}
			if (connection) element = connection;
			modelGraphicsScriptEditor->setHoveredElement(element);
			/*if (auto node = dynamic_cast<project::ScriptNode*>(element)) {
				sdk::utils::Logger::out(node->getName() + " hovered");
			} else if (auto nodeSlot = dynamic_cast<project::ScriptNodeSlot*>(element)) {
				sdk::utils::Logger::out("{}->{} hovered", nodeSlot->getParent()->getName(), nodeSlot->getName());
			} else if (auto nodeSlotConnection = dynamic_cast<project::ScriptNodeSlotConnection*>(element)) {
				sdk::utils::Logger::out(
					"Connection between {}->{} and {}->{} hovered",
					nodeSlotConnection->getSelf()->getParent()->getName(), nodeSlotConnection->getSelf()->getName(),
					nodeSlotConnection->getOther()->getParent()->getName(), nodeSlotConnection->getOther()->getName());
			}*/
		},
		true);
	viewGraphicsScriptEditor->connectMouseButtonPressedEvent(
		[this, fakeSetterSlot, fakeCallbackSlot](int /*pPressCount*/, const double /*pX*/, const double /*pY*/) {
			const auto hoveredElement = modelGraphicsScriptEditor->getHoveredElement();
			if (const auto node = dynamic_cast<project::ScriptNode*>(hoveredElement)) {
				modelGraphicsScriptEditor->setMovingElement(node);
			} else if (const auto nodeSlot = dynamic_cast<project::ScriptNodeSlot*>(hoveredElement)) {
				if (nodeSlot->getConnectedTo()) return;
				switch (nodeSlot->getType()) {

					case project::SlotConnectionType::SETTER:
						if (const auto fakeConnectedTo = fakeSetterSlot->getConnectedTo()) {
							if (fakeConnectedTo->getType() == project::SlotConnectionType::CALLBACK)
								fakeConnectedTo->exchange(nodeSlot);
						} else if (!fakeCallbackSlot->getConnectedTo())
							nodeSlot->connect(fakeCallbackSlot);
						break;
					case project::SlotConnectionType::CALLBACK:
						if (const auto fakeConnectedTo = fakeCallbackSlot->getConnectedTo()) {
							if (fakeConnectedTo->getType() == project::SlotConnectionType::SETTER) {
								fakeConnectedTo->exchange(nodeSlot);
							}

						} else if (!fakeSetterSlot->getConnectedTo())
							nodeSlot->connect(fakeSetterSlot);
						break;
				}
			}
		},
		true);
	viewGraphicsScriptEditor->connectMouseButtonReleasedEvent(
		[this](int /*pPressCount*/, const double /*pX*/, const double /*pY*/) {
			if (moving) return;
			modelGraphicsScriptEditor->setSelectedElement(modelGraphicsScriptEditor->getHoveredElement());
		},
		true);
	modelGraphicsScriptEditor->connectSelectedElementChanged(
		[this](CanvasElement* pElement) { viewGraphicsScriptEditor->setSelectedElement(pElement); });

	viewGraphicsScriptEditor->connectDragBegin(
		[this](double /*pX*/, double /*pY*/) {
			if (const auto button = viewGraphicsScriptEditor->getCurrentDragButton(); button == 1) {
				if (const auto movingElement = modelGraphicsScriptEditor->getMovingElement()) {
					prevElementPos = movingElement->getPosition();
				}
			}
		},
		true);
	viewGraphicsScriptEditor->connectDragUpdate(
		[this](const double pX, const double pY) {
			const glm::dvec2 deltaPos = glm::dvec2(pX, pY) / viewGraphicsScriptEditor->getCanvasZoom();
			if (const auto button = viewGraphicsScriptEditor->getCurrentDragButton(); button == 1) {
				if (const auto movingElement = modelGraphicsScriptEditor->getMovingElement()) {

					movingElement->setPosition(prevElementPos + deltaPos);
					moving = true;
				}
			}
		},
		true);
	viewGraphicsScriptEditor->connectDragEnd(
		[this](double /*pX*/, double /*pY*/) {
			if (const auto button = viewGraphicsScriptEditor->getCurrentDragButton(); button == 1) {
				moving = false;
				modelGraphicsScriptEditor->setMovingElement(nullptr);
			}
		},
		true);
}

void PresenterGraphicsScriptEditor::run() { viewGraphicsScriptEditor->openView(); }

void PresenterGraphicsScriptEditor::stop() { viewGraphicsScriptEditor->closeView(); }

} // namespace mer::editor::mvp

// mvp