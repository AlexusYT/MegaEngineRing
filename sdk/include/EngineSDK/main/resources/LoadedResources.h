//
// Created by alexus on 31.01.24.
//

#ifndef LOADEDRESOURCES_H
#define LOADEDRESOURCES_H
#include <unordered_map>
#include <unordered_set>

#include "Resources.h"

namespace mer::sdk::main {
class IScene;
enum class ResourceLoadingPolicy;
class ResourceRequest;
class IResource;
class ResourceRequests;

class ILoadedResources {
public:
	virtual ~ILoadedResources() = default;
	virtual std::shared_ptr<Resources> executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
													   const std::shared_ptr<IScene> &pScene) = 0;
};

class LoadedResources : public ILoadedResources {
	std::unordered_map<std::string, std::shared_ptr<IResource>> resources;
	std::vector<std::string> processingRequests;


	LoadedResources();

public:
	static std::shared_ptr<ILoadedResources> create();

	~LoadedResources() override = default;
	std::shared_ptr<Resources> executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
											   const std::shared_ptr<IScene> &pScene) override;

private:
	std::shared_ptr<IResource> getResource(const std::string &pName) const;

	std::shared_ptr<IResource> processRequest(const std::shared_ptr<ResourceRequest> &pRequest,
											  const std::shared_ptr<IScene> &pScene);

	std::shared_ptr<IResource> getLoadedResourceByRequest(const std::shared_ptr<ResourceRequest> &pRequests,
														  const std::shared_ptr<IScene> &pScene) const;

	static std::shared_ptr<IResource> executeRequest(const std::shared_ptr<Resources> &pDependencies,
													 const std::shared_ptr<ResourceRequest> &pRequest);
};

} // namespace mer::sdk::main


#endif //LOADEDRESOURCES_H
