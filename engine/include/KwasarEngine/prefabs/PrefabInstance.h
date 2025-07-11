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
// Created by alexus on 27.11.24.
//

#ifndef PREFABINSTANCE_H
#define PREFABINSTANCE_H
#include <glm/vec4.hpp>
#include <memory>
#include <unordered_map>

#include "KwasarEngine/utils/Transformable.h"
#include "KwasarEngine/utils/Transformation.h"
#include "KwasarEngine/resources/materials/IMaterialResource.h"
#include "KwasarEngine/utils/Property.h"
#include "KwasarEngine/utils/UUID.h"
#include "IPrefabInstance.h"

namespace ke {
class Transformation;
}

namespace ke {
class IMaterialResource;
class PrefabElementInstance;

struct PrefabInstanceData {
	bool visible{};
	uint32_t padding{};
};

class PrefabInstance : public IPrefabInstance, public Transformable {
	std::unordered_map<UUID, std::shared_ptr<PrefabElementInstance>> elements;
	uint32_t instanceId{};
	Property<PrefabInstanceData> data;

protected:
	PrefabInstance();

public:
	static std::shared_ptr<PrefabInstance> create();

	Property<bool> visible;

	Property<std::shared_ptr<IMaterialResource>> material;

	std::shared_ptr<PrefabElementInstance> getElement(const UUID &pUuid) const override;

	void addElement(const UUID &pUuid, const std::shared_ptr<PrefabElementInstance> &pInstance) override;

	void removeElement(const UUID &pUuid) override;

	[[nodiscard]] PropertyReadOnly<PrefabInstanceData> getData() { return data.getReadOnly(); }

	[[nodiscard]] uint32_t getInstanceId() const { return instanceId; }

protected:
	void onGlobalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) override;

	void setInstanceId(uint32_t pInstanceId) override { instanceId = pInstanceId; }
};
} // namespace ke

#endif //PREFABINSTANCE_H
