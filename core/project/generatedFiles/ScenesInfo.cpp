//
// Created by alexus on 27.01.24.
//

#include "ScenesInfo.h"

namespace PROJECT_CORE {
ScenesInfo::ScenesInfo(const std::shared_ptr<Project> &pProject) : GeneratedFileEntry(pProject) {
	setEntryChildren(Gio::ListStore<ProjectExplorerEntry>::create());
	setName("Сцены");
	auto menuAdd = Gio::Menu::create();
	menuAdd->append("3D сцена", "win.scene.add.scene3d");
	Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();
	menu->append_submenu("Добавить сцену", menuAdd);
	setContextMenu(menu);
}

void ScenesInfo::getActionGroup(const Glib::RefPtr<Gio::SimpleActionGroup> &pActionGroup) {

	pActionGroup->add_action("scene.add.scene3d", [this] {
		auto sceneInfo = SceneInfo::create(getProject());
		addChildEntry(sceneInfo);
		engine::utils::Logger::out("Test");
	});

}
}