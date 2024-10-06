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
// Created by alexus on 31.01.24.
//

#ifndef RESOURCEREQUEST_H
#define RESOURCEREQUEST_H
#include <memory>

#include "ResourceLoader.h"

namespace mer::sdk::main {
class ResourceRequest {
	friend class LoadedResources;
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

class RegularRequest : public ResourceRequest {};
} // namespace mer::sdk::main

#endif //RESOURCEREQUEST_H
