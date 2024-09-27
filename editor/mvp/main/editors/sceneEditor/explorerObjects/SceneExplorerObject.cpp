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
// Created by alexus on 31.08.24.
//

#include "SceneExplorerObject.h"

#include "EngineSDK/main/scene/objects/ISceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionProperties.h"
#include "mvp/main/objectProperties/ObjectPropertyEntry.h"

namespace mer::editor::mvp {
SceneExplorerObject::SceneExplorerObject(sdk::main::ISceneObject* pObject)
	: propertyEntries(Gio::ListStore<ObjectPropertyEntry>::create()) {
	setObject(pObject);
}

std::shared_ptr<SceneExplorerObject> SceneExplorerObject::create(sdk::main::ISceneObject* pObject) {
	return Glib::make_refptr_for_instance(new SceneExplorerObject(pObject));
}

const std::string &SceneExplorerObject::getName() const { return object->getName(); }

void SceneExplorerObject::setName(const std::string &pName) { object->setName(pName); }

std::shared_ptr<Gio::ListStore<ObjectPropertyEntry>> SceneExplorerObject::getPropertyEntries() const {
	return propertyEntries;
}

void SceneExplorerObject::setObject(sdk::main::ISceneObject* pObject) {
	if (object == pObject) return;
	object = pObject;
	propertyEntries->remove_all();
	for (auto connection: connections) { connection.disconnect(); }
	connections.clear();
	/*connections.push_back(
		pObject->connectOnNameChanged([this](const std::string & pOldName, const std::string &pNewName) {
			name = pNewName;
		}));*/

	auto basicGroup = std::make_shared<sdk::main::ExtensionPropertyGroup>();
	basicGroup->setName("Basic properties");
	auto nameProp = std::make_shared<sdk::main::ExtensionProperty<std::string>>();
	nameProp->setName("Object name");
	nameProp->setGetterFunc(sigc::mem_fun(*object, &sdk::main::ISceneObject::getName));
	nameProp->setSetterFunc(sigc::mem_fun(*object, &sdk::main::ISceneObject::setName));
	basicGroup->addChild(nameProp);
	auto positionProp = std::make_shared<sdk::main::ExtensionProperty<glm::vec3>>();
	positionProp->setName("Position");
	positionProp->setGetterFunc(sigc::mem_fun(*object, &sdk::main::ISceneObject::getPosition));
	positionProp->setSetterFunc(sigc::mem_fun(*object, &sdk::main::ISceneObject::setPosition));
	basicGroup->addChild(positionProp);

	propertyEntries->append(ObjectPropertyEntry::create(basicGroup));

	for (const auto &extension: object->getExtensions()) { addExtension(extension.second); }

	connections.push_back(object->connectOnExtensionAdded(
		[this](const std::shared_ptr<sdk::main::Extension> &pNewExt) { addExtension(pNewExt); }));
}

Glib::RefPtr<Gio::MenuModel> SceneExplorerObject::getMenu() {
	auto menu = Gio::Menu::create();
	const auto variant = Glib::Variant<uintptr_t>::create(reinterpret_cast<uintptr_t>(this));
	menu->append_item(createItem("New Object", "object.manage.new.object", variant));
	menu->append_item(createItem("Delete", "object.manage.remove.object", variant));
	return menu;
}

void SceneExplorerObject::addExtension(const std::shared_ptr<sdk::main::Extension> &pExtension) const {
	auto group = std::make_shared<sdk::main::ExtensionPropertyGroup>();
	group->setName(pExtension->getTypeName());
	for (sdk::main::ExtensionProperties properties = pExtension->getProperties();
		 const auto &extensionPropertyBase: properties) {
		group->addChild(extensionPropertyBase);
	}
	propertyEntries->append(ObjectPropertyEntry::create(group));
}

std::shared_ptr<Gio::MenuItem> SceneExplorerObject::createItem(const std::string &pName, const std::string &pAction,
															   const Glib::VariantBase &pVariant) {
	std::shared_ptr<Gio::MenuItem> item = Gio::MenuItem::create(pName, "");
	item->set_action_and_target(pAction, pVariant);
	return item;
}
} // namespace mer::editor::mvp