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

#include "TextureResourceSaver.h"

#include <png++/image.hpp>

#include "EngineSDK/main/resources/IResource.h"
#include "EngineSDK/main/resources/textures/ITextureResource.h"
#include "EngineSDK/main/resources/textures/Texture2DImageFormat.h"

namespace mer::editor::mvp {

template<typename PixelType>
void write_data(const uint32_t pWidth, const uint32_t pHeight, void* pData, std::ostream &pStream) {
	png::image<PixelType> img;
	img.resize(pWidth, pHeight);
	auto &pixelBuffer = img.get_pixbuf();
	PixelType* pixels = static_cast<PixelType*>(pData);
	for (uint32_t x = 0; x < pHeight; ++x) {
		memcpy(pixelBuffer.get_row(x).data(), &pixels[static_cast<size_t>(x) * pWidth],
			   static_cast<uint32_t>(pWidth * sizeof(PixelType)));
	}
	img.write_stream(pStream);
}

sdk::utils::ReportMessagePtr TextureResourceSaver::saveToFile(
	const std::filesystem::path &pPath, const std::shared_ptr<sdk::main::ITextureResource> &pTexture) {

	auto dir = pPath.parent_path();
	if (!exists(dir)) { create_directories(dir); }
	auto resource = pTexture->asResource();
	auto resourceUri = resource->getResourceUri();
	try {
		std::ofstream file(pPath, std::ios::out | std::ios::binary);
		file.exceptions(std::_S_badbit | std::_S_failbit);

		writeString(file, resourceUri);
		writeUuid(file, resource->getUuid());

		auto width = pTexture->getWidth();
		auto height = pTexture->getHeight();
		auto data = pTexture->getData();
		switch (pTexture->getFormat()) {

			case sdk::main::Texture2DImageFormat::RED: write_data<png::gray_pixel>(width, height, data, file); break;
			case sdk::main::Texture2DImageFormat::RG: write_data<png::ga_pixel>(width, height, data, file); break;
			case sdk::main::Texture2DImageFormat::BGR:
			case sdk::main::Texture2DImageFormat::RGB: write_data<png::rgb_pixel>(width, height, data, file); break;
			case sdk::main::Texture2DImageFormat::RGBA:
			case sdk::main::Texture2DImageFormat::BGRA: write_data<png::rgba_pixel>(width, height, data, file); break;
			default: throw std::runtime_error("Unknown texture image format");
		}
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to save texture resource to file");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("Path: {}", pPath.string());
		msg->addInfoLine("Resource URI: {}", resourceUri.string());
		return msg;
	}
	return nullptr;
}

} // namespace mer::editor::mvp