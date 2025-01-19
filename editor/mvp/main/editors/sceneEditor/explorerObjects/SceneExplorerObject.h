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
// Created by alexus on 31.08.24.
//

#ifndef SCENEEXPLOREROBJECT_H
#define SCENEEXPLOREROBJECT_H
#include "ExplorerObject.h"

namespace mer::sdk::main {
class Extension;
class ISceneObject;
} // namespace mer::sdk::main

namespace mer::editor::mvp {
class SceneExplorerObject : public ExplorerObject {
	sdk::main::ISceneObject* object;
	std::vector<sigc::connection> connections;
	std::shared_ptr<Gio::ListStore<ObjectExtensionEntry>> propertyEntries;


	explicit SceneExplorerObject(sdk::main::ISceneObject* pObject);

public:
	static std::shared_ptr<SceneExplorerObject> create(sdk::main::ISceneObject* pObject);

	[[nodiscard]] const std::string &getName() const override;

	void setName(const std::string &pName) override;

	std::shared_ptr<Gio::ListStore<ObjectExtensionEntry>> getPropertyEntries() const override;

	[[nodiscard]] sdk::main::ISceneObject* getObject() const { return object; }

	void setObject(sdk::main::ISceneObject* pObject);

	Glib::RefPtr<Gio::MenuModel> getMenu() override;

private:
	void addExtension(const std::shared_ptr<sdk::main::Extension> &pExtension) const;

	static std::shared_ptr<Gio::MenuItem> createItem(const std::string &pName, const std::string &pAction,
													 const Glib::VariantBase &pVariant);
};
} // namespace mer::editor::mvp

#endif //SCENEEXPLOREROBJECT_H
