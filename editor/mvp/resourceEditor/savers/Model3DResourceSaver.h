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
// Created by alexus on 02.10.24.
//

#ifndef MODEL3DRESOURCESAVER_H
#define MODEL3DRESOURCESAVER_H
#include "ResourceSaverBase.h"

namespace mer::sdk::main {
class IModel3DResource;
}

namespace mer::editor::mvp {

class Model3DResourceSaver : public ResourceSaverBase {
public:
	static sdk::utils::ReportMessagePtr saveToFile(const std::filesystem::path &pPath,
												   const std::shared_ptr<sdk::main::IModel3DResource> &pModel);

private:
	template<typename T>
	static void writeArray(std::ofstream &pStream, const std::vector<T> &pArray) {
		uint64_t size = static_cast<uint64_t>(pArray.size());
		pStream.write(reinterpret_cast<const std::ostream::char_type*>(&size), sizeof(size));
		pStream.write(reinterpret_cast<const std::ostream::char_type*>(pArray.data()),
					  static_cast<long int>(size * sizeof(T)));
	}
};

} // namespace mer::editor::mvp

#endif //MODEL3DRESOURCESAVER_H
