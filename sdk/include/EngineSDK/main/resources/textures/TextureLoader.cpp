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

#include "TextureLoader.h"

#include "Texture2DImageFormat.h"
#include "Texture2DType.h"
#include "TextureResource.h"
#include "png++/png.hpp"

namespace mer::sdk::main {

template<typename PixelType>
std::vector<PixelType> get_data(const uint32_t pWidth, const uint32_t pHeight, std::istream &pStream) {
	std::vector<PixelType> pixelsOut;
	pixelsOut.resize(pWidth * pHeight);
	png::image<PixelType> img(pStream);
	auto &pixelBuffer = img.get_pixbuf();
	for (uint32_t x = 0; x < pHeight; ++x) {
		memcpy(&pixelsOut[static_cast<size_t>(x) * pWidth], pixelBuffer.get_row(x).data(),
			   static_cast<uint32_t>(pWidth * sizeof(PixelType)));
	}
	return pixelsOut;
}

utils::ReportMessagePtr TextureLoader::load(IResourceLoadExecutor* /*pLoadExecutor*/,
											std::shared_ptr<std::istream> &pStream,
											std::shared_ptr<IResource> &pResourceOut) {
	auto resource = TextureResource::create();
	resource->setResourceUri(readString(pStream));

	png::reader reader(*pStream);
	reader.read_info();
	auto colorType = reader.get_color_type();
	auto width = reader.get_width();
	auto height = reader.get_height();
	pStream->seekg(0);
	switch (colorType) {

		case png::color_type_gray:
			resource->setData(get_data<png::gray_pixel>(width, height, *pStream).data(), width, height,
							  Texture2DImageFormat::RED, Texture2DType::UNSIGNED_BYTE);
			break;
		case png::color_type_rgb:
			resource->setData(get_data<png::rgb_pixel>(width, height, *pStream).data(), width, height,
							  Texture2DImageFormat::RGB, Texture2DType::UNSIGNED_BYTE);
			break;
		case png::color_type_rgb_alpha:
			resource->setData(get_data<png::rgba_pixel>(width, height, *pStream).data(), width, height,
							  Texture2DImageFormat::RGBA, Texture2DType::UNSIGNED_BYTE);
			break;
		case png::color_type_gray_alpha:
			resource->setData(get_data<png::ga_pixel>(width, height, *pStream).data(), width, height,
							  Texture2DImageFormat::RG, Texture2DType::UNSIGNED_BYTE);
			break;
		default:;
	}
	pResourceOut = resource;
	return nullptr;
}

} // namespace mer::sdk::main