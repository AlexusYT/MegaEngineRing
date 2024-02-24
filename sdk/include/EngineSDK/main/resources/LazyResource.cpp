//
// Created by alexus on 02.02.24.
//

#include "LazyResource.h"

#include "EngineUtils/utils/Logger.h"
#include "EngineUtils/utils/ReportMessage.h"
#include "ResourceRequest.h"
#include "Resources.h"

namespace mer::sdk::main {
void LazyResource::getAsync(const sigc::slot<void(std::shared_ptr<IResource> pResource)> &pSlot) {
	std::thread([pSlot, this] { pSlot(getSync()); }).detach();
}

std::shared_ptr<IResource> LazyResource::getSync() {
	if (isLoaded()) return resource;
	auto error = sdk::utils::ReportMessage::create();
	if (const auto res = request->getLoader()->load(request, error, dependencies)) {
		resource = res;
		dependencies->clear();

	} else {
		dependencies->clear();
		scene->onResourceLoadingError(request, error);
		return nullptr;
	}
	return resource;
}

} // namespace mer::sdk::main