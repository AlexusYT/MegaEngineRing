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
// Created by alexus on 08.01.24.
//

#ifndef ENGINESCENEOBJECT_H
#define ENGINESCENEOBJECT_H

//TODO delete file
#include <EngineUtils/utils/UUID.h>

#include "EngineSDK/extensions/MainObjectExtension.h"
#include "EngineSDK/scene/objects/ISceneObject.h"
#include "project/generatedFiles/GeneratedFiles.h"

namespace mer::editor::mvp {
class ObjectPropertyEntry;
}

namespace mer::editor::project {
class Project;
class GraphicsScript;
} // namespace mer::editor::project

namespace mer::sdk {
class ISceneObject;
}

namespace mer::editor::ui {
class EditorSceneObject : public project::GeneratedFiles {
	std::shared_ptr<Gio::ListStore<EditorSceneObject>> childrenUi;

	std::shared_ptr<project::GraphicsScript> graphicsScript;
	sdk::ISceneObject* nativeObject{};

	EditorSceneObject(sdk::ISceneObject* pNativeObject, std::shared_ptr<project::Project> pProject);

public:
	static std::shared_ptr<EditorSceneObject> create(sdk::ISceneObject* const pNativeObject,
													 const std::shared_ptr<project::Project> &pProject) {
		return Glib::make_refptr_for_instance(new EditorSceneObject(pNativeObject, pProject));
	}

	EditorSceneObject(const EditorSceneObject &pOther) = delete;

	EditorSceneObject &operator=(const EditorSceneObject &pOther) = delete;


	~EditorSceneObject() override = default;
	void onGetActionGroup(const Glib::RefPtr<Gio::SimpleActionGroup> &) override;

	[[nodiscard]] const Glib::RefPtr<Gio::ListStore<EditorSceneObject>> &getChildrenUi() const { return childrenUi; }

	void addChild(const std::shared_ptr<EditorSceneObject> &pChild) { childrenUi->append(pChild); }

	bool removeChild(const std::shared_ptr<UUID> & /*pUuid*/) {
		throw std::runtime_error("Not implemented yet");
		return false;
	}

	[[nodiscard]] const std::string &getObjectName() const { return nativeObject->getMainExtension()->propertyName; }

	void setObjectName(const std::string &pName) { nativeObject->getMainExtension()->propertyName = pName; }

	void setNativeObject(sdk::ISceneObject* pNativeObject) { nativeObject = pNativeObject; }

	[[nodiscard]] sdk::ISceneObject* getNativeObject() const { return nativeObject; }

	[[nodiscard]] const std::shared_ptr<project::GraphicsScript> &getGraphicsScript() const { return graphicsScript; }

private:
	sdk::ReportMessagePtr onSaveDatabase() const override;
};
} // namespace mer::editor::ui

template<>
struct std::formatter<mer::editor::ui::EditorSceneObject> : std::formatter<string_view> {
	auto format(const mer::editor::ui::EditorSceneObject &col, std::format_context &ctx) const {

		std::string temp;
		if (const auto obj = col.getNativeObject())
			std::format_to(std::back_inserter(temp), "Name: {}\nUUID: {}", col.getObjectName(), obj->getUuid());
		else
			std::format_to(std::back_inserter(temp), "Name: {}\nUUID: nullptr", col.getObjectName());
		return std::formatter<string_view>::format(temp, ctx);
	}
};

#endif //ENGINESCENEOBJECT_H
