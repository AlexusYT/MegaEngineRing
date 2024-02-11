//
// Created by alexus on 01.02.24.
//

#ifndef RESOURCEREQUESTS_H
#define RESOURCEREQUESTS_H
#include <memory>
#include <unordered_map>

#include "ResourceRequest.h"

namespace n::sdk::main {
class ResourceRequest;

class ResourceRequests {
	std::unordered_map<std::string, std::shared_ptr<ResourceRequest>> requests;

public:
	template<typename ClassT, typename... Args>
	std::shared_ptr<ClassT> addRequest(Args... pArgs) {
		auto req = std::make_shared<ClassT>(pArgs...);
		addRequestWithPolicy(req);
		return req;
	}

	std::shared_ptr<ResourceRequest> addRequest(const std::shared_ptr<ResourceRequest> &pRequest) {
		return requests.emplace(pRequest->getName(), std::move(pRequest)).first->second;
	}

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<ResourceRequest>> &getRequests() const {
		return requests;
	}

	void clearRequests() { requests.clear(); }
};
} // namespace n::sdk::main


#endif //RESOURCEREQUESTS_H
