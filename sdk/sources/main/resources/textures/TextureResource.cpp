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

#include "EngineSDK/main/resources/textures/TextureResource.h"

#include <cstring>
#include <epoxy/gl.h>

#include "EngineSDK/main/resources/textures/Texture2DImageFormat.h"
#include "EngineSDK/main/resources/textures/Texture2DType.h"
#include "EngineSDK/main/resources/textures/TextureMagFilter.h"
#include "EngineSDK/main/resources/textures/TextureMinFilter.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk::main {
TextureResource::TextureResource()
	: minFilter(TextureMinFilter::LINEAR), magFilter(TextureMagFilter::LINEAR),
	  textureHandle(nullptr, "TextureHandle") {}

std::shared_ptr<TextureResource> TextureResource::create() {
	return std::shared_ptr<TextureResource>(new TextureResource());
}

TextureResource::~TextureResource() {
	TextureResource::destroyRender();
	if (data) free(data);
}

void TextureResource::setupRender() {
	//if (inited) return;
	/*glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, GL_RGBA8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glTextureSubImage2D(id,
						// level, xoffset, yoffset, width, height
						0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLenum>(format),
						static_cast<GLenum>(type), data);
	glGenerateTextureMipmap(id);*/
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	//glActiveTexture(GL_TEXTURE0 + textureBlock);
	glTexImage2D(GL_TEXTURE_2D, mipmapLevel, static_cast<GLint>(format), static_cast<GLsizei>(width),
				 static_cast<GLsizei>(height), 0, static_cast<GLenum>(format), static_cast<GLenum>(type), data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*static_cast<GLint>(minFilter)*/ GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));

	auto handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);
	textureHandle = handle;
	glBindTexture(GL_TEXTURE_2D, 0);
	inited = true;
}

void TextureResource::render() {}

void TextureResource::destroyRender() {
	if (id) glDeleteTextures(1, &id);
}

void TextureResource::setData(void* pData, uint32_t pWidth, uint32_t pHeight, Texture2DImageFormat pFormat,
							  Texture2DType pType) {
	width = pWidth;
	height = pHeight;
	format = pFormat;
	type = pType;
	size_t components{};

	switch (pFormat) {
		case Texture2DImageFormat::RED: components = 1; break;
		case Texture2DImageFormat::RG: components = 2; break;
		case Texture2DImageFormat::BGR:
		case Texture2DImageFormat::RGB: components = 3; break;
		case Texture2DImageFormat::RGBA:
		case Texture2DImageFormat::BGRA: components = 4; break;
		default: throw std::runtime_error("Unknown texture image format");
	}
	size_t bytesCount{};

	switch (pType) {

		case Texture2DType::UNSIGNED_BYTE:
		case Texture2DType::BYTE: bytesCount = 1; break;
		case Texture2DType::UNSIGNED_SHORT:
		case Texture2DType::SHORT: bytesCount = 2; break;
		case Texture2DType::UNSIGNED_INT:
		case Texture2DType::INT: bytesCount = 4; break;
		case Texture2DType::FLOAT: bytesCount = 4; break;
		default: throw std::runtime_error("Unknown texture pixel type");
	}

	size_t size = width * height * bytesCount * components;
	if (data) free(data);
	data = malloc(size);
	memcpy(data, pData, size);
}
} // namespace mer::sdk::main