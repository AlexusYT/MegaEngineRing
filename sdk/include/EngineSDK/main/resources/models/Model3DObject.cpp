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

#include "Model3DObject.h"

#include <epoxy/gl.h>

#include "EngineSDK/main/render/IRenderInstance.h"
#include "EngineSDK/main/render/RenderInstanceData.h"
#include "EngineSDK/main/resources/materials/MaterialData.h"
#include "EngineSDK/renderer/buffers/SSBO.h"

namespace mer::sdk::main {


Model3DObject::~Model3DObject() { Model3DObject::onUninitialize(); }

std::shared_ptr<IModel3DObject> Model3DObject::create() { return std::shared_ptr<Model3DObject>(new Model3DObject()); }

utils::ReportMessagePtr Model3DObject::onInitialize() {

	instancesSsbo = std::make_shared<renderer::SSBO>();
	instancesSsbo->setUsage(renderer::DYNAMIC_DRAW);
	materialsSsbo = std::make_shared<renderer::SSBO>();
	materialsSsbo->setUsage(renderer::DYNAMIC_DRAW);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &dataBuffer);
	glGenBuffers(1, &indexBuffer);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(indices.size() * sizeof(uint16_t)), indices.data(),
				 GL_STATIC_DRAW);
	int stride = (3 + 3 + 2) * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(data.size() * sizeof(float)), data.data(), GL_STATIC_DRAW);
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
	return nullptr;
}

void Model3DObject::render() {
	if (instancesData.empty()) return;
	if (!shader) return;
	glBindTexture(GL_TEXTURE_2D, 0);
	instancesSsbo->bindBufferBase(1);
	materialsSsbo->bindBufferBase(2);
	glBindVertexArray(vao);
	for (auto [instShader, instData]: instancesData) {
		if (instData.first.empty()) continue;
		if (instData.second.empty()) continue;
		if (!instShader) shader->use();
		else
			instShader->use();
		if (const int64_t bufSize = static_cast<int64_t>(instData.first.size() * sizeof(RenderInstanceData));
			instancesSsbo->getSize() < bufSize) {
			instancesSsbo->reallocate(bufSize, instData.first.data());
		} else
			instancesSsbo->bufferSubData(0, bufSize, instData.first.data());
		if (const int64_t bufSize = static_cast<int64_t>(instData.second.size() * sizeof(MaterialData));
			materialsSsbo->getSize() < bufSize) {
			materialsSsbo->reallocate(bufSize, instData.second.data());
		} else
			materialsSsbo->bufferSubData(0, bufSize, instData.second.data());
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<int32_t>(indices.size()), GL_UNSIGNED_SHORT, nullptr,
								static_cast<int>(instData.first.size()));
	}
	glBindVertexArray(0);
	instancesSsbo->unbindBufferBase(1);
	instancesSsbo->unbindBufferBase(2);
}

void Model3DObject::onUninitialize() {
	if (instancesSsbo) instancesSsbo.reset();
	//TODO Use unique_ptr with custom deleter or wrap Buffer and VertexArray into the classes
	if (dataBuffer) { glDeleteBuffers(1, &dataBuffer); }
	if (indexBuffer) { glDeleteBuffers(1, &indexBuffer); }
	if (vao) { glDeleteVertexArrays(1, &vao); }
}

void Model3DObject::addRenderInstance(IRenderInstance* pNewInstance) {
	pNewInstance->setInstancedRender(this);
	instances.push_back(pNewInstance);
	onInstanceDataChanged(pNewInstance);
}

void Model3DObject::removeRenderInstance(IRenderInstance* pOldInstance) {
	pOldInstance->setInstancedRender(nullptr);
	erase(instances, pOldInstance);
	onInstanceDataChanged(pOldInstance);
}

void Model3DObject::onInstanceDataChanged(IRenderInstance* /*pInstance*/) {
	instancesData.clear();
	for (auto instance: instances) {

		auto instanceShader = instance->getShader();
		//Use object shader if instance shader is not set
		if (!instanceShader) {
			if (!shader) continue;
			instanceShader = shader;
		}
		auto instanceData = instance->getRenderInstanceData();
		auto materialDataOpt = instance->getMaterialData();
		MaterialData materialData;
		if (materialDataOpt.has_value()) {
			materialData = materialDataOpt.value();
		} else {
			materialData.aoMap = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			materialData.metallicMap = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			materialData.roughnessMap = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			materialData.normalMap = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			materialData.baseColorMap = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		auto iter = instancesData.find(instanceShader);
		if (iter == instancesData.end()) {
			iter = instancesData
					   .emplace(instanceShader, std::pair<std::vector<RenderInstanceData>, std::vector<MaterialData>>())
					   .first;
		}
		iter->second.first.emplace_back(instanceData);
		iter->second.second.emplace_back(materialData);
	}
}

void Model3DObject::onMaterialDataChanged(RenderInstance* pInstance) { onInstanceDataChanged(pInstance); }

bool Model3DObject::operator<(const IModel3DObject &pRhs) const {
	if (this->data < pRhs.getData()) return true;
	if (pRhs.getData() < this->data) return false;

	return this->indices < pRhs.getIndices();
}
} // namespace mer::sdk::main