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
// Created by alexus on 22.10.24.
//

#ifndef IMATERIALRESOURCE_H
#define IMATERIALRESOURCE_H
#include "EngineSDK/main/Application.h"
#include "EngineSDK/main/render/IRenderable.h"
#include "MaterialData.h"

namespace mer::sdk::utils {
template<typename T>
class PropertyReadOnly;
}

namespace mer::sdk::main {
class IMaterialComponent;
class ITextureResource;
class IResource;

class IMaterialResource : public IRenderable {
public:
	~IMaterialResource() override = default;

	virtual const MaterialData &getData() = 0;

	[[nodiscard]] virtual utils::PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getAlbedo() = 0;

	void virtual setAlbedo(const std::shared_ptr<IMaterialComponent> &pBaseColorMap) = 0;

	[[nodiscard]] virtual utils::PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getNormal() = 0;

	virtual void setNormal(const std::shared_ptr<IMaterialComponent> &pNormalMap) = 0;

	[[nodiscard]] virtual utils::PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getMetallic() = 0;

	virtual void setMetallic(const std::shared_ptr<IMaterialComponent> &pMetallicMap) = 0;

	[[nodiscard]] virtual utils::PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getRoughness() = 0;

	virtual void setRoughness(const std::shared_ptr<IMaterialComponent> &pRoughnessMap) = 0;

	[[nodiscard]] virtual utils::PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getAo() = 0;

	virtual void setAo(const std::shared_ptr<IMaterialComponent> &pAoMap) = 0;

	virtual IResource* asResource() = 0;

	virtual sigc::connection connectOnDataChangedSignal(const sigc::slot<void(const MaterialData &pData)> &pSlot) = 0;
};

} // namespace mer::sdk::main

#endif //IMATERIALRESOURCE_H
