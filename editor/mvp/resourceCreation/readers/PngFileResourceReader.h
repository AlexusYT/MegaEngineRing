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

#ifndef PNGFILERESOURCEREADER_H
#define PNGFILERESOURCEREADER_H

#include <png++/types.hpp>

#include "FileResourceReader.h"

namespace mer::sdk::main {
class ITextureResource;
}

namespace mer::editor::mvp {

class PngFileResourceReader : public FileResourceReader {
	png::color_type colorType;
	uint32_t width;
	uint32_t height;

public:
	explicit PngFileResourceReader(const std::filesystem::path &pPath) : FileResourceReader(pPath) {}

	sdk::utils::ReportMessagePtr checkType() override;

	std::shared_ptr<sdk::main::ITextureResource> generateResource() const;
};

} // namespace mer::editor::mvp

#endif //PNGFILERESOURCEREADER_H
