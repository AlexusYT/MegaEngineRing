//
// Created by alexus on 02.02.24.
//

#include "Scene.h"

#include "EngineUtils/utils/Logger.h"

void n::sdk::main::Scene::onResourceLoadingError(const std::shared_ptr<ResourceRequest> & /*pRequest*/,
												 const engine::utils::ReportMessagePtr &pError) {

	engine::utils::Logger::error(pError);
}