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
// Created by alexus on 29.11.24.
//

#ifndef IPREFABELEMENTINSTANCE_H
#define IPREFABELEMENTINSTANCE_H

namespace ke {
class PrefabElement;
class PrefabInstance;

class IPrefabElementInstance {
public:
	virtual ~IPrefabElementInstance() = default;

	virtual void setElement(PrefabElement* pElement) = 0;

	virtual void setPrefabInstanceId(uint64_t pPrefabInstanceId) = 0;

	virtual void setPrefabInstance(PrefabInstance* pPrefabInstance) = 0;
};
} // namespace ke

#endif //IPREFABELEMENTINSTANCE_H
