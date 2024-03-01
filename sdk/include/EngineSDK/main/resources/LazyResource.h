//
// Created by alexus on 02.02.24.
//

#ifndef LAZYRESOURCE_H
#define LAZYRESOURCE_H
#include <future>
#include <sigc++/functors/slot.h>

#include "IResource.h"

namespace mer::sdk::main {
class ResourceRequest;
class Resources;
} // namespace mer::sdk::main

namespace mer::sdk::main {
class LazyResource : public IResource {
	friend class LoadedResources;
	std::shared_ptr<ResourceRequest> request;
	std::shared_ptr<IResource> resource;
	std::shared_ptr<Resources> dependencies;

protected:
	LazyResource(const std::shared_ptr<ResourceRequest> &pRequest, const std::shared_ptr<Resources> &pDependencies)
		: request(pRequest), dependencies(pDependencies) {}

public:
	[[nodiscard]] bool isLoaded() const { return resource != nullptr; }

	template<typename ClassT>
	void getAsync(const sigc::slot<void(std::shared_ptr<ClassT> pResource)> &pSlot) {
		std::thread([pSlot, this] { pSlot(getSync<ClassT>()); }).detach();
	}

	void getAsync(const sigc::slot<void(std::shared_ptr<IResource> pResource)> &pSlot);

	template<typename ClassT>
	std::shared_ptr<ClassT> getSync() {
		return std::dynamic_pointer_cast<ClassT>(getSync());
	}

	std::shared_ptr<IResource> getSync();

	template<typename ClassT>
	std::future<std::shared_ptr<ClassT>> get() {
		return std::async(std::launch::async, [this] { return getSync<ClassT>(); });
	}

	std::future<std::shared_ptr<IResource>> get() {
		return std::async(std::launch::async, [this] { return getSync(); });
	}
};
} // namespace mer::sdk::main


#endif //LAZYRESOURCE_H
