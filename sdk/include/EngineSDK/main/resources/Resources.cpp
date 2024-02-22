//
// Created by alexus on 01.02.24.
//

#include "Resources.h"

#include "LazyResource.h"

namespace n::sdk::main {
bool Resources::isLazyResource(const std::string &pName) const { return get<LazyResource>(pName) != nullptr; }

std::shared_ptr<LazyResource> Resources::getLazyResource(const std::string &pName) const {
	return get<LazyResource>(pName);
}

} // namespace n::sdk::main