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
// Created by alexus on 22.10.24.
//

#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include "EngineSDK/resources/ResourceLoader.h"

namespace mer::sdk::main {

class TextureLoader : public ResourceLoader {
public:
	std::shared_ptr<IResource> createResource() override;

	utils::ReportMessagePtr load(IResourceLoadExecutor* pLoadExecutor, std::shared_ptr<std::istream> &pStream,
								 const std::shared_ptr<IResource> &pResource) override;


private:
	std::string getFileExtension() override { return "entex"; }
};
} // namespace mer::sdk::main

#endif //TEXTURELOADER_H
