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
// Created by alexus on 29.11.24.
//

#ifndef IPREFABINSTANCE_H
#define IPREFABINSTANCE_H


class UUID;

namespace mer::sdk {
class PrefabElementInstance;

class IPrefabInstance {
public:
	virtual ~IPrefabInstance() = default;

	virtual std::shared_ptr<PrefabElementInstance> getElement(const UUID &pUuid) const = 0;

	virtual void addElement(const UUID &pUuid, const std::shared_ptr<PrefabElementInstance> &pInstance) = 0;

	virtual void removeElement(const UUID &pUuid) = 0;

	virtual void setInstanceId(uint32_t pInstanceId) = 0;
};

} // namespace mer::sdk

#endif //IPREFABINSTANCE_H
