//
// Created by alexus on 06.02.24.
//

#ifndef MODELSCENEEDITOR_H
#define MODELSCENEEDITOR_H
#include "IModelSceneEditor.h"

namespace mer::editor::mvp {
class ModelSceneEditor : public IModelSceneEditor {
	std::shared_ptr<project::Project> project;
	project::SceneInfo* sceneInfo{};

	std::shared_ptr<sdk::main::IScene> scene;
	bool interactive{};

public:
	[[nodiscard]] const std::shared_ptr<project::Project> &getProject() const override { return project; }

	void setProject(const std::shared_ptr<project::Project> &pProject) override { project = pProject; }

	[[nodiscard]] project::SceneInfo* getSceneInfo() const override { return sceneInfo; }

	void setSceneInfo(project::SceneInfo* const pSceneInfo) override { sceneInfo = pSceneInfo; }

	[[nodiscard]] const std::shared_ptr<sdk::main::IScene> &getScene() const override { return scene; }

	void setScene(const std::shared_ptr<sdk::main::IScene> &pScene) override { scene = pScene; }

	[[nodiscard]] bool isInteractive() const override { return interactive; }

	void setInteractive(const bool pInteractive) override { interactive = pInteractive; }
};
} // namespace mer::editor::mvp
#endif //MODELSCENEEDITOR_H
