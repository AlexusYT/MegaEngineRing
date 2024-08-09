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
// Created by alexus on 28.01.24.
//

#ifndef SCENEINFO_H
#define SCENEINFO_H
#include <EngineUtils/utils/UUID.h>

#include "GeneratedFileEntry.h"
#include "project/sceneObjects/EditorSceneObject.h"

namespace mer::editor::ui {
class EditorSceneObject;
}

namespace mer::editor::project {
class CppClass;
class CppExternC;
} // namespace mer::editor::project

namespace mer::editor::project {
class CppMethod;

class SceneInfo : public GeneratedFileEntry {
	std::shared_ptr<UUID> uuid;
	bool primaryScene{true};

	std::unordered_map<UUID*, ui::EditorSceneObject*> objects;

	std::shared_ptr<Gio::ListStore<ui::EditorSceneObject>> toplevelObjects;

protected:
	explicit SceneInfo(const std::shared_ptr<Project> &pProject);

	explicit SceneInfo(const std::shared_ptr<Project> &pProject, const std::string &pName,
					   const std::shared_ptr<UUID> &pUuid);

public:
	std::shared_ptr<mvp::IEditorPresenter> openEditor() override;

	static std::shared_ptr<SceneInfo> create(const std::shared_ptr<Project> &pProject) {
		return Glib::make_refptr_for_instance(new SceneInfo(pProject));
	}

	static std::shared_ptr<SceneInfo> create(const std::shared_ptr<Project> &pProject, const std::string &pName,
											 const std::shared_ptr<UUID> &pUuid) {
		return Glib::make_refptr_for_instance(new SceneInfo(pProject, pName, pUuid));
	}

	[[nodiscard]] const std::shared_ptr<UUID> &getUuid() const { return uuid; }

	void addToplevelSceneObject(UUID* pUuid, const std::shared_ptr<ui::EditorSceneObject> &pObject) {
		pObject->setCenterWindow(getCenterWindow());
		toplevelObjects->append(pObject);
		objects.emplace(pUuid, pObject.get());
	}

	[[nodiscard]] const std::shared_ptr<Gio::ListStore<ui::EditorSceneObject>> &getToplevelObjects() const {
		return toplevelObjects;
	}

	void onGetActionGroup(const Glib::RefPtr<Gio::SimpleActionGroup> &) override;

	void resetSceneInfo();

private:
	sdk::utils::ReportMessagePtr onLoadDatabase() override;

	sdk::utils::ReportMessagePtr onSaveDatabase() const override;

	sdk::utils::ReportMessagePtr onSaveFile() const override;

	sdk::utils::ReportMessagePtr createTable() const;


	sdk::utils::ReportMessagePtr writeFile() const;

	std::filesystem::path getHeaderPath() const override { return "source/scenes/" + getName() + ".h"; }

	std::filesystem::path getSourcePath() const override { return "source/scenes/" + getName() + ".cpp"; }

	static std::shared_ptr<CppExternC> createExternCBlock(const std::string &pSceneName);

	static std::shared_ptr<CppMethod> createInitMethod(const std::shared_ptr<CppClass> &pClass);
};

} // namespace mer::editor::project

#endif //SCENEINFO_H
