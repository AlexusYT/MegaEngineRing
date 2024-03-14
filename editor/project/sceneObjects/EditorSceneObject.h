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

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include <EngineUtils/utils/UUID.h>

#include "EngineSDK/main/scene/objects/ISceneObject.h"

namespace mer::sdk::main {
class ISceneObject;
}

namespace mer::editor::ui {
class EditorSceneObject : public Glib::Object {
	std::shared_ptr<Gio::ListStore<EditorSceneObject>> childrenUi;

	sdk::main::ISceneObject* nativeObject{};

	explicit EditorSceneObject(sdk::main::ISceneObject* const pNativeObject)
		: childrenUi(Gio::ListStore<EditorSceneObject>::create()), nativeObject(pNativeObject) {}

public:
	static std::shared_ptr<EditorSceneObject> create(sdk::main::ISceneObject* const pNativeObject) {
		return Glib::make_refptr_for_instance(new EditorSceneObject(pNativeObject));
	}

	EditorSceneObject(const EditorSceneObject &pOther) = delete;

	EditorSceneObject &operator=(const EditorSceneObject &pOther) = delete;


	~EditorSceneObject() override = default;

	[[nodiscard]] const Glib::RefPtr<Gio::ListStore<EditorSceneObject>> &getChildrenUi() const { return childrenUi; }

	void addChild(const std::shared_ptr<EditorSceneObject> &pChild) { childrenUi->append(pChild); }

	bool removeChild(const std::shared_ptr<UUID> & /*pUuid*/) {
		throw std::runtime_error("Not implemented yet");
		return false;
	}

	[[nodiscard]] const std::string &getName() const { return nativeObject->getName(); }

	void setName(const std::string &pName) { nativeObject->setName(pName); }

	[[nodiscard]] sdk::main::ISceneObject* getNativeObject() const { return nativeObject; }
};
} // namespace mer::editor::ui

template<>
struct std::formatter<mer::editor::ui::EditorSceneObject> : std::formatter<string_view> {
	auto format(const mer::editor::ui::EditorSceneObject &col, std::format_context &ctx) const {

		std::string temp;
		if (const auto obj = col.getNativeObject())
			std::format_to(std::back_inserter(temp), "Name: {}\nUUID: {}", col.getName(), obj->getUuid());
		else
			std::format_to(std::back_inserter(temp), "Name: {}\nUUID: nullptr", col.getName());
		return std::formatter<string_view>::format(temp, ctx);
	}
};

#endif //SCENEOBJECT_H
