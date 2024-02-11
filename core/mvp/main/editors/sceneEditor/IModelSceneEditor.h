//
// Created by alexus on 06.02.24.
//

#ifndef IMODELSCENEEDITOR_H
#define IMODELSCENEEDITOR_H

namespace n::sdk::main {
class IScene;
}

namespace n::core::project {
class Project;
class SceneInfo;
} // namespace n::core::project

namespace MVP_CORE {
class IModelSceneEditor {
public:
	virtual ~IModelSceneEditor() = default;
	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;

	[[nodiscard]] virtual project::SceneInfo* getSceneInfo() const = 0;

	virtual void setSceneInfo(project::SceneInfo* const pSceneInfo) = 0;

	[[nodiscard]] virtual const std::shared_ptr<sdk::main::IScene> &getScene() const = 0;

	virtual void setScene(const std::shared_ptr<sdk::main::IScene> &pScene) = 0 ;
};
} // namespace MVP_CORE


#endif //IMODELSCENEEDITOR_H
