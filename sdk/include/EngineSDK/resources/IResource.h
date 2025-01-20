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
// Created by alexus on 01.02.24.
//

#ifndef IRESOURCE_H
#define IRESOURCE_H
#include <filesystem>

#include "EngineUtils/utils/IReportable.h"


class UUID;

namespace mer::sdk::main {
enum class ResourceType;

class IResource : public utils::IReportable {
protected:
	~IResource() override = default;

public:
	virtual const std::filesystem::path &getResourceUri() = 0;

	[[nodiscard]] virtual const std::string &getResourceName() const = 0;

	virtual void setResourceUri(const std::filesystem::path &pResourceUri) = 0;

	[[nodiscard]] virtual const std::shared_ptr<UUID> &getUuid() const = 0;

	virtual void setUuid(const std::shared_ptr<UUID> &pUuid) = 0;

	virtual ResourceType getResourceType() = 0;

	[[nodiscard]] virtual bool isInited() const = 0;

	[[nodiscard]] virtual bool isIncomplete() const = 0;

	virtual void setIncomplete(bool pIncomplete) = 0;
};

} // namespace mer::sdk::main

#endif //IRESOURCE_H
