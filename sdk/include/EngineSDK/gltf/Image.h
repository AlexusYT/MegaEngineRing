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

#ifndef IMAGE_H
#define IMAGE_H

#include "EngineSDK/render/Initializable.h"
#include "EngineUtils/utils/IReportable.h"
#include "GLTFSDK/Document.h"
#include "GLTFSDK/GLTFResourceReader.h"

struct spng_ihdr;
typedef struct spng_ctx spng_ctx;

namespace Microsoft::glTF {
struct Image;
}

namespace mer::sdk {
class MyIstream;
enum class Texture2DType;
enum class Texture2DImageFormat;

class Image : public Initializable, public IReportable {
	std::vector<uint8_t> rawData;
	int32_t width;
	int32_t height;
	Texture2DImageFormat format;
	Texture2DType type;
	std::vector<uint8_t> data;
	std::string name;


	explicit Image(const Microsoft::glTF::Image &pImage, const Microsoft::glTF::Document &pDocument,
				   const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pReader);

public:
	static std::shared_ptr<Image> create(const Microsoft::glTF::Image &pImage,
										 const Microsoft::glTF::Document &pDocument,
										 const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pReader);

	[[nodiscard]] int32_t getWidth() const { return width; }

	[[nodiscard]] int32_t getHeight() const { return height; }

	[[nodiscard]] Texture2DImageFormat getFormat() const { return format; }

	[[nodiscard]] Texture2DType getType() const { return type; }

	[[nodiscard]] const std::vector<uint8_t> &getData() const { return data; }

	void addReportInfo(const ReportMessagePtr &pMsg) const override;

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

	ReportMessagePtr readPng(const std::unique_ptr<spng_ctx, void (*)(spng_ctx*)> &pCtx, const spng_ihdr &pIhdr);

	std::unique_ptr<spng_ctx, void (*)(spng_ctx*)> isPng(ReportMessagePtr &pErrorOut, spng_ihdr &pIhdrOut) const;
};
} // namespace mer::sdk

#endif //IMAGE_H
