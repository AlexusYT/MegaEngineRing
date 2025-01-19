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
// Created by alexus on 24.06.24.
//

#include "ScriptParser.h"

#include "EngineSDK/main/scripting/Script.h"
#include "project/Project.h"
#include "project/generatedFiles/GraphicsScript.h"
#include "project/generators/cpp/CppClass.h"
#include "project/generators/cpp/CppCustomStatement.h"
#include "project/generators/cpp/CppExternC.h"
#include "project/generators/cpp/CppHeaderFile.h"
#include "project/generators/cpp/CppMethod.h"

namespace mer::editor::project {

void ScriptParser::generateSource(const GraphicsScript* pScript, CppSourceFile &pSourceFile) {

	std::string scriptName = pScript->getName();

	pSourceFile.addInclude("EngineSDK/main/scripting/Script.h");
	pSourceFile.addInclude("EngineUtils/utils/ReportMessage.h");
	pSourceFile.addInclude("memory");
	pSourceFile.addInclude("iostream");


	auto setupMethod = CppMethod::create();
	setupMethod->setName("setup");
	setupMethod->setIsOverride(true);
	setupMethod->setReturnType<sdk::utils::ReportMessagePtr>();

	std::unordered_map<ScriptNodeSlot*, bool> connected;
	auto nodes = pScript->getScriptNodes();

	setupMethod->addStatement(CppCustomStatement::create(R"(using namespace mer::sdk::main)"));
	setupMethod->addStatement(CppCustomStatement::create(R"(auto extensions = getObject()->getExtensions())"));
	for (auto [nodeName, node]: nodes) {
		if (auto extNode = std::dynamic_pointer_cast<ExtensionScriptNode>(node)) {
			auto ext = extNode->getExtension();
			std::string line = std::format(R"(auto {0} = std::dynamic_pointer_cast<{1}>(extensions.at("{0}")))",
										   ext->getName(), ext->getTypeName());
			setupMethod->addStatement(CppCustomStatement::create(line));

			pSourceFile.addInclude(ext->getHeader());
			for (auto [name, slot]: node->getSlots()) {
				if (auto iter = connected.find(slot.get()); iter != connected.end()) continue;


				auto connectedTo = slot->getConnectedTo();
				if (!connectedTo) {
					connected.emplace(slot.get(), false);

					continue;
				}
				if (auto iter = connected.find(connectedTo); iter != connected.end()) continue;
				connected.emplace(slot.get(), true);
			}
		}
	}

	for (auto [slot, isConnected]: connected) {
		if (!isConnected) continue;
		ScriptNodeSlot* eventSlot;
		ScriptNodeSlot* setterSlot;
		if (slot->getType() == SlotConnectionType::CALLBACK) {
			eventSlot = slot;
			setterSlot = slot->getConnectedTo();
		} else {
			eventSlot = slot->getConnectedTo();
			setterSlot = slot;
		}
		auto eventNode = eventSlot->getParent();
		auto setterNode = setterSlot->getParent();
		sdk::main::Extension* eventExtension;
		sdk::main::Extension* setterExtension;
		if (auto extEventNode = dynamic_cast<ExtensionScriptNode*>(eventNode)) {
			eventExtension = extEventNode->getExtension();
		} else {
			throw std::runtime_error("Unsupported extension type");
		}

		if (auto extSetterNode = dynamic_cast<ExtensionScriptNode*>(setterNode)) {
			setterExtension = extSetterNode->getExtension();
		} else {
			throw std::runtime_error("Unsupported extension type");
		}

		std::string line =
			std::format(R"({0}->{1}().connect(sigc::mem_fun(*{2}, &{3}::{4})))", eventExtension->getName(),
						eventSlot->getMethodName(), setterExtension->getName(), setterExtension->getTypeName(),
						setterSlot->getMethodName());
		setupMethod->addStatement(CppCustomStatement::create(line));
	}
	setupMethod->addStatement(
		CppCustomStatement::create(R"(std::cout<<"Script generated from script parser."<<std::endl)"));
	setupMethod->addStatement(CppCustomStatement::create(R"(return 0)"));

	auto clazz = CppClass::create(scriptName);

	clazz->setImplementsList({"public mer::sdk::main::Script"});
	clazz->addDefinition(setupMethod->getDefinition(), AccessModifier::PUBLIC);

	pSourceFile.addDefinition(clazz->getDefinition());


	auto createMethod = CppMethod::create();
	createMethod->setName("create" + scriptName);
	createMethod->setReturnType<std::shared_ptr<sdk::main::Script>>();
	createMethod->addStatement(CppCustomStatement::create("return std::make_shared<" + scriptName + ">();"));

	auto externC = CppExternC::create();
	externC->addDefinition(createMethod->getDefinition());
	pSourceFile.addDefinition(externC->getDefinition());
}

} // namespace mer::editor::project