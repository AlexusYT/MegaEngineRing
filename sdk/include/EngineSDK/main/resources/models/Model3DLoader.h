// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 26.03.24.
//

#ifndef MODEL3DLOADER_H
#define MODEL3DLOADER_H

#include "EngineSDK/main/resources/ResourceLoader.h"

namespace mer::sdk::main {
class Model3DLoader : public ResourceLoader {
public:
	utils::ReportMessagePtr load(IResourceLoadExecutor* pLoadExecutor, std::shared_ptr<std::istream> &pStream,
								 std::shared_ptr<IResource> &pResourceOut) override;

	template<typename T>
	static void readArray(const std::shared_ptr<std::istream> &pStream, std::vector<T> &pArray) {
		uint16_t size = 0;
		pStream->read(reinterpret_cast<std::istream::char_type*>(&size), sizeof(size));
		pArray.resize(size);
		pStream->read(reinterpret_cast<std::istream::char_type*>(pArray.data()),
					  static_cast<long int>(size * sizeof(T)));
	}

	utils::ReportMessagePtr init(IResourceLoadExecutor* pLoadExecutor,
								 const std::shared_ptr<IResource> &pLoadedResource) override;

private:
	std::string getFileExtension() override { return "enmodel"; }
};

} // namespace mer::sdk::main


#endif //MODEL3DLOADER_H
