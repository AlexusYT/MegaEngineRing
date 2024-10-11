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
// Created by alexus on 27.02.24.
//

#ifndef IRESOURCES_H
#define IRESOURCES_H
#include <sigc++/functors/slot.h>

#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::sdk::main {
class ILoadedResources;
class IApplication;
class ResourceRequest;
class IResource;

class IResourceLoadExecutor {
public:
	using LoadingFinishedSlot =
		sigc::slot<void(const std::shared_ptr<IResource> &pResource, const utils::ReportMessagePtr &pError)>;

	virtual ~IResourceLoadExecutor() = default;


	virtual std::pair<std::shared_ptr<IResource>, utils::ReportMessagePtr> loadResourceSync(
		const std::string &pResourceUri) = 0;

	virtual void loadResourceAsync(const std::string &pResourceUri, const LoadingFinishedSlot &pSlot) = 0;

	[[nodiscard]] virtual IApplication* getApplication() const = 0;

	virtual void setApplication(IApplication* pApplication) = 0;

	virtual const std::shared_ptr<ILoadedResources> &getResources() = 0;
};
} // namespace mer::sdk::main


#endif //IRESOURCES_H