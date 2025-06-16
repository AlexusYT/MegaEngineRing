//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 26.02.25.
//

#include "EngineSDK/gltf/Texture.h"

#include <epoxy/gl.h>

#include "EngineSDK/gltf/Image.h"
#include "EngineSDK/gltf/Sampler.h"

namespace mer::sdk {
Texture::Texture(const Microsoft::glTF::Texture & /*pTexture*/, const std::shared_ptr<Image> &pImage,
				 const std::shared_ptr<Sampler> &pSampler) : image(pImage), sampler(pSampler) {}

std::shared_ptr<Texture> Texture::create(const Microsoft::glTF::Texture &pTexture, const std::shared_ptr<Image> &pImage,
										 const std::shared_ptr<Sampler> &pSampler) {
	return std::shared_ptr<Texture>(new Texture(pTexture, pImage, pSampler));
}

ReportMessagePtr Texture::onInitialize() {
	if (sampler)
		if (auto msg = sampler->initialize()) return msg;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
	auto format = image->getFormat();
	auto bitDepth = image->getType();
	/*GLenum internalFormat = GL_RGB8UI;
	if (bitDepth == Texture2DType::UNSIGNED_BYTE) {
		if (format == Texture2DImageFormat::RGBA) internalFormat = GL_RGBA8UI;
		if (format == Texture2DImageFormat::RGB) internalFormat = GL_RGB8UI;
		if (format == Texture2DImageFormat::RG) internalFormat = GL_RG8UI;
		if (format == Texture2DImageFormat::RED) internalFormat = GL_R8UI;
	}
	if (bitDepth == Texture2DType::BYTE) {
		if (format == Texture2DImageFormat::RGBA) internalFormat = GL_RGBA8I;
		if (format == Texture2DImageFormat::RGB) internalFormat = GL_RGB8I;
		if (format == Texture2DImageFormat::RG) internalFormat = GL_RG8I;
		if (format == Texture2DImageFormat::RED) internalFormat = GL_R8I;
	}
	if (bitDepth == Texture2DType::UNSIGNED_SHORT) {
		if (format == Texture2DImageFormat::RGBA) internalFormat = GL_RGBA16UI;
		if (format == Texture2DImageFormat::RGB) internalFormat = GL_RGB16UI;
		if (format == Texture2DImageFormat::RG) internalFormat = GL_RG16UI;
		if (format == Texture2DImageFormat::RED) internalFormat = GL_R16UI;
	}
	if (bitDepth == Texture2DType::SHORT) {
		if (format == Texture2DImageFormat::RGBA) internalFormat = GL_RGBA16I;
		if (format == Texture2DImageFormat::RGB) internalFormat = GL_RGB16I;
		if (format == Texture2DImageFormat::RG) internalFormat = GL_RG16I;
		if (format == Texture2DImageFormat::RED) internalFormat = GL_R16I;
	}
	if (bitDepth == Texture2DType::UNSIGNED_INT) {
		if (format == Texture2DImageFormat::RGBA) internalFormat = GL_RGBA32UI;
		if (format == Texture2DImageFormat::RGB) internalFormat = GL_RGB32UI;
		if (format == Texture2DImageFormat::RG) internalFormat = GL_RG32UI;
		if (format == Texture2DImageFormat::RED) internalFormat = GL_R32UI;
	}
	if (bitDepth == Texture2DType::INT) {
		if (format == Texture2DImageFormat::RGBA) internalFormat = GL_RGBA32I;
		if (format == Texture2DImageFormat::RGB) internalFormat = GL_RGB32I;
		if (format == Texture2DImageFormat::RG) internalFormat = GL_RG32I;
		if (format == Texture2DImageFormat::RED) internalFormat = GL_R32I;
	}*/
	const auto width = image->getWidth();
	const auto height = image->getHeight();
	const auto &data = image->getData();
	glTextureStorage2D(textureId, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(textureId, 0, 0, 0, width, height, static_cast<GLenum>(format), static_cast<GLenum>(bitDepth),
						data.data());
	glGenerateTextureMipmap(textureId);
	if (sampler) bindlessHandle = glGetTextureSamplerHandleARB(textureId, sampler->getSamplerId());
	else {
		//When sampler undefined, a sampler with repeat wrapping and auto filtering SHOULD be used.
		glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
		bindlessHandle = glGetTextureHandleARB(textureId);
	}
	glMakeTextureHandleResidentARB(bindlessHandle);
	return Initializable::onInitialize();
}

void Texture::onUninitialize() {
	glDeleteTextures(1, &textureId);
	Initializable::onUninitialize();
}
} // namespace mer::sdk
