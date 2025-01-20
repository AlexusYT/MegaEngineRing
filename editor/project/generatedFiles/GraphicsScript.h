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

#ifndef GRAPHICSSCRIPT_H
#define GRAPHICSSCRIPT_H
#include "EngineSDK/extensions/Extension.h"
#include "GeneratedFileEntry.h"
#include "project/Project.h"
#include "project/sceneObjects/EditorSceneObject.h"
#include "project/scripting/ExtensionScriptNode.h"
#include "project/scripting/ScriptNode.h"

namespace mer::editor::project {
class ScriptNode;
}

namespace mer::editor::ui {
class EditorSceneObject;
}

namespace mer::editor::project {
class GraphicsScript : public GeneratedFileEntry, public mvp::ITab {
	int64_t scriptId;
	ui::EditorSceneObject* sceneObject;
	std::unordered_map<std::string /*name*/, std::shared_ptr<ScriptNode>> scriptNodes;
	sigc::signal<void(ScriptNode* pRemovedNode)> nodeRemovingSignal;
	std::vector<std::shared_ptr<ScriptNodeSlotConnection>> connections;
	sigc::signal<void(ScriptNodeSlotConnection* pRemovedConnection)> connectionRemovingSignal;

public:
	explicit GraphicsScript(ui::EditorSceneObject* pSceneObject, const std::shared_ptr<Project> &pProject)
		: GeneratedFileEntry(pProject), sceneObject(pSceneObject) {
		setName(sceneObject->getObjectName() + "Script");
		for (auto [extName, extension]: sceneObject->getNativeObject()->getExtensions()) {
			auto props = extension->getProperties();

			auto node = std::make_shared<ExtensionScriptNode>();
			for (auto property: props) {
				node->addNewSlot(property->getPropertyName(), "setValue", SlotConnectionType::SETTER);
				node->addNewSlot(property->getPropertyName(), "getValue", SlotConnectionType::CALLBACK);
			}
			node->setExtension(extension.get());
			node->setName(extName);
			addNode(node);
		}
		/*{
			ScriptNodeSlot* onAngleChanged;
			{
				const auto iter = scriptNodes.find("camera");
				if (iter == scriptNodes.end()) return;

				onAngleChanged = iter->second->getSlotByName("eventAngle");
			}
			{
				auto iter = scriptNodes.find("cameraKeyboard");
				if (iter == scriptNodes.end()) return;

				onAngleChanged->connect(iter->second->getSlotByName("setAngle"));
			}
		}*/

		/*{
			ScriptNodeSlot* onAngleChanged;
			{
				auto iter = scriptNodes.find("cameraMouse");
				if (iter == scriptNodes.end()) return;

				onAngleChanged = iter->second->getSlotByName("eventAngle");
			}
			{
				auto iter = scriptNodes.find("camera");
				if (iter == scriptNodes.end()) return;

				iter->second->connectToSlot("setAngle", onAngleChanged);
			}
		}*/

		setFileSaveRequired(true);
	}

	~GraphicsScript() override {
		for (auto iter = scriptNodes.begin(); iter != scriptNodes.end(); ++iter) {
			nodeRemovingSignal(iter->second.get());
		}
		scriptNodes.clear();
	}

	void addNode(std::shared_ptr<ScriptNode> pNode);

	sdk::ReportMessagePtr removeNode(ScriptNode* pNodeToRemove);

	void getHoveredElements(const glm::dvec2 &pPos, std::list<mvp::CanvasElement*> &pElementsOut) const;

	sigc::connection connectNodeRemovingSignal(const sigc::slot<void(ScriptNode* pRemovedNode)> &pSlot) {
		return nodeRemovingSignal.connect(pSlot);
	}

	sigc::connection connectConnectionRemovingSignal(
		const sigc::slot<void(ScriptNodeSlotConnection* pRemovedConnection)> &pSlot) {
		return connectionRemovingSignal.connect(pSlot);
	}

	sdk::ReportMessagePtr removeConnection(ScriptNodeSlotConnection* pConnectionToRemove);

	sdk::ReportMessagePtr removeAllConnections(const ScriptNode* pNode);

	std::shared_ptr<mvp::IPresenter> createEditorPresenter(
		const std::shared_ptr<mvp::TabPlaceholder> &pPlaceholder) override;

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<ScriptNode>> &getScriptNodes() const {
		return scriptNodes;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<ScriptNodeSlotConnection>> &getConnections() const {
		return connections;
	}

	[[nodiscard]] int64_t getScriptId() const { return scriptId; }

	void setScriptId(int64_t pScriptId) { scriptId = pScriptId; }

	static sdk::ReportMessagePtr createTable(const std::shared_ptr<SQLite::Database> &);

private:
	sdk::ReportMessagePtr onSaveDatabase() const override;
	sdk::ReportMessagePtr onSaveFile() const override;
};
} // namespace mer::editor::project

#endif //GRAPHICSSCRIPT_H
