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
// Created by alexus on 27.11.24.
//

#ifndef PREFABELEMENT_H
#define PREFABELEMENT_H
#include <string>

#include "EngineUtils/utils/Property.h"
#include "EngineUtils/utils/UUID.h"

namespace mer::sdk::main {
class IMaterialResource;
class Prefab;
class PrefabElementInstance;

class PrefabElement {
	UUID uuid;
	Prefab* prefab{};
	std::string name;
	uint64_t indicesStart{};
	uint64_t dataStart{};
	uint64_t indicesCount{};
	uint64_t dataCount{};
	std::vector<std::shared_ptr<PrefabElementInstance>> instances;
	uint32_t idInPrefab{};

protected:
	explicit PrefabElement(const std::string &pName);

public:
	virtual ~PrefabElement() = default;

	utils::Property<bool> visible;

	utils::Property<std::shared_ptr<IMaterialResource>> material;


	std::shared_ptr<PrefabElementInstance> createInstance();

	void addInstance(std::shared_ptr<PrefabElementInstance> pInstance);

	void setPrefab(Prefab* pPrefab) { prefab = pPrefab; }

	[[nodiscard]] Prefab* getPrefab() const { return prefab; }

	[[nodiscard]] const UUID &getUuid() const { return uuid; }

	[[nodiscard]] const std::string &getName() const { return name; }

	virtual const std::vector<float> &getData() const = 0;

	virtual const std::vector<uint16_t> &getIndices() const = 0;

	[[nodiscard]] uint64_t getIndicesStart() const { return indicesStart; }

	void setIndicesStart(uint64_t pIndicesStart);

	[[nodiscard]] uint64_t getDataStart() const { return dataStart; }

	void setDataStart(uint64_t pDataStart);

	[[nodiscard]] uint64_t getIndicesCount() const { return indicesCount; }

	void setIndicesCount(uint64_t pIndicesCount);

	[[nodiscard]] uint64_t getDataCount() const { return dataCount; }

	void setDataCount(uint64_t pDataCount);

	[[nodiscard]] uint32_t getIdInPrefab() const { return idInPrefab; }

	void setIdInPrefab(const uint32_t pIdInPrefab) { idInPrefab = pIdInPrefab; }

	void onParentMaterialChanged(const std::shared_ptr<IMaterialResource> &pMaterial);
};

} // namespace mer::sdk::main

#endif //PREFABELEMENT_H
