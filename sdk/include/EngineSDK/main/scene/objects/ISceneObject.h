//
// Created by alexus on 12.02.24.
//

#ifndef ISCENEOBJECT_H
#define ISCENEOBJECT_H
#include <EngineUtils/utils/ReportMessageFwd.h>

namespace mer::sdk::main {
class ResourceRequests;
class Resources;

class ISceneObject {
public:
	virtual ~ISceneObject() = default;

	virtual void fillResourceRequests(const std::shared_ptr<ResourceRequests> &pRequests) = 0;

	[[nodiscard]] virtual const std::shared_ptr<Resources> &getResources() const = 0;

	virtual void setResources(const std::shared_ptr<Resources> &pResources) = 0;

	virtual sdk::utils::ReportMessagePtr init() = 0;

	virtual void render() = 0;
};

} // namespace mer::sdk::main

#endif //ISCENEOBJECT_H
