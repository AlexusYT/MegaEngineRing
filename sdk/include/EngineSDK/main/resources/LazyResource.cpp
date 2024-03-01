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
		utils::Logger::error(error);
		//scene->onResourceLoadingError(request, error);
		return nullptr;
	}
	return resource;
}

} // namespace mer::sdk::main