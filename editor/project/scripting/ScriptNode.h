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
// Created by alexus on 25.06.24.
//

#ifndef SCRIPTNODE_H
#define SCRIPTNODE_H
#include "mvp/main/editors/graphicsScriptEditor/canvas/CanvasElement.h"

namespace mer::sdk::main {
class Extension;
}

namespace mer::editor::project {
class ScriptNodeSlot;
class ScriptNodeSlotConnection;

enum class SlotConnectionType { SETTER, CALLBACK };

class ScriptNode : public mvp::CanvasElement {
	std::string name;
	std::map<std::string /*name*/, std::shared_ptr<ScriptNodeSlot>> slots;
	bool hasSize{};
	static constexpr glm::dvec2 connectionSpacing{10, 0};
	int64_t nodeId{};

public:
	sdk::utils::ReportMessagePtr connectToSlot(const std::string &pSelfSlotName, ScriptNodeSlot* pOtherSlot);

	void removeAllConnections() const;

	ScriptNodeSlot* getSlotByName(const std::string &pSelfSlotName);

	sdk::utils::ReportMessagePtr addNewSlot(const std::string &pSlotName, const std::string &pMethodName,
											SlotConnectionType pType);

	[[nodiscard]] const std::map<std::string, std::shared_ptr<ScriptNodeSlot>> &getSlots() const { return slots; }

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	CanvasElement* getHoveredElement(const glm::dvec2 &pPos);

	[[nodiscard]] int64_t getNodeId() const { return nodeId; }

	void setNodeId(const int64_t pNodeId) { nodeId = pNodeId; }

private:
	void onDraw(const std::shared_ptr<Cairo::Context> &pContext) override;

	void calculateSize(const std::shared_ptr<Cairo::Context> &pContext, glm::dvec2 &pSizeOut) override;

protected:
	void onWidgetChanged(mvp::CanvasWidget* pWidget) override;
};

class ScriptNodeSlot : public mvp::CanvasElement {
	friend ScriptNode;
	friend class GraphicsScript;
	friend class ScriptNodeSlotConnection;
	std::string name;
	std::string methodName;
	ScriptNode* parent{};
	ScriptNodeSlot* connectedTo{};
	std::shared_ptr<ScriptNodeSlotConnection> ownedConnection;
	ScriptNodeSlotConnection* usedConnection;
	SlotConnectionType type = SlotConnectionType::SETTER;
	double connectionRadius = 8;

public:
	sdk::utils::ReportMessagePtr connect(ScriptNodeSlot* pOtherSlot);

	sdk::utils::ReportMessagePtr exchange(ScriptNodeSlot* pNewOtherSlot);

	sdk::utils::ReportMessagePtr checkConnectable(ScriptNodeSlot* pOtherSlot);

	void disconnect();

	[[nodiscard]] ScriptNode* getParent() const { return parent; }

	[[nodiscard]] ScriptNodeSlot* getConnectedTo() const { return connectedTo; }

	void setType(SlotConnectionType pType) { type = pType; }

	[[nodiscard]] SlotConnectionType getType() const { return type; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::string &getName() const { return name; }

	[[nodiscard]] const std::string &getMethodName() const { return methodName; }

	void setMethodName(const std::string &pMethodName) { methodName = pMethodName; }

	[[nodiscard]] double getConnectionRadius() const { return connectionRadius; }

	CanvasElement* getHoveredElement(const glm::dvec2 &pPos);

protected:
	void onDraw(const std::shared_ptr<Cairo::Context> &pContext) override;

	void calculateSize(const std::shared_ptr<Cairo::Context> &pContext, glm::dvec2 &pSizeOut) override;

	void onWidgetChanged(mvp::CanvasWidget* pWidget) override;

private:
	void setParent(ScriptNode* pParent) { parent = pParent; }
};

class ScriptNodeSlotConnection : public mvp::CanvasElement {
	friend ScriptNodeSlot;
	ScriptNodeSlot* self{};
	ScriptNodeSlot* other{};

public:
	ScriptNodeSlotConnection(ScriptNodeSlot* pSelf, ScriptNodeSlot* pOther) : self(pSelf), other(pOther) {}

	[[nodiscard]] ScriptNodeSlot* getSelf() const { return self; }

	void setSelf(ScriptNodeSlot* pSelf) { self = pSelf; }

	[[nodiscard]] ScriptNodeSlot* getOther() const { return other; }

	void setOther(ScriptNodeSlot* pOther) { other = pOther; }

	void getAnotherSlotPos(glm::dvec2 &pPosOut) const;

	CanvasElement* getHoveredElement(const ScriptNodeSlot* pCallerSlot, const glm::dvec2 &pPos);

private:
	void onDraw(const std::shared_ptr<Cairo::Context> &pContext) override;

protected:
	void calculateSize(const std::shared_ptr<Cairo::Context> &pContext, glm::dvec2 &pSizeOut) override;


private:
	void queueDraw() const;
};
} // namespace mer::editor::project


#endif //SCRIPTNODE_H
