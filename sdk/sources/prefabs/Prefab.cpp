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


#include "EngineSDK/prefabs/Prefab.h"

#include <algorithm>
#include <epoxy/gl.h>

#include "EngineSDK/prefabs/IPrefabElementInstance.h"
#include "EngineSDK/prefabs/PrefabSsbo.h"
#include "EngineSDK/prefabs/elements/PrefabElement.h"
#include "EngineSDK/prefabs/elements/PrefabElementInstance.h"
#include "EngineSDK/prefabs/elements/PrefabElementsSsbo.h"
#include "EngineSDK/resources/materials/IMaterialResource.h"
#include "EngineSDK/resources/shaders/ShaderProgram.h"

namespace mer::sdk {
Prefab::Prefab()
	: name("UnnamedPrefab"), uuid(UUID::newInstance()), prefabElementsSsbo(PrefabElementsSsbo::create()),
	  visible(nullptr, ""), material(nullptr, "") {
	prefabSsbo = PrefabSsbo::create();
	visible = true;
	material.connectEvent([this](const std::shared_ptr<IMaterialResource> &pMaterial) {
		for (auto element: elementsList) { element->onParentMaterialChanged(pMaterial); }
	});
}

std::shared_ptr<Prefab> Prefab::create() { return std::shared_ptr<Prefab>(new Prefab()); }

Prefab::~Prefab() {}

void Prefab::addElement(const std::shared_ptr<PrefabElement> &pPrefabElement) {
	pPrefabElement->setPrefab(this);
	pPrefabElement->setIdInPrefab(static_cast<uint32_t>(elementsList.size()));
	auto elementUuid = pPrefabElement->getUuid();
	elements.emplace(elementUuid, pPrefabElement);
	elementsList.emplace_back(pPrefabElement.get());
	auto elementMeshData = pPrefabElement->getData();
	pPrefabElement->setDataCount(elementMeshData.size());
	pPrefabElement->setDataStart(data.size());
	data.insert(data.end(), elementMeshData.begin(), elementMeshData.end());
	auto elementMeshIndices = pPrefabElement->getIndices();
	pPrefabElement->setIndicesCount(elementMeshIndices.size());
	pPrefabElement->setIndicesStart(indices.size());
	indices.insert(indices.end(), elementMeshIndices.begin(), elementMeshIndices.end());
	buffersDirty = true;
	for (auto instance: instances) {
		auto elementInstance = pPrefabElement->createInstance();
		if (auto interface = std::dynamic_pointer_cast<IPrefabElementInstance>(elementInstance))
			interface->setPrefabInstance(instance.get());
		prefabElementsSsbo->trackInstance(elementInstance.get());
		instance->addElement(elementUuid, elementInstance);
	}
	prefabElementsSsbo->setElementsPerInstance(static_cast<uint32_t>(elementsList.size()));
	pPrefabElement->onParentMaterialChanged(material);
}

void Prefab::removeElement(const std::shared_ptr<PrefabElement> &pPrefabElement) {
	auto resultIter = std::ranges::find_if(elementsList, [pPrefabElement](const PrefabElement* pFirst) {
		return pFirst->getName() == pPrefabElement->getName();
	});

	if (resultIter == elementsList.end()) return;
	auto elementToRemove = *resultIter;
	data.erase(data.begin() + static_cast<long int>(elementToRemove->getDataStart()),
			   data.begin() + static_cast<long int>(elementToRemove->getDataStart() + elementToRemove->getDataCount()));
	indices.erase(indices.begin() + static_cast<long int>(elementToRemove->getIndicesStart()),
				  indices.begin() +
					  static_cast<long int>(elementToRemove->getIndicesStart() + elementToRemove->getIndicesCount()));
	buffersDirty = true;

	for (auto iter = resultIter, endIter = elementsList.end(); iter != endIter; ++iter) {
		auto element = *iter;
		element->setIndicesStart(element->getIndicesStart() - (*resultIter)->getIndicesCount());
		element->setDataStart(element->getDataStart() - (*resultIter)->getDataCount());
	}
	auto elementUuid = pPrefabElement->getUuid();
	elementsList.erase(resultIter);
	elements.erase(elementUuid);
	uint32_t id = 0;
	for (auto element: elementsList) {
		element->setIdInPrefab(id);
		id++;
	}
	for (auto instance: instances) {
		auto elementInstance = instance->getElement(elementUuid);
		instance->removeElement(elementUuid);
		prefabElementsSsbo->untrackInstance(elementInstance.get());
	}
	prefabElementsSsbo->setElementsPerInstance(static_cast<uint32_t>(elementsList.size()));
	pPrefabElement->setPrefab(nullptr);
}

std::shared_ptr<PrefabElement> Prefab::getElement(const UUID &pElementUuid) {
	const auto iter = elements.find(pElementUuid);
	if (iter == elements.end()) return nullptr;
	return iter->second;
}

std::shared_ptr<PrefabInstance> Prefab::createInstance() {
	auto prefabInstance = PrefabInstance::create();
	addInstance(prefabInstance);
	return prefabInstance;
}

void Prefab::addInstance(const std::shared_ptr<PrefabInstance> &pInstance) {

	if (auto interface = std::dynamic_pointer_cast<IPrefabInstance>(pInstance))
		interface->setInstanceId(static_cast<uint32_t>(instances.size()));
	instances.emplace_back(pInstance);
	for (auto &[elementName, element]: elements) {
		auto elementInstance = element->createInstance();

		if (auto interface = std::dynamic_pointer_cast<IPrefabElementInstance>(elementInstance))
			interface->setPrefabInstance(pInstance.get());
		prefabElementsSsbo->trackInstance(elementInstance.get());
		pInstance->addElement(element->getUuid(), elementInstance);
	}
	prefabSsbo->trackInstance(pInstance.get());
}

void Prefab::updateElement(PrefabElement* /*pElement*/) { /*pElement*/ }

ReportMessagePtr Prefab::onInitialize() {
	prefabElementsSsbo->initialize();
	createBuffers();
	return nullptr;
}

void Prefab::createBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &dataBuffer);
	glGenBuffers(1, &indexBuffer);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(indices.size() * sizeof(uint16_t)), indices.data(),
				 GL_DYNAMIC_DRAW);
	int stride = (3 + 3 + 2) * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(data.size() * sizeof(float)), data.data(), GL_DYNAMIC_DRAW);
	//Vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	buffersDirty = false;
}

void Prefab::render() {
	if (!visible) return;

	prefabElementsSsbo->render();
	prefabSsbo->render();
	if (!shaderProgram) return;
	shaderProgram->use();
	prefabElementsSsbo->bind(1);
	prefabSsbo->bindBufferBase(2);
	if (buffersDirty) {
		deleteBuffers();
		createBuffers();
	}
	glBindVertexArray(vao);

	for (uint32_t i = 0; auto element: elementsList) {
		if (!element->visible) {
			i++;
			continue;
		}
		shaderProgram->setUniform("elementIdIn", i);
		glDrawElementsInstancedBaseVertex(
			GL_TRIANGLES, static_cast<int32_t>(element->getIndicesCount()), GL_UNSIGNED_SHORT,
			reinterpret_cast<void*>(static_cast<int>(element->getIndicesStart() * sizeof(uint16_t))),
			static_cast<int>(instances.size()), static_cast<int>(element->getDataStart() / 8));
		i++;
	}

	glBindVertexArray(0);
}

void Prefab::deleteBuffers() {
	if (vao) glDeleteVertexArrays(1, &vao);
	if (dataBuffer) glGenBuffers(1, &dataBuffer);
	if (indexBuffer) glGenBuffers(1, &indexBuffer);
}

void Prefab::onUninitialize() {
	prefabElementsSsbo.reset();
	prefabSsbo.reset();
}
} // namespace mer::sdk