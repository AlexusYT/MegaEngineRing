//
// Created by alexus on 06.02.24.
//

#ifndef MODELSCENEEDITOR_H
#define MODELSCENEEDITOR_H
#include "IModelSceneEditor.h"

namespace MVP_CORE {
class ModelSceneEditor : public IModelSceneEditor {
	std::shared_ptr<project::Project> project;
	project::SceneInfo* sceneInfo{};

	std::shared_ptr<sdk::main::IScene> scene;

public:
	[[nodiscard]] const std::shared_ptr<project::Project> &getProject() const override { return project; }

	void setProject(const std::shared_ptr<project::Project> &pProject) override { project = pProject; }

	[[nodiscard]] project::SceneInfo* getSceneInfo() const override { return sceneInfo; }

	void setSceneInfo(project::SceneInfo* const pSceneInfo) override { sceneInfo = pSceneInfo; }

	[[nodiscard]] const std::shared_ptr<sdk::main::IScene> &getScene() const override { return scene; }

	void setScene(const std::shared_ptr<sdk::main::IScene> &pScene) override { scene = pScene; }
};
} // namespace MVP_CORE
#endif //MODELSCENEEDITOR_H
