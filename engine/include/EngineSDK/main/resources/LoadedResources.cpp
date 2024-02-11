//
// Created by alexus on 31.01.24.
//

#include "LoadedResources.h"

#include "EngineSDK/main/scene/IScene.h"
#include "EngineUtils/utils/Logger.h"
#include "EngineUtils/utils/ReportMessage.h"
#include "LazyResource.h"
#include "ResourceRequest.h"
#include "ResourceRequests.h"
#include "Resources.h"

namespace n::sdk::main {
LoadedResources::LoadedResources() {}

std::shared_ptr<ILoadedResources> LoadedResources::create() {
	return std::shared_ptr<ILoadedResources>(new LoadedResources);
}

std::shared_ptr<Resources> LoadedResources::executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
															const std::shared_ptr<IScene> &pScene) {
	auto resourcesOut = std::make_shared<Resources>();
	for (auto &requests = pRequests->getRequests(); const auto &[name, request]: requests) {
		processingRequests.clear();
		if (auto res = processRequest(request, pScene)) { resourcesOut->resources.emplace(name, res); }
	}
	return resourcesOut;
}

std::shared_ptr<IResource> LoadedResources::getResource(const std::string &pName) const {
	if (const auto iter = resources.find(pName); iter != resources.end()) return iter->second;
	return nullptr;
}

std::shared_ptr<IResource> LoadedResources::processRequest(const std::shared_ptr<ResourceRequest> &pRequest,
														   const std::shared_ptr<IScene> &pScene) {

	if (auto res = getLoadedResourceByRequest(pRequest, pScene)) { return res; }

	if (std::ranges::find(processingRequests, pRequest->getName()) != processingRequests.end()) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to process request");
		msg->setMessage("Cyclic dependencies have been found");
		for (auto reqIter = processingRequests.begin(); reqIter != processingRequests.end(); ++reqIter) {
			auto iterNext = reqIter;
			std::advance(iterNext, 1);
			if (iterNext == processingRequests.end())
				msg->addInfoLine("Request {} required {}", *reqIter, pRequest->getName());
			else
				msg->addInfoLine("Request {} required {}", *reqIter, *iterNext);
		}
		pScene->onResourceLoadingError(pRequest, msg);
		return nullptr;
	}
	processingRequests.emplace_back(pRequest->getName());

	std::shared_ptr<Resources> dependencies = std::make_shared<Resources>();
	for (const auto &resourceRequest: pRequest->getRequired()) {
		if (auto result = processRequest(resourceRequest, pScene)) {
			dependencies->resources.emplace(resourceRequest->getName(), result);
			resources.emplace(resourceRequest->getName(), result);
		}
	}

	if (std::dynamic_pointer_cast<RegularRequest>(pRequest)) {
		if (auto resource = executeRequest(dependencies, pRequest)) {

			resources.emplace(pRequest->getName(), resource);
			return resource;
		}
		return nullptr;
	}

	return std::shared_ptr<LazyResource>(new LazyResource(pRequest, pScene, dependencies));
}

std::shared_ptr<IResource> LoadedResources::getLoadedResourceByRequest(const std::shared_ptr<ResourceRequest> &pRequest,
																	   const std::shared_ptr<IScene> &pScene) const {

	auto res = getResource(pRequest->getName());
	if (!res) return nullptr;
	if (std::dynamic_pointer_cast<LazyResource>(res)) {
		if (std::dynamic_pointer_cast<RegularRequest>(pRequest)) {
			const auto msg = engine::utils::ReportMessage::create();
			msg->setTitle("Resource loading policy mismatch");
			msg->setMessage("Resource previously loaded with LAZY policy, but trying to load with REGULAR");
			msg->addInfoLine("DO NOT CHANGE POLICY FOR SINGLE RESOURCE");
			msg->addInfoLine("Resource added as LAZY");
			msg->addInfoLine("Resource name: {}", pRequest->getName());
			pScene->onResourceLoadingError(pRequest, msg);
		}
		return res;
	}
	if (std::dynamic_pointer_cast<LazyRequest>(pRequest)) {
		const auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Resource loading policy mismatch");
		msg->setMessage("Resource previously loaded with REGULAR policy, but trying to load with LAZY");
		msg->addInfoLine("DO NOT CHANGE POLICY FOR SINGLE RESOURCE");
		msg->addInfoLine("Resource added as IMMEDIATELY");
		msg->addInfoLine("Resource name: {}", pRequest->getName());
		pScene->onResourceLoadingError(pRequest, msg);
	}
	return res;
}

std::shared_ptr<IResource> LoadedResources::executeRequest(const std::shared_ptr<Resources> &pDependencies,
														   const std::shared_ptr<ResourceRequest> &pRequest) {

	auto error = engine::utils::ReportMessage::create();
	if (auto resource = pRequest->getLoader()->load(pRequest, error, pDependencies)) { return resource; }
	engine::utils::Logger::error(error);
	return nullptr;
}
} // namespace n::sdk::main