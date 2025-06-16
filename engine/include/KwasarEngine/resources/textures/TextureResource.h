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
// Created by alexus on 12.10.24.
//

#ifndef TEXTURERESOURCE_H
#define TEXTURERESOURCE_H

#include <glm/vec4.hpp>

#include "KwasarEngine/resources/Resource.h"
#include "KwasarEngine/resources/ResourceType.h"
#include "KwasarEngine/utils/Property.h"
#include "ITextureResource.h"

namespace mer::sdk {
class TextureResource : public ITextureResource, public Resource {
	uint32_t id{};
	void* data{};
	uint32_t width{};
	uint32_t height{};
	int32_t mipmapLevel{};
	TextureMinFilter minFilter;
	TextureMagFilter magFilter;
	Texture2DImageFormat format{};
	Texture2DType type{};
	uint32_t textureBlock{};
	Property<uint64_t> textureHandle;
	Property<glm::vec4> materialComponentVal;

	TextureResource();

public:
	static std::shared_ptr<TextureResource> create();

	~TextureResource() override;

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

public:
	void render() override;

	[[nodiscard]] void* getData() const override { return data; }

	void setData(void* pData, uint32_t pWidth, uint32_t pHeight, Texture2DImageFormat pFormat,
				 Texture2DType pType) override;

	[[nodiscard]] uint32_t getWidth() const override { return width; }

	[[nodiscard]] uint32_t getHeight() const override { return height; }

	[[nodiscard]] int32_t getMipmapLevel() const override { return mipmapLevel; }

	void setMipmapLevel(int32_t pMipmapLevel) override { mipmapLevel = pMipmapLevel; }

	[[nodiscard]] TextureMinFilter getMinFilter() const override { return minFilter; }

	void setMinFilter(TextureMinFilter pMinFilter) override { minFilter = pMinFilter; }

	[[nodiscard]] TextureMagFilter getMagFilter() const override { return magFilter; }

	void setMagFilter(TextureMagFilter pMagFilter) override { magFilter = pMagFilter; }

	[[nodiscard]] Texture2DImageFormat getFormat() const override { return format; }

	[[nodiscard]] Texture2DType getType() const override { return type; }

	[[nodiscard]] uint32_t getTextureBlock() const override { return textureBlock; }

	void setTextureBlock(uint32_t pTextureBlock) override { textureBlock = pTextureBlock; }

	IResource* asResource() override { return this; }

	ResourceType getResourceType() override { return ResourceType::TEXTURE; }

	[[nodiscard]] PropertyReadOnly<uint64_t> getTextureHandle() override { return textureHandle.getReadOnly(); }

	PropertyReadOnly<glm::vec4> getComponentValueProperty() override { return materialComponentVal.getReadOnly(); }

private:
	static glm::vec4 handleToVec(uint64_t pHandle);
};
} // namespace mer::sdk

#endif //TEXTURERESOURCE_H
