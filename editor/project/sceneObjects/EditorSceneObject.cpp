// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 08.01.24.
//

#include "EditorSceneObject.h"

#include "../../mvp/main/objectProperties/ObjectPropertyEntry.h"
#include "EditorSceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "mvp/main/centerWindow/ViewCenterWindow.h"
#include "project/generatedFiles/GraphicsScript.h"

namespace mer::editor::ui {
EditorSceneObject::EditorSceneObject(sdk::main::ISceneObject* const pNativeObject,
									 std::shared_ptr<project::Project> pProject)
	: GeneratedFiles(pProject), childrenUi(Gio::ListStore<EditorSceneObject>::create()),
	  propertyEntries(Gio::ListStore<mvp::ObjectPropertyEntry>::create()), nativeObject(pNativeObject) {

	graphicsScript = std::make_shared<project::GraphicsScript>(this, pProject);

	auto menuOpenScript = Gio::Menu::create();
	menuOpenScript->append("Open Visual Script", "win.object." + getObjectName() + ".open.graphicScript");
	setContextMenu(menuOpenScript);

	auto basicGroup = std::make_shared<sdk::main::ExtensionPropertyGroup>();
	basicGroup->setName("Basic properties");
	auto nameProp = std::make_shared<sdk::main::ExtensionProperty<std::string>>();
	nameProp->setName("Object name");
	nameProp->setGetterFunc(sigc::mem_fun(*this, &EditorSceneObject::getObjectName));
	nameProp->setSetterFunc(sigc::mem_fun(*this, &EditorSceneObject::setObjectName));
	basicGroup->addChild(nameProp);
	auto positionProp = std::make_shared<sdk::main::ExtensionProperty<glm::vec3>>();
	positionProp->setName("Position");
	positionProp->setGetterFunc(sigc::mem_fun(*pNativeObject, &sdk::main::ISceneObject::getPosition));
	positionProp->setSetterFunc(sigc::mem_fun(*pNativeObject, &sdk::main::ISceneObject::setPosition));
	basicGroup->addChild(positionProp);

	propertyEntries->append(mvp::ObjectPropertyEntry::create(basicGroup));

	for (const auto &extension: pNativeObject->getExtensions()) {
		auto group = std::make_shared<sdk::main::ExtensionPropertyGroup>();
		group->setName(extension.second->getTypeName());
		sdk::main::ExtensionProperties properties = extension.second->getProperties();
		for (const auto &extensionPropertyBase: properties) { group->addChild(extensionPropertyBase); }
		propertyEntries->append(mvp::ObjectPropertyEntry::create(group));
	}
}

void EditorSceneObject::onGetActionGroup(const Glib::RefPtr<Gio::SimpleActionGroup> &pActionGroup) {
	pActionGroup->add_action("object." + getObjectName() + ".open.graphicScript",
							 [this] { getCenterWindow()->openEntry(graphicsScript.get()); });
}

sdk::utils::ReportMessagePtr EditorSceneObject::onSaveDatabase() const { return graphicsScript->saveDatabase(); }
} // namespace mer::editor::ui