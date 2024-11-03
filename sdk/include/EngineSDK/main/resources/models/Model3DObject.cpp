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


Model3DObject::~Model3DObject() { Model3DObject::destroyRender(); }

std::shared_ptr<IModel3DObject> Model3DObject::create() { return std::shared_ptr<Model3DObject>(new Model3DObject()); }

void Model3DObject::setupRender() {

	instancesSsbo = std::make_shared<renderer::SSBO>();
	instancesSsbo->setUsage(renderer::DYNAMIC_DRAW);
	materialsSsbo = std::make_shared<renderer::SSBO>();
	materialsSsbo->setUsage(renderer::DYNAMIC_DRAW);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &normalsBuffer);
	glGenBuffers(1, &indexBuffer);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(indices.size() * sizeof(uint16_t)), indices.data(),
				 GL_STATIC_DRAW);
	//Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(vertices.size() * sizeof(glm::vec3)), vertices.data(),
				 GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	//UV buffer
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(uvs.size() * sizeof(glm::vec2)), uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);

	//Normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(normals.size() * sizeof(glm::vec3)), normals.data(),
				 GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void Model3DObject::render() {
	if (instancesData.empty()) return;
	if (!shader) return;
	glBindTexture(GL_TEXTURE_2D, 0);
	instancesSsbo->bindBufferBase(1);
	materialsSsbo->bindBufferBase(2);
	glBindVertexArray(vao);
	for (auto [instShader, data]: instancesData) {
		if (data.first.empty()) continue;
		if (data.second.empty()) continue;
		if (!instShader) shader->use();
		else
			instShader->use();
		if (const int64_t bufSize = static_cast<int64_t>(data.first.size() * sizeof(RenderInstanceData));
			instancesSsbo->getSize() < bufSize) {
			instancesSsbo->reallocate(bufSize, data.first.data());
		} else
			instancesSsbo->bufferSubData(0, bufSize, data.first.data());
		if (const int64_t bufSize = static_cast<int64_t>(data.second.size() * sizeof(MaterialData));
			materialsSsbo->getSize() < bufSize) {
			materialsSsbo->reallocate(bufSize, data.second.data());
		} else
			materialsSsbo->bufferSubData(0, bufSize, data.second.data());
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<int32_t>(indices.size()), GL_UNSIGNED_SHORT, nullptr,
								static_cast<int>(data.first.size()));
	}
	glBindVertexArray(0);
	instancesSsbo->unbindBufferBase(1);
	instancesSsbo->unbindBufferBase(2);
}

void Model3DObject::destroyRender() {
	if (instancesSsbo) instancesSsbo.reset();
	//TODO Use unique_ptr with custom deleter or wrap Buffer and VertexArray into the classes
	if (vertexBuffer) { glDeleteBuffers(1, &vertexBuffer); }
	if (uvBuffer) { glDeleteBuffers(1, &uvBuffer); }
	if (normalsBuffer) { glDeleteBuffers(1, &normalsBuffer); }
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
	if (this->vertices < pRhs.getVertices()) return true;
	if (pRhs.getVertices() < this->vertices) return false;
	if (this->uvs < pRhs.getUvs()) return true;
	if (pRhs.getUvs() < this->uvs) return false;
	if (this->normals < pRhs.getNormals()) return true;
	if (pRhs.getNormals() < this->normals) return false;
	return this->indices < pRhs.getIndices();
}
} // namespace mer::sdk::main