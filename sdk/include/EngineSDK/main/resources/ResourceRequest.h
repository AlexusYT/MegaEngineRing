//
// Created by alexus on 31.01.24.
//

#ifndef RESOURCEREQUEST_H
#define RESOURCEREQUEST_H
#include <memory>

#include "ResourceLoader.h"

namespace mer::sdk::main {
class ResourceRequest {
	friend class LoadedResources;
	friend class LazyResource;
	std::vector<std::shared_ptr<ResourceRequest>> required;

protected:
	ResourceRequest() = default;

	virtual ~ResourceRequest() = default;

public:
	[[nodiscard]] virtual std::string getName() const = 0;

	void setRequired(const std::vector<std::shared_ptr<ResourceRequest>> &pRequired) { required = pRequired; }

	template<typename... Requests>
	void setRequired(Requests... pRequests) {
		required = {pRequests...};
	}


private:
	[[nodiscard]] virtual std::shared_ptr<ResourceLoader> getLoader() const = 0;

	[[nodiscard]] const std::vector<std::shared_ptr<ResourceRequest>> &getRequired() const { return required; }
};

class LazyRequest : public ResourceRequest {};

class RegularRequest : public ResourceRequest {};
} // namespace mer::sdk::main

#endif //RESOURCEREQUEST_H
