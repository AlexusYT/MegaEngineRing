//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 29.09.24.
//

#ifndef PROPERTYJSONSERIALIZER_H
#define PROPERTYJSONSERIALIZER_H

#include <nlohmann/json.hpp>

namespace ke {
class Extension;
}

namespace ke {
template <class T>
struct PropertyJsonSerializer {
	template <std::enable_if_t<std::is_arithmetic<T>::value, bool>  = true>
	static nlohmann::json serialize(const T &pValue, Extension* /*pExtension*/) { return pValue; }

	template <std::enable_if_t<std::is_arithmetic<T>::value, bool>  = true>
	static T deserialize(const nlohmann::json &pJson, Extension* /*pExtension*/) { return pJson.get<T>(); }
};
} // namespace ke


#endif //PROPERTYJSONSERIALIZER_H
