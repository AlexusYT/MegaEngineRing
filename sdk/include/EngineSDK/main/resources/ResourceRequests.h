// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 01.02.24.
//

#ifndef RESOURCEREQUESTS_H
#define RESOURCEREQUESTS_H
#include <memory>
#include <unordered_map>

#include "ResourceRequest.h"

namespace mer::sdk::main {
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
} // namespace mer::sdk::main


#endif //RESOURCEREQUESTS_H
