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
// Created by alexus on 12.10.24.
//

#ifndef ITEXTURERESOURCE_H
#define ITEXTURERESOURCE_H
#include "EngineSDK/main/render/IRenderable.h"

namespace mer::sdk::main {
enum class Texture2DType;
enum class Texture2DImageFormat;
enum class TextureBaseInternalFormat;
enum class TextureMagFilter;
enum class TextureMinFilter;
class IResource;

class ITextureResource : public IRenderable {
public:
	[[nodiscard]] virtual void* getData() const = 0;

	virtual void setData(void* pData, uint32_t pWidth, uint32_t pHeight, Texture2DImageFormat pFormat,
						 Texture2DType pType) = 0;

	[[nodiscard]] virtual uint32_t getWidth() const = 0;

	[[nodiscard]] virtual uint32_t getHeight() const = 0;

	[[nodiscard]] virtual int32_t getMipmapLevel() const = 0;

	virtual void setMipmapLevel(int32_t pMipmapLevel) = 0;

	[[nodiscard]] virtual TextureMinFilter getMinFilter() const = 0;

	virtual void setMinFilter(TextureMinFilter pMinFilter) = 0;

	[[nodiscard]] virtual TextureMagFilter getMagFilter() const = 0;

	virtual void setMagFilter(TextureMagFilter pMagFilter) = 0;

	[[nodiscard]] virtual Texture2DImageFormat getFormat() const = 0;

	[[nodiscard]] virtual Texture2DType getType() const = 0;

	[[nodiscard]] virtual uint32_t getTextureBlock() const = 0;

	virtual void setTextureBlock(uint32_t pTextureBlock) = 0;

	virtual IResource* asResource() = 0;
};

} // namespace mer::sdk::main

#endif //ITEXTURERESOURCE_H
