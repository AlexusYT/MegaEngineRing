//
// Created by alexus on 28.01.24.
//

#ifndef SCENEINFO_H
#define SCENEINFO_H
#include <EngineUtils/utils/UUID.h>

#include "GeneratedFileEntry.h"

namespace mer::editor::project {
class CppClass;
class CppExternC;
} // namespace mer::editor::project
class Scene;

namespace mer::editor::project {
class CppMethod;

class SceneInfo : public GeneratedFileEntry {
	std::shared_ptr<Scene> scene;
	std::shared_ptr<UUID> uuid;
	bool primaryScene{true};

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

private:
	sdk::utils::ReportMessagePtr onLoadDatabase() override;

	sdk::utils::ReportMessagePtr onSaveDatabase() const override;

	sdk::utils::ReportMessagePtr onSaveFile() const override;

	sdk::utils::ReportMessagePtr createTable();


	sdk::utils::ReportMessagePtr writeFile() const;

	std::filesystem::path getHeaderPath() const override { return "source/scenes/" + getName() + ".h"; }

	std::filesystem::path getSourcePath() const override { return "source/scenes/" + getName() + ".cpp"; }

	static std::shared_ptr<CppExternC> createExternCBlock(const std::string &pSceneName);

	static std::shared_ptr<CppMethod> createInitMethod(const std::shared_ptr<CppClass> &pClass);
};

} // namespace mer::editor::project

#endif //SCENEINFO_H
