//
// Created by alexus on 01.02.24.
//

#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H
#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::sdk::main {
class IResource;
class ResourceRequest;
class Resources;

class ResourceLoader {
public:
	virtual ~ResourceLoader() = default;
	virtual std::shared_ptr<IResource> load(const std::shared_ptr<ResourceRequest> &pRequest,
											const sdk::utils::ReportMessagePtr &pError,
											const std::shared_ptr<Resources> &pDependencies) = 0;
};
} // namespace mer::sdk::main


#endif //RESOURCELOADER_H
