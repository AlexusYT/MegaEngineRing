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

#include "EngineSDK/gltf/Image.h"

#include <png++/image.hpp>
#include <png++/reader.hpp>
#include <turbojpeg.h>

#include "EngineSDK/resources/textures/Texture2DImageFormat.h"
#include "EngineSDK/resources/textures/Texture2DType.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk {
Image::Image(const Microsoft::glTF::Image &pImage, const Microsoft::glTF::Document &pDocument,
			 const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pReader) {

	rawData = pReader->ReadBinaryData(pDocument, pImage);
	name = pImage.name;
}

std::shared_ptr<Image> Image::create(const Microsoft::glTF::Image &pImage, const Microsoft::glTF::Document &pDocument,
									 const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pReader) {
	return std::shared_ptr<Image>(new Image(pImage, pDocument, pReader));
}

class MyIstream {
	std::vector<uint8_t> data;
	size_t pos = 0;

public:
	explicit MyIstream(const std::vector<uint8_t> &pData) : data(pData) {}

	void read(char* pStr, size_t pSize) {
		if (pos + pSize > data.size()) { return; }
		std::string s(data.begin() + static_cast<long int>(pos), data.begin() + static_cast<long int>(pos + pSize));
		memcpy(pStr, s.data(), pSize);
		pos += pSize;
	}

	bool good() const { return pos <= data.size(); }

	void seekg(size_t pNewPow) { pos = pNewPow; }

	size_t tellg() const { return pos; }
};

template<typename PixelType>
std::vector<uint8_t> get_data(const int32_t pWidth, const int32_t pHeight, MyIstream &pStream) {
	std::vector<uint8_t> pixelsOut;
	pixelsOut.resize(static_cast<size_t>(pWidth * pHeight) * sizeof(PixelType));
	png::image<PixelType> img;
	img.read_stream(pStream);
	auto &pixelBuffer = img.get_pixbuf();

	for (int32_t x = 0; x < pHeight; ++x) {
		memcpy(&pixelsOut[static_cast<size_t>(x * pWidth) * sizeof(PixelType)],
			   pixelBuffer.get_row(static_cast<size_t>(x)).data(), static_cast<size_t>(pWidth) * sizeof(PixelType));
	}

	return pixelsOut;
void Image::addReportInfo(const ReportMessagePtr &pMsg) const {
	pMsg->addInfoLine("Image name: {}", name.empty() ? "(null)" : name);
	pMsg->addInfoLine("Image size: {}x{}", width, height);
	pMsg->addInfoLine("Image color format: {} ({})", to_string(format), static_cast<int>(format));
	pMsg->addInfoLine("Image bit depth: {} ({})", to_string(type), static_cast<int>(type));
}

ReportMessagePtr Image::onInitialize() {
	try {
		MyIstream stream(rawData);
		const auto pos = stream.tellg();
		png::reader reader(stream);
		reader.read_info();
		stream.seekg(pos);
		if (auto msg = readPng(reader, stream)) return msg;
	} catch (png::error &) {
		if (auto msg = Utils::decompressJpeg(rawData.data(), rawData.size(), data, &width, &height)) return msg;
		//TODO Read from JPEG file
		format = Texture2DImageFormat::RGB;
		type = Texture2DType::UNSIGNED_BYTE;
	} catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to read image");
		msg->setMessage("Exception occurred");
		return msg;
	}

	rawData.clear();

	return nullptr;
}

void Image::onUninitialize() { Initializable::onUninitialize(); }

ReportMessagePtr Image::readPng(const png::io_base &pReader, MyIstream &pStream) {
	try {
		width = static_cast<int32_t>(pReader.get_width());
		height = static_cast<int32_t>(pReader.get_height());
		const png::color_type colorType = pReader.get_color_type();
		const int bitDepth = pReader.get_bit_depth();
		switch (bitDepth) {
			case 4: type = Texture2DType::UNSIGNED_BYTE; break;
			case 8: type = Texture2DType::UNSIGNED_BYTE; break;
			case 16: type = Texture2DType::UNSIGNED_SHORT; break;
			case 32: type = Texture2DType::UNSIGNED_INT; break;
			default:
				auto msg = ReportMessage::create();
				msg->setTitle("Failed to read image");
				msg->setMessage("Unsupported image bit depth");
				msg->addInfoLine("Image size: {}x{}", width, height);
				msg->addInfoLine("Bit depth: {}", bitDepth);
				return msg;
		}
		switch (colorType) {

			case png::color_type_gray:
				format = Texture2DImageFormat::RED;
				data = get_data<png::gray_pixel>(width, height, pStream);
				break;
			case png::color_type_rgb:
				format = Texture2DImageFormat::RGB;
				data = get_data<png::rgb_pixel>(width, height, pStream);
				break;
			case png::color_type_rgb_alpha:
				format = Texture2DImageFormat::RGBA;
				data = get_data<png::rgba_pixel>(width, height, pStream);
				break;
			case png::color_type_gray_alpha:
				format = Texture2DImageFormat::RG;
				data = get_data<png::ga_pixel>(width, height, pStream);
				break;
			case png::color_type_palette:
				format = Texture2DImageFormat::RGB;
				data = get_data<png::rgb_pixel>(width, height, pStream);
				break;
			default:
				auto msg = ReportMessage::create();
				msg->setTitle("Failed to read image");
				msg->setMessage("Unsupported image type");
				msg->addInfoLine("Image size: {}x{}", width, height);
				msg->addInfoLine("Bit depth: {}", bitDepth);
				msg->addInfoLine("Color type: {}", static_cast<int>(colorType));
				return msg;
		}
	} catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to read image");
		msg->setMessage("Exception occurred");
		return msg;
	}
	return nullptr;
}

} // namespace mer::sdk