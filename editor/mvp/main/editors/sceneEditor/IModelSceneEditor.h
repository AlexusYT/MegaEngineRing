//
// Created by alexus on 06.02.24.
//

#ifndef IMODELSCENEEDITOR_H
#define IMODELSCENEEDITOR_H

namespace mer::sdk::main {
class IScene;
}

namespace mer::editor::project {
class Project;
class SceneInfo;
} // namespace mer::editor::project

namespace mer::editor::mvp {
class IModelSceneEditor {
public:
	virtual ~IModelSceneEditor() = default;
	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;

	[[nodiscard]] virtual project::SceneInfo* getSceneInfo() const = 0;

	virtual void setSceneInfo(project::SceneInfo* const pSceneInfo) = 0;

	[[nodiscard]] virtual const std::shared_ptr<sdk::main::IScene> &getScene() const = 0;

	virtual void setScene(const std::shared_ptr<sdk::main::IScene> &pScene) = 0;

	[[nodiscard]] virtual bool isInteractive() const = 0;

	virtual void setInteractive(bool pInteractive) = 0;
};
} // namespace mer::editor::mvp


#endif //IMODELSCENEEDITOR_H
