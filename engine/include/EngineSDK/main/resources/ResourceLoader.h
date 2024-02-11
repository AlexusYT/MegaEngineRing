//
// Created by alexus on 01.02.24.
//

#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H
#include "EngineUtils/utils/ReportMessageFwd.h"

namespace n::sdk::main {
class IResource;
class ResourceRequest;
class Resources;

class ResourceLoader {
public:
	virtual ~ResourceLoader() = default;
	virtual std::shared_ptr<IResource> load(const std::shared_ptr<ResourceRequest> &pRequest,
											const engine::utils::ReportMessagePtr &pError,
											const std::shared_ptr<Resources> &pDependencies) = 0;
};
} // namespace n::sdk::main


#endif //RESOURCELOADER_H
