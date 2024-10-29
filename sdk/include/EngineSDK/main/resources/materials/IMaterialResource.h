//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 22.10.24.
//

#ifndef IMATERIALRESOURCE_H
#define IMATERIALRESOURCE_H
#include "EngineSDK/main/Application.h"
#include "EngineSDK/main/render/IRenderable.h"
#include "MaterialData.h"

namespace mer::sdk::main {
class ITextureResource;
class IResource;

class IMaterialResource : public IRenderable {
public:
	~IMaterialResource() override = default;

	virtual const MaterialData &getData() = 0;

	[[nodiscard]] virtual const std::shared_ptr<ITextureResource> &getBaseColorMap() const = 0;

	utils::ReportMessagePtr virtual setBaseColorMap(const std::shared_ptr<ITextureResource> &pBaseColorMap) = 0;

	[[nodiscard]] virtual std::optional<glm::vec3> getBaseColor() = 0;

	[[nodiscard]] virtual const std::shared_ptr<ITextureResource> &getNormalMap() const = 0;

	virtual utils::ReportMessagePtr setNormalMap(const std::shared_ptr<ITextureResource> &pNormalMap) = 0;

	[[nodiscard]] virtual std::optional<glm::vec3> getNormalColor() = 0;

	[[nodiscard]] virtual const std::shared_ptr<ITextureResource> &getMetallicMap() const = 0;

	virtual utils::ReportMessagePtr setMetallicMap(const std::shared_ptr<ITextureResource> &pMetallicMap) = 0;

	[[nodiscard]] virtual std::optional<float> getMetallicColor() = 0;

	[[nodiscard]] virtual const std::shared_ptr<ITextureResource> &getRoughnessMap() const = 0;

	virtual utils::ReportMessagePtr setRoughnessMap(const std::shared_ptr<ITextureResource> &pRoughnessMap) = 0;

	[[nodiscard]] virtual std::optional<float> getRoughnessColor() = 0;

	[[nodiscard]] virtual const std::shared_ptr<ITextureResource> &getAoMap() const = 0;

	virtual utils::ReportMessagePtr setAoMap(const std::shared_ptr<ITextureResource> &pAoMap) = 0;

	[[nodiscard]] virtual std::optional<float> getAoColor() = 0;

	virtual IResource* asResource() = 0;

	virtual sigc::connection connectOnDataChangedSignal(const sigc::slot<void()> &pSlot) = 0;
};

} // namespace mer::sdk::main

#endif //IMATERIALRESOURCE_H
