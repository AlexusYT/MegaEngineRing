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

#include "PngFileResourceReader.h"

#include "EngineSDK/resources/textures/ITextureResource.h"
#include "EngineSDK/resources/textures/Texture2DImageFormat.h"
#include "EngineSDK/resources/textures/Texture2DType.h"
#include "EngineSDK/resources/textures/TextureBaseInternalFormat.h"
#include "png++/png.hpp"

namespace mer::editor::mvp {
sdk::ReportMessagePtr PngFileResourceReader::checkType() {

	if (auto msg = open()) {
		msg->setTitle("Unable to prove that the file is the PNG File");
		return msg;
	}
	png::reader<std::istream> reader(*getStream());
	reader.read_info();
	colorType = reader.get_color_type();
	width = reader.get_width();
	height = reader.get_height();
	getStream()->seekg(0);

	return nullptr;
}

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

void PngFileResourceReader::generateResourceData(const std::shared_ptr<sdk::ITextureResource> &pResource) const {

	using namespace sdk;
	auto &streamSelf = *getStream();
	switch (colorType) {

		case png::color_type_gray:
			pResource->setData(get_data<png::gray_pixel>(width, height, streamSelf).data(), width, height,
							   Texture2DImageFormat::RED, Texture2DType::UNSIGNED_BYTE);
			break;
		case png::color_type_rgb:
			pResource->setData(get_data<png::rgb_pixel>(width, height, streamSelf).data(), width, height,
							   Texture2DImageFormat::RGB, Texture2DType::UNSIGNED_BYTE);
			break;
		case png::color_type_rgb_alpha:
			pResource->setData(get_data<png::rgba_pixel>(width, height, streamSelf).data(), width, height,
							   Texture2DImageFormat::RGBA, Texture2DType::UNSIGNED_BYTE);
			break;
		case png::color_type_gray_alpha:
			pResource->setData(get_data<png::ga_pixel>(width, height, streamSelf).data(), width, height,
							   Texture2DImageFormat::RG, Texture2DType::UNSIGNED_BYTE);
			break;
		default:;
	}
}
} // namespace mer::editor::mvp