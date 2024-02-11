//
// Created by alexus on 31.01.24.
//

#ifndef ISCENE_H
#define ISCENE_H
#include "EngineUtils/utils/ReportMessageFwd.h"

namespace n::sdk::main {
class Resources;
class ResourceRequests;
class ResourceRequest;
} // namespace n::sdk::main

namespace n::core::mvp {
class PresenterSceneEditor;
} // namespace n::core::mvp

namespace n::sdk::main {
class IScene {
	friend class MainWindow;
	friend class core::mvp::PresenterSceneEditor;

public:
	virtual ~IScene() = default;

	virtual engine::utils::ReportMessagePtr preloadScene(const std::shared_ptr<ResourceRequests> &pRequests) = 0;

	virtual engine::utils::ReportMessagePtr initScene() = 0;
	[[nodiscard]] virtual const std::shared_ptr<Resources> &getResources() const = 0;


	virtual void onResourceLoadingError(const std::shared_ptr<ResourceRequest> &pRequest,
										const engine::utils::ReportMessagePtr &pError) = 0;

private:
	virtual void setResources(const std::shared_ptr<Resources> &pResources) = 0;

	virtual void render() = 0;
};
} // namespace n::sdk::main


#endif //ISCENE_H
