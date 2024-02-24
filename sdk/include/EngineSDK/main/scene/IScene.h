//
// Created by alexus on 31.01.24.
//

#ifndef ISCENE_H
#define ISCENE_H
#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::sdk::main {
class Resources;
class ResourceRequests;
class ResourceRequest;
} // namespace mer::sdk::main

namespace mer::editor::mvp {
class PresenterSceneEditor;
} // namespace mer::editor::mvp

namespace mer::sdk::main {
class IScene {
	friend class MainWindow;
	friend class editor::mvp::PresenterSceneEditor;

public:
	virtual ~IScene() = default;

	virtual sdk::utils::ReportMessagePtr preloadScene(const std::shared_ptr<ResourceRequests> &pRequests) = 0;

	virtual sdk::utils::ReportMessagePtr initScene() = 0;
	[[nodiscard]] virtual const std::shared_ptr<Resources> &getResources() const = 0;


	virtual void onResourceLoadingError(const std::shared_ptr<ResourceRequest> &pRequest,
										const sdk::utils::ReportMessagePtr &pError) = 0;

private:
	virtual void setResources(const std::shared_ptr<Resources> &pResources) = 0;

	virtual void render() = 0;

	virtual void resize(int pWidth, int pHeight) = 0;
};
} // namespace mer::sdk::main


#endif //ISCENE_H
