//
// Created by alexus on 28.01.24.
//

#ifndef SCENEINFO_H
#define SCENEINFO_H
#include "GeneratedFileEntry.h"
class Scene;

namespace PROJECT_CORE {

class SceneInfo : public GeneratedFileEntry {
	std::shared_ptr<Scene> scene;

protected:
	explicit SceneInfo(const std::shared_ptr<Project> &pProject) : GeneratedFileEntry(pProject) { setName("Сцены"); }

public:
	static std::shared_ptr<SceneInfo> create(const std::shared_ptr<Project> &pProject) {
		return Glib::make_refptr_for_instance(new SceneInfo(pProject));
	}
};

} // namespace PROJECT_CORE

#endif //SCENEINFO_H
