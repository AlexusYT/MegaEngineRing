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
// Created by alexus on 24.11.24.
//

#include "ResourceSaverBase.h"

namespace mer::editor::mvp {
void ResourceSaverBase::writeString(std::ofstream &pStream, const std::string &pStr) {

	const auto nameSize = static_cast<uint16_t>(pStr.size());
	writeNumber(pStream, nameSize);
	pStream.write(pStr.data(), static_cast<long int>(nameSize));
}

void ResourceSaverBase::writeUuid(std::ofstream &pStream, const std::shared_ptr<UUID> &pUuid) {
	const auto uuid = pUuid.get();
	pStream.write(reinterpret_cast<const std::ostream::char_type*>(uuid), sizeof(*uuid));
}
} // namespace mer::editor::mvp