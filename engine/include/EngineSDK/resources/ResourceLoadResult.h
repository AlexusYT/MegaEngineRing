//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 26.10.24.
//

#ifndef RESOURCELOADCALLBACKS_H
#define RESOURCELOADCALLBACKS_H
#include <sigc++/functors/slot.h>

#include "EngineSDK/utils/ReportMessageFwd.h"

namespace mer::sdk {
class IResource;
}

namespace mer::sdk {
class ResourceLoadResult {
public:
	enum class State { NONE = 0, ERRORED, PRELOADED, READY };

private:
	ReportMessagePtr error{};
	std::shared_ptr<IResource> resource{};
	State state{State::NONE};
	std::string requestedUri{};

	ResourceLoadResult() = default;

public:
	static std::shared_ptr<ResourceLoadResult> create();

	[[nodiscard]] const ReportMessagePtr &getError() const { return error; }

	void setError(const ReportMessagePtr &pError) {
		error = pError;
		state = State::ERRORED;
	}

	[[nodiscard]] const std::shared_ptr<IResource> &getResource() const { return resource; }

	void setResource(const std::shared_ptr<IResource> &pResource) { resource = pResource; }

	[[nodiscard]] const std::string &getRequestedUri() const { return requestedUri; }

	void setRequestedUri(const std::string &pUri) { requestedUri = pUri; }

	[[nodiscard]] State getState() const { return state; }

	std::string getStateStr() const {
		switch (state) {
			case State::ERRORED: return "ERRORED";
			case State::PRELOADED: return "PRELOADED";
			case State::READY: return "READY";
			default: return "NONE";
		}
	}

	void setState(const State pState) { state = pState; }

	bool isErrored() const { return state == State::ERRORED; }

	bool isPreLoaded() const { return state == State::PRELOADED; }

	bool isReady() const { return state == State::READY; }
};
} // namespace mer::sdk

#endif //RESOURCELOADCALLBACKS_H
