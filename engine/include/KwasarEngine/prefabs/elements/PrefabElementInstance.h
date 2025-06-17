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

#ifndef PREFABELEMENTINSTANCE_H
#define PREFABELEMENTINSTANCE_H
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <sigc++/scoped_connection.h>

#include "KwasarEngine/prefabs/IPrefabElementInstance.h"
#include "KwasarEngine/resources/materials/IMaterialResource.h"
#include "KwasarEngine/resources/materials/MaterialData.h"
#include "KwasarEngine/utils/Transformable.h"
#include "KwasarEngine/utils/Property.h"

namespace ke {
class PrefabInstance;
class PrefabElement;
class IMaterialResource;

struct PrefabElementInstanceData {
	MaterialData material;
	glm::mat4 modelMat;
	glm::mat4 normalMat;
	bool visible{};

	[[maybe_unused]] uint32_t padding1{};
	[[maybe_unused]] uint32_t padding2{};
	[[maybe_unused]] uint32_t padding3{};
	/*uint32_t alignment6{};
	uint32_t alignment7{};
	uint32_t alignment8{};
	uint32_t alignment9{};
	uint32_t alignment10{};*/
	/*uint16_t alignment1{};
	uint32_t alignment2{};
	uint64_t alignment3{};*/
};

class PrefabElementInstance : public IPrefabElementInstance, public Transformable {
	PrefabElement* element{};
	PrefabInstance* prefabInstance{};
	Property<PrefabElementInstanceData> data;
	uint64_t prefabInstanceId{};
	sigc::scoped_connection materialConnection;

protected:
	PrefabElementInstance();

public:
	static std::shared_ptr<PrefabElementInstance> create();

	Property<bool> visible;
	Property<std::shared_ptr<IMaterialResource>> material;

	[[nodiscard]] PropertyReadOnly<PrefabElementInstanceData> getData() { return data.getReadOnly(); }

	[[nodiscard]] PrefabElement* getElement() const { return element; }

	[[nodiscard]] uint64_t getPrefabInstanceId() const { return prefabInstanceId; }

	[[nodiscard]] PrefabInstance* getPrefabInstance() const { return prefabInstance; }

	void onParentMaterialChanged(const std::shared_ptr<IMaterialResource> &pMaterial);

protected:
	void onGlobalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) override;

private:
	void setElement(PrefabElement* pElement) final { element = pElement; }

	void setPrefabInstanceId(uint64_t pPrefabInstanceId) final { prefabInstanceId = pPrefabInstanceId; }

	void setPrefabInstance(PrefabInstance* pPrefabInstance) override { prefabInstance = pPrefabInstance; }

	void onMaterialChanged(const MaterialData &pData);
};
} // namespace ke

#endif //PREFABPARTINSTANCE_H
