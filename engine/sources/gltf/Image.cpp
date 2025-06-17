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

#include "KwasarEngine/gltf/Image.h"

#include <spng.h>

#include "KwasarEngine/resources/textures/Texture2DImageFormat.h"
#include "KwasarEngine/resources/textures/Texture2DType.h"
#include "KwasarEngine/utils/Logger.h"

namespace ke {
Image::Image(const Microsoft::glTF::Image &pImage, const Microsoft::glTF::Document &pDocument,
			 const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pReader)
	: width(-1), height(-1), format(Texture2DImageFormat::UNDEFINED), type(Texture2DType::UNDEFINED) {
	rawData = pReader->ReadBinaryData(pDocument, pImage);
	name = pImage.name;
}

std::shared_ptr<Image> Image::create(const Microsoft::glTF::Image &pImage, const Microsoft::glTF::Document &pDocument,
									 const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pReader) {
	return std::shared_ptr<Image>(new Image(pImage, pDocument, pReader));
}

void Image::addReportInfo(const ReportMessagePtr &pMsg) const {
	pMsg->addInfoLine("Image name: {}", name.empty() ? "(null)" : name);
	pMsg->addInfoLine("Image size: {}x{}", width, height);
	pMsg->addInfoLine("Image color format: {} ({})", to_string(format), static_cast<int>(format));
	pMsg->addInfoLine("Image bit depth: {} ({})", to_string(type), static_cast<int>(type));
}

ReportMessagePtr Image::onInitialize() {
	ReportMessagePtr msg;

	try {
		if (spng_ihdr ihdrOut; auto ctx = isPng(msg, ihdrOut)) { msg = readPng(ctx, ihdrOut); } else if (
			msg = Utils::decompressJpeg(rawData.data(), rawData.size(), data, &width, &height); !msg) {
			//TODO Read from JPEG file
			format = Texture2DImageFormat::RGB;
			type = Texture2DType::UNSIGNED_BYTE;
		}
	}
	catch (...) {
		msg = ReportMessage::create();
		msg->setTitle("Failed to read image");
		msg->setMessage("Exception occurred");
		addReportInfo(msg);
	}

	rawData.clear();

	return msg;
}

void Image::onUninitialize() { Initializable::onUninitialize(); }

ReportMessagePtr Image::readPng(const std::unique_ptr<spng_ctx, void (*)(spng_ctx*)> &pCtx, const spng_ihdr &pIhdr) {
	try {
		width = static_cast<int32_t>(pIhdr.width);
		height = static_cast<int32_t>(pIhdr.height);
		const auto colorType = pIhdr.color_type;
		const int bitDepth = pIhdr.bit_depth;
		switch (bitDepth) {
			case 4: type = Texture2DType::UNSIGNED_BYTE;
				break;
			case 8: type = Texture2DType::UNSIGNED_BYTE;
				break;
			case 16: type = Texture2DType::UNSIGNED_SHORT;
				break;
			case 32: type = Texture2DType::UNSIGNED_INT;
				break;
			default:
				auto msg = ReportMessage::create();
				msg->setTitle("Failed to read image");
				msg->setMessage("Unsupported image bit depth");
				msg->addInfoLine("File bit depth: {}", bitDepth);
				addReportInfo(msg);
				return msg;
		}
		switch (colorType) {
			case SPNG_COLOR_TYPE_GRAYSCALE: format = Texture2DImageFormat::RED;
				break;
			case SPNG_COLOR_TYPE_TRUECOLOR: format = Texture2DImageFormat::RGB;
				break;
			case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA: format = Texture2DImageFormat::RGBA;
				break;
			case SPNG_COLOR_TYPE_GRAYSCALE_ALPHA: format = Texture2DImageFormat::RG;
				break;
			case SPNG_COLOR_TYPE_INDEXED: format = Texture2DImageFormat::RGB;
				break;
			default:
				auto msg = ReportMessage::create();
				msg->setTitle("Failed to read image");
				msg->setMessage("Unsupported image type");
				msg->addInfoLine("File color type: {}", static_cast<int>(colorType));
				addReportInfo(msg);
				return msg;
		}
	}
	catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to read image");
		msg->setMessage("Exception occurred");
		addReportInfo(msg);
		return msg;
	}
	/*spng_plte plte{};
	ret = spng_get_plte(ctx.get(), &plte);

	if (ret && ret != SPNG_ECHUNKAVAIL) { printf("spng_get_plte() error: %s\n", spng_strerror(ret)); }

	if (!ret) printf("palette entries: %u\n", plte.n_entries);*/


	size_t image_size;

	/* Output format, does not depend on source PNG format except for
	   SPNG_FMT_PNG, which is the PNG's format in host-endian or
	   big-endian for SPNG_FMT_RAW.
	   Note that for these two formats <8-bit images are left byte-packed */
	int fmt = SPNG_FMT_PNG;

	/* With SPNG_FMT_PNG indexed color images are output as palette indices,
	   pick another format to expand them. */
	if (pIhdr.color_type == SPNG_COLOR_TYPE_INDEXED) fmt = SPNG_FMT_RGB8;


	if (auto ret = spng_decoded_image_size(pCtx.get(), fmt, &image_size)) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to read png image");
		msg->setMessage("Function spng_decoded_image_size failed");
		msg->addInfoLine("Error message: {} ({})", spng_strerror(ret), ret);
		addReportInfo(msg);
		return msg;
	}

	std::vector<unsigned char> image(image_size);

	/* Decode the image in one go */
	/* ret = spng_decode_image(ctx, image, image_size, SPNG_FMT_RGBA8, 0);

	if(ret)
	{
		printf("spng_decode_image() error: %s\n", spng_strerror(ret));
		goto error;
	}*/

	/* Alternatively, you can decode the image progressively;
	   this requires an initialization step. */


	if (auto ret = spng_decode_image(pCtx.get(), NULL, 0, fmt, SPNG_DECODE_PROGRESSIVE)) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to read png image");
		msg->setMessage("Function spng_decode_image failed");
		msg->addInfoLine("Error message: {} ({})", spng_strerror(ret), ret);
		addReportInfo(msg);
		return msg;
	}

	/* ihdr.height will always be non-zero if spng_get_ihdr() succeeds */
	size_t image_width = image_size / static_cast<size_t>(height);

	spng_row_info rowInfo = {};
	int ret;
	do {
		ret = spng_get_row_info(pCtx.get(), &rowInfo);
		if (ret) break;

		ret = spng_decode_row(pCtx.get(), image.data() + rowInfo.row_num * image_width, image_width);
	} while (!ret);

	if (ret != SPNG_EOI) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to read png image");
		msg->setMessage("Progressive decoding failed");
		msg->addInfoLine("Error message: {} ({})", spng_strerror(ret), ret);
		if (pIhdr.interlace_method) msg->addInfoLine("Last pass: {}, scanline: {}", rowInfo.pass, rowInfo.scanline_idx);
		else
			msg->addInfoLine("last row: {}", rowInfo.row_num);
		addReportInfo(msg);
	}
	data = image;
	return nullptr;
}

std::unique_ptr<spng_ctx, void (*)(spng_ctx*)> Image::isPng(ReportMessagePtr &pErrorOut, spng_ihdr &pIhdrOut) const {
	std::unique_ptr<spng_ctx, void (*)(spng_ctx*)>
		ctx(spng_ctx_new(0), spng_ctx_free);

	/* Ignore and don't calculate chunk CRC's */
	spng_set_crc_action(ctx.get(), SPNG_CRC_USE, SPNG_CRC_USE);

	/* Set memory usage limits for storing standard and unknown chunks,
		   this is important when reading untrusted files! */
	size_t limit = 1024 * 1024 * 64;
	spng_set_chunk_limits(ctx.get(), limit, limit);

	/* Set source PNG */
	spng_set_png_buffer(ctx.get(), rawData.data(), rawData.size());

	auto ret = spng_get_ihdr(ctx.get(), &pIhdrOut);

	if (ret == SPNG_ESIGNATURE) { ctx.reset(); } else if (ret != SPNG_OK) {
		const auto msg = ReportMessage::create();
		msg->setTitle("Failed to read png image");
		msg->setMessage("Function spng_get_ihdr failed");
		msg->addInfoLine("Error message: {} ({})", spng_strerror(ret), ret);
		addReportInfo(msg);
		pErrorOut = msg;
		ctx.reset();
	}
	return ctx;
}
} // namespace ke
