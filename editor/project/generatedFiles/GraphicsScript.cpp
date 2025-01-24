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

#include "GraphicsScript.h"

#include <nlohmann/json.hpp>

#include "mvp/main/editors/graphicsScriptEditor/ModelGraphicsScriptEditor.h"
#include "mvp/main/editors/graphicsScriptEditor/PresenterGraphicsScriptEditor.h"
#include "mvp/main/editors/graphicsScriptEditor/ViewGraphicsScriptEditor.h"
#include "project/generators/cpp/CppHeaderFile.h"
#include "project/scripting/ScriptParser.h"

namespace mer::editor::project {

void GraphicsScript::addNode(std::shared_ptr<ScriptNode> pNode) { scriptNodes.emplace(pNode->getName(), pNode); }

sdk::ReportMessagePtr GraphicsScript::removeNode(ScriptNode* pNodeToRemove) {
	std::string nodeName = pNodeToRemove->getName();
	auto iter = scriptNodes.find(nodeName);
	if (iter == scriptNodes.end()) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Unable to remove node from script");
		msg->setMessage("Node not found");
		auto pos = pNodeToRemove->getPosition();
		msg->addInfoLine("Node position: (X: {}; Y: {})", pos.x, pos.y);
		msg->addInfoLine("Node name: " + nodeName);
		return msg;
	}

	if (auto msg = removeAllConnections(iter->second.get())) { return msg; }
	nodeRemovingSignal(pNodeToRemove);
	pNodeToRemove->onRemove();
	scriptNodes.erase(iter);

	return nullptr;
}

void GraphicsScript::getHoveredElements(const glm::dvec2 &pPos, std::list<mvp::CanvasElement*> &pElementsOut) const {
	for (auto scriptNode: scriptNodes) {
		glm::dvec2 cursorPos = pPos - scriptNode.second->getPosition();
		if (auto hoveredElement = scriptNode.second->getHoveredElement(cursorPos))
			pElementsOut.emplace_back(hoveredElement);
	}
}

sdk::ReportMessagePtr GraphicsScript::removeConnection(ScriptNodeSlotConnection* pConnectionToRemove) {

	connectionRemovingSignal(pConnectionToRemove);
	pConnectionToRemove->onRemove();

	return nullptr;
}

sdk::ReportMessagePtr GraphicsScript::removeAllConnections(const ScriptNode* pNode) {

	for (auto slot: pNode->getSlots()) {


		auto conn = slot.second->ownedConnection;
		if (!conn) continue;
		conn->getOther()->usedConnection = nullptr;
		if (auto msg = removeConnection(conn.get())) return msg;
	}
	return nullptr;
}

std::shared_ptr<mvp::IPresenter> GraphicsScript::createEditorPresenter(const std::shared_ptr<mvp::TabPlaceholder> &/*pPlaceholder*/) {

	auto model = std::make_shared<mvp::ModelGraphicsScriptEditor>();
	model->setScript(this);
	auto view = std::make_shared<mvp::ViewGraphicsScriptEditor>();
	return std::make_shared<mvp::PresenterGraphicsScriptEditor>(view, model);
}

sdk::ReportMessagePtr GraphicsScript::createTable(const std::shared_ptr<SQLite::Database> &pDatabase) {
	try {
		//language=sql
		pDatabase->exec(R"(DROP TABLE IF EXISTS ScriptNodes;
CREATE TABLE ScriptNodes
(
    NodeID   INTEGER
        CONSTRAINT ScriptNodes_pk
            PRIMARY KEY AUTOINCREMENT,
    ScriptID     INTEGER
        CONSTRAINT ScriptNodes_Scripts_ScriptID_fk
            REFERENCES Scripts,
    NodePosition TEXT    NOT NULL,
    NodeName     TEXT    NOT NULL
);
)");
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to create ScriptNodes table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	try {
		//language=sql
		pDatabase->exec(R"(DROP TABLE IF EXISTS ScriptNodeConnections;
CREATE TABLE "ScriptNodeConnections" (
	"ConnectionID"	INTEGER,
	"FirstNodeID"	INTEGER NOT NULL,
	"SecondNodeID"	INTEGER NOT NULL,
	"FirstSlotName"	TEXT NOT NULL,
	"SecondSlotName"	TEXT NOT NULL,
	CONSTRAINT "ScriptNodeConnections_pk" PRIMARY KEY("ConnectionID" AUTOINCREMENT),
	FOREIGN KEY("FirstNodeID") REFERENCES "ScriptNodes"("NodeID"),
	FOREIGN KEY("SecondNodeID") REFERENCES "ScriptNodes"("NodeID")
);
)");
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to create ScriptNodeConnections table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	return nullptr;
}

sdk::ReportMessagePtr GraphicsScript::onSaveDatabase() const {
	auto database = getProject()->getDatabase();

	std::unique_ptr<SQLite::Transaction> transaction;
	try {
		transaction = std::make_unique<SQLite::Transaction>(*database);
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to save ScriptNodes into database");
		msg->setMessage("Exception occurred while starting transaction");
		return msg;
	}
	try {
		//language=sql
		SQLite::Statement statementNodeConnection(*database, R"(
INSERT INTO ScriptNodeConnections (FirstNodeID, SecondNodeID, FirstSlotName, SecondSlotName) VALUES (?, ?, ?, ?)
)");
		//language=sql
		SQLite::Statement statementNode(*database, R"(
INSERT INTO ScriptNodes (ScriptID, NodePosition, NodeName) VALUES (?, ?, ?)
)");
		for (auto [nodeName, node]: scriptNodes) {
			statementNode.bind(1, getScriptId());
			auto pos = node->getPosition();
			nlohmann::json json = {pos.x, pos.y};
			statementNode.bind(2, json.dump());
			statementNode.bind(3, nodeName);
			statementNode.executeStep();
			statementNode.clearBindings();
			statementNode.reset();

			node->setNodeId(database->getLastInsertRowid());

			for (auto [slotName, slot]: node->getSlots()) {
				auto firstId = slot->getParent()->getNodeId();
				if (firstId == 0) continue;
				auto connectedTo = slot->getConnectedTo();
				if (!connectedTo) continue;
				auto secondId = connectedTo->getParent()->getNodeId();
				if (secondId == 0) continue;
				statementNodeConnection.bind(1, firstId);
				statementNodeConnection.bind(2, secondId);
				statementNodeConnection.bind(3, slotName);
				statementNodeConnection.bind(4, connectedTo->getName());
				statementNodeConnection.executeStep();
				statementNodeConnection.clearBindings();
				statementNodeConnection.reset();
			}
		}
	} catch (...) {
		transaction->rollback();
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to create ScriptNodes table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	transaction->commit();

	return nullptr;
	//return onSaveFile();
}

sdk::ReportMessagePtr GraphicsScript::onSaveFile() const {

	auto parser = getProject()->getScriptParser();
	CppSourceFile source;
	parser->generateSource(this, source);
	auto filePath = getProject()->getProjectPath() / "source/scripts";
	auto fileName = filePath / (getName() + ".cpp");

	if (auto msg = source.writeFile(fileName)) return msg;

	return nullptr;
}


} // namespace mer::editor::project