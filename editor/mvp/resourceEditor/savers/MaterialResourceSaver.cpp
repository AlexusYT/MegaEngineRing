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
// Created by alexus on 25.10.24.
//

#include "MaterialResourceSaver.h"

#include "EngineSDK/main/resources/IResource.h"
#include "EngineSDK/main/resources/materials/IMaterialResource.h"

namespace mer::editor::mvp {
sdk::utils::ReportMessagePtr MaterialResourceSaver::saveToFile(
	const std::filesystem::path &pPath, const std::shared_ptr<sdk::main::IMaterialResource> &pResource) {
	auto dir = pPath.parent_path();
	if (!exists(dir)) { create_directories(dir); }
	auto resourceUri = pResource->asResource()->getResourceUri();
	try {
		std::ofstream file(pPath, std::ios::out | std::ios::binary);
		file.exceptions(std::_S_badbit | std::_S_failbit);

		writeString(file, resourceUri);

	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to save material resource to file");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("Path: {}", pPath.string());
		msg->addInfoLine("Resource URI: {}", resourceUri.string());
		return msg;
	}
	return nullptr;
}

void MaterialResourceSaver::writeString(std::ofstream &pStream, const std::string &pStr) {

	auto nameSize = pStr.size();
	pStream.write(reinterpret_cast<const std::ostream::char_type*>(&nameSize), sizeof(nameSize));
	pStream.write(pStr.data(), static_cast<long int>(nameSize));
}
} // namespace mer::editor::mvp