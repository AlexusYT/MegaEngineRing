//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 14.03.25.
//

#include "EngineSDK/render/Renderer.h"

#include <epoxy/gl.h>
#include <mutex>
#include <sigc++/adaptors/bind.h>

#include "EngineSDK/gltf/Accessor.h"
#include "EngineSDK/gltf/Material.h"
#include "EngineSDK/gltf/Mesh.h"
#include "EngineSDK/gltf/Primitive.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk {
class Logger;

RenderPass::RenderPass() {}

Renderer::Renderer() {
	addRenderPass(getMainPassName(), std::make_shared<RenderPass>());
	materialToIndexMap.emplace(nullptr, 0);
	MaterialData defaultMaterial;
	//defaultMaterial.baseColorFactor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	defaultMaterial.baseColorFactor = glm::vec4(1.0f);
	defaultMaterial.metallicRoughnessInfo = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	defaultMaterial.emissiveFactor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	materialsSsbo.addElement(defaultMaterial);
}

void Renderer::addMaterial(const std::shared_ptr<Material> &pMaterial) {

	auto connection = pMaterial->connectOnChanged([this](Material* pChangedMaterial) {
		auto index = materialToIndexMap.at(pChangedMaterial);
		materialsSsbo.setElement(index, pChangedMaterial->getData());
	});
	materials.emplace_back(pMaterial);
	materialsConnections.emplace(pMaterial.get(), connection);
	materialToIndexMap.emplace(pMaterial.get(), materialsSsbo.size());
	materialsSsbo.addElement(pMaterial->getData());
	if (!pMaterial->isInited()) {
		std::lock_guard lock(uninitializedMaterialsMutex);
		uninitializedMaterials.emplace_back(pMaterial.get());
	}
}

void Renderer::removeAllMaterials() {
	{
		std::lock_guard lock(uninitializedMaterialsMutex);
		uninitializedMaterials.clear();
	}
	materialsConnections.clear();
	materials.clear();
	materialsSsbo.clear();
}

void Renderer::addMesh(const std::shared_ptr<Mesh> &pNewMesh) {
	if (meshToInfoMap.contains(pNewMesh.get())) return;
	std::scoped_lock lock(indicesMutex);

	MeshInfo info;
	for (auto primitive: pNewMesh->getPrimitives()) {
		auto indexAccessor = primitive->getIndexAccessor();
		auto indexType = indexAccessor->getComponentType();
		if (indexType != Microsoft::glTF::COMPONENT_SHORT && indexType != Microsoft::glTF::COMPONENT_UNSIGNED_INT &&
			indexType != Microsoft::glTF::COMPONENT_UNSIGNED_SHORT &&
			indexType != Microsoft::glTF::COMPONENT_UNSIGNED_BYTE && indexType != Microsoft::glTF::COMPONENT_BYTE)
			continue;
		auto vertexAccessor = primitive->getPositionAccessor();
		if (vertexAccessor->getComponentType() != Microsoft::glTF::COMPONENT_FLOAT) continue;
		MeshMetadata metadata;
		DrawElementsIndirectCommand cmd;
		cmd.firstIndex = static_cast<unsigned int>(indices.size());
		if (indexType == Microsoft::glTF::COMPONENT_SHORT) {
			auto &data = indexAccessor->getInt16Data();
			cmd.count = static_cast<unsigned int>(data.size());
			indices.insert(indices.end(), data.begin(), data.end());
		} else if (indexType == Microsoft::glTF::COMPONENT_UNSIGNED_SHORT) {
			auto &data = indexAccessor->getUint16Data();
			cmd.count = static_cast<unsigned int>(data.size());
			indices.insert(indices.end(), data.begin(), data.end());
		} else if (indexType == Microsoft::glTF::COMPONENT_UNSIGNED_INT) {
			auto &data = indexAccessor->getUint32Data();
			cmd.count = static_cast<unsigned int>(data.size());
			indices.insert(indices.end(), data.begin(), data.end());
		} else if (indexType == Microsoft::glTF::COMPONENT_UNSIGNED_BYTE) {
			auto &data = indexAccessor->getUint8Data();
			cmd.count = static_cast<unsigned int>(data.size());
			indices.insert(indices.end(), data.begin(), data.end());
		} else if (indexType == Microsoft::glTF::COMPONENT_BYTE) {
			auto &data = indexAccessor->getInt8Data();
			cmd.count = static_cast<unsigned int>(data.size());
			indices.insert(indices.end(), data.begin(), data.end());
		}
		eboDirty = true;

		if (vertexAccessor->getComponentType() == Microsoft::glTF::COMPONENT_FLOAT) {
			auto &data = vertexAccessor->getFloatData();
			cmd.baseVertex = /*static_cast<int>(
				vertexAccessor->getCount())*/
				static_cast<int>(vbo.size() / vertexAccessor->getTypeCount());

			vbo.insertElements(vbo.end(), data.begin(), data.end());
		}

		if (auto accessor = primitive->getUvAccessor();
			accessor && accessor->getComponentType() == Microsoft::glTF::COMPONENT_FLOAT) {
			auto &data = accessor->getFloatData();
			metadata.uvCount = static_cast<uint32_t>(data.size() / 2);
			metadata.uvStartPos = static_cast<uint32_t>(cmd.baseVertex) - static_cast<uint32_t>(uvSsbo.size());
			for (auto iterBegin = data.begin(), iterEnd = data.end(); iterBegin != iterEnd;) {
				glm::vec2 val;
				val.x = static_cast<float>(*iterBegin);
				++iterBegin;
				if (iterBegin == iterEnd) break;
				val.y = static_cast<float>(*iterBegin);
				++iterBegin;
				uvSsbo.addElement(val);
			}
		}
		if (auto accessor = primitive->getNormalAccessor();
			accessor && accessor->getComponentType() == Microsoft::glTF::COMPONENT_FLOAT) {
			auto &data = accessor->getFloatData();
			normalsVbo.insertElements(normalsVbo.end(), data.begin(), data.end());
		}
		if (auto accessor = primitive->getColorAccessor();
			accessor && accessor->getComponentType() == Microsoft::glTF::COMPONENT_FLOAT) {
			auto &data = accessor->getFloatData();
			metadata.colorCount = static_cast<uint32_t>(data.size() / 4);
			metadata.colorStartPos = static_cast<uint32_t>(cmd.baseVertex) - static_cast<uint32_t>(colorSsbo.size());
			for (auto iterBegin = data.begin(), iterEnd = data.end(); iterBegin != iterEnd;) {
				glm::vec4 val;
				val.x = static_cast<float>(*iterBegin);
				if (++iterBegin == iterEnd) break;
				val.y = static_cast<float>(*iterBegin);
				if (++iterBegin == iterEnd) break;
				val.z = static_cast<float>(*iterBegin);
				if (++iterBegin == iterEnd) break;
				val.w = static_cast<float>(*iterBegin);
				++iterBegin;
				colorSsbo.addElement(val);
			}
		}
		info.commandIndices.emplace_back(commands.size());

		commands.emplace_back(cmd);
		info.materialChangedConnections.emplace_back(primitive->connectOnMaterialChangedSignal(
			sigc::bind(sigc::mem_fun(*this, &Renderer::onPrimitiveMaterialChanged), pNewMesh.get(), primitive.get())));
		if (auto mat = primitive->getMaterial())
			metadata.materialId = static_cast<uint32_t>(materialToIndexMap.at(mat.get()));
		info.metadataIds.emplace_back(meshMetadataSsbo.size());
		meshMetadataSsbo.addElement(metadata);
	}

	meshes.emplace_back(pNewMesh);
	meshToInfoMap.emplace(pNewMesh.get(), info);
}

void Renderer::removeMesh(Mesh* /*pMesh*/) { throw std::runtime_error("Renderer::removeMesh(): Not implemented yet"); }

void Renderer::removeAllMeshes() {
	meshes.clear();
	meshMetadataSsbo.clear();
	colorSsbo.clear();
	normalsVbo.clear();
	uvSsbo.clear();
	vbo.clear();
	indices.clear();
}

void Renderer::updateBuffers() {
	if (eboDirty && indicesMutex.try_lock()) {
		if (ebo) glDeleteBuffers(1, &ebo);
		glCreateBuffers(1, &ebo);
		glNamedBufferData(ebo, static_cast<GLsizeiptr>(indices.size() * sizeof(uint16_t)), indices.data(),
						  GL_STATIC_DRAW);
		glVertexArrayElementBuffer(vao, ebo);
		eboDirty = false;
		indicesMutex.unlock();
	}

	if (!uninitializedMaterials.empty()) {
		if (!uninitializedMaterialsMutex.try_lock()) {

			for (auto uninitializedMaterial: uninitializedMaterials) { uninitializedMaterial->initialize(); }
			uninitializedMaterials.clear();
			uninitializedMaterialsMutex.unlock();
		}
	}
	if (vbo.tryUpdate()) vbo.bindVertexArray(vao, 0);
	if (normalsVbo.tryUpdate()) normalsVbo.bindVertexArray(vao, 3);
	if (materialsSsbo.tryUpdate()) materialsSsbo.bindBufferBase(2);
	if (meshMetadataSsbo.tryUpdate()) meshMetadataSsbo.bindBufferBase(4);
	if (uvSsbo.tryUpdate()) uvSsbo.bindBufferBase(5);
	if (!colorSsbo.empty() && colorSsbo.tryUpdate()) colorSsbo.bindBufferBase(7);
}

std::vector<DrawElementsIndirectCommand> Renderer::getCommandsByInfo(const MeshInfo &pMeshInfo) {
	std::vector<DrawElementsIndirectCommand> out;
	for (uint64_t commandIndex: pMeshInfo.commandIndices) out.emplace_back(commands.at(commandIndex));
	return out;
}

std::optional<MeshInfo> Renderer::getMeshInfo(Mesh* pMesh) {
	auto iter = meshToInfoMap.find(pMesh);
	if (iter == meshToInfoMap.end()) { return std::nullopt; }
	return iter->second;
}

void Renderer::executeRenderPass(const std::string &pPassName) {
	if (!isInited()) initialize();
	auto renderPass = getRenderPass(pPassName);
	if (!renderPass) return;
	glBindVertexArray(vao);
	updateBuffers();
	renderPass->render();
	glBindVertexArray(0);
}

ReportMessagePtr Renderer::onInitialize() {
	glCreateVertexArrays(1, &vao);


	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);
	glEnableVertexArrayAttrib(vao, 3);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 1);
	glVertexArrayAttribBinding(vao, 2, 2);
	glVertexArrayAttribBinding(vao, 3, 3);

	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, false, 0);

	glVertexArrayAttribIFormat(vao, 1, 1, GL_UNSIGNED_INT, 0);
	glVertexArrayBindingDivisor(vao, 1, 1);

	glVertexArrayAttribIFormat(vao, 2, 1, GL_UNSIGNED_INT, 0);
	glVertexArrayBindingDivisor(vao, 2, 1);

	glVertexArrayAttribFormat(vao, 3, 3, GL_FLOAT, false, 0);
	return Initializable::onInitialize();
}

void Renderer::onUninitialize() {

	if (vao) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	vbo.unintialize();
	if (ebo) {
		glDeleteBuffers(1, &ebo);
		ebo = 0;
	}
	materialsSsbo.unintialize();
	meshMetadataSsbo.unintialize();
	uvSsbo.unintialize();
	normalsVbo.unintialize();
	colorSsbo.unintialize();
}

void Renderer::onPrimitiveMaterialChanged(const std::shared_ptr<Material> &pNewMaterial, Mesh* pMesh,
										  Primitive* pPrimitive) {
	auto info = getMeshInfo(pMesh);
	if (!info.has_value()) return;
	auto matIter = materialToIndexMap.find(pNewMaterial.get());
	if (matIter == materialToIndexMap.end()) return;

	auto &primitives = pMesh->getPrimitives();
	auto foundAtIter = std::ranges::find_if(
		primitives, [pPrimitive](const std::shared_ptr<Primitive> &pElement) { return pElement.get() == pPrimitive; });
	auto index = static_cast<uint64_t>(foundAtIter - primitives.begin());
	if (info->metadataIds.size() <= index) return;
	MeshMetadata &metadata = meshMetadataSsbo.getElement(info->metadataIds.at(index));
	metadata.materialId = static_cast<uint32_t>(matIter->second);
	meshMetadataSsbo.markDirty();
}

void RenderPass::addMeshInstance(Mesh* pMesh, MeshInstance* pMeshInstance) {
	auto iter = meshesToRender.find(pMesh);
	if (iter == meshesToRender.end()) {
		iter = meshesToRender.emplace_hint(iter, pMesh, 0);
		commandsBufferDirty = true;
	}

	auto connection = pMeshInstance->connectDataChanged([this](MeshInstance* pChangedInstance) {
		const auto &val = instances.at(pChangedInstance);
		meshInstanceSsbo.setElement(val.first, pChangedInstance->getData());
	});
	auto newIndex = meshInstanceSsbo.size();
	iter->second.emplace_back(newIndex);
	instances.emplace(pMeshInstance, std::make_pair(newIndex, connection));
	meshInstanceSsbo.addElement(pMeshInstance->getData());

	/*{
		std::lock_guard lock(commandsBufferMutex);

		for (auto commandsByInfo: renderer->getCommandsByInfo(iter->second)) {
			commandsByInfo.instanceCount = 1;
			commands.emplace_back(commandsByInfo);
		}
	}*/
}

void RenderPass::removeMeshInstance(Mesh* pMesh, MeshInstance* pMeshInstance) {
	std::lock_guard lock(commandsBufferMutex);
	auto iter = meshesToRender.find(pMesh);
	if (iter == meshesToRender.end()) { return; }
	auto instIter = instances.find(pMeshInstance);
	if (instIter == instances.end()) { return; }
	auto &instanceInfo = instIter->second;

	erase(iter->second, instanceInfo.first);
	if (iter->second.empty()) { meshesToRender.erase(iter); }
	/*meshInstanceSsbo.removeElement(meshInstanceSsbo.begin() + static_cast<long int>(instanceInfo.first));

	for (auto &[instance, instanceData]: instances) {
		if (instanceData.first > instanceInfo.first) instanceData.first--;
	}
	for (auto &mesh: meshesToRender) {
		for (unsigned long &second: mesh.second)
			if (second > instanceInfo.first) second--;
	}*/
	instances.erase(instIter);
	commandsBufferDirty = true;
}

void RenderPass::removeAllMeshInstances() {
	commands.clear();
	instances.clear();
	meshInstanceSsbo.clear();
}

void RenderPass::render() {
	if (meshesToRender.empty()) return;
	static int i = 0;
	if (commandsBufferDirty && commandsBufferMutex.try_lock()) {
		commands.clear();
		instanceIndices.clear();
		meshIndices.clear();
		/*for (const auto &[instance, data]: instances) {
			auto mesh = instance->getMesh().get();
			auto info = renderer->getMeshInfo(mesh);
			if (!info.has_value()) continue;
			auto instances for (auto commandsByInfo: renderer->getCommandsByInfo(info.value())) {
				commandsByInfo.instanceCount = mesh.second.size();
				commands.emplace_back(commandsByInfo);
			}
		}*/
		for (auto &mesh: meshesToRender) {
			if (mesh.second.empty()) continue;
			auto info = renderer->getMeshInfo(mesh.first);
			if (!info.has_value()) continue;
			meshIndices.insert(meshIndices.cend(), info->metadataIds.begin(), info->metadataIds.end());
			for (auto commandsByInfo: renderer->getCommandsByInfo(info.value())) {
				commandsByInfo.instanceCount = static_cast<unsigned int>(mesh.second.size());
				commandsByInfo.baseInstance = static_cast<unsigned int>(instanceIndices.size());
				commands.emplace_back(commandsByInfo);
			}
			for (unsigned long instanceId: mesh.second) { instanceIndices.emplace_back(instanceId); }
			i++;
			//if (i > 0) break;
		}
		if (drawCmdBuffer) glDeleteBuffers(1, &drawCmdBuffer);
		glCreateBuffers(1, &drawCmdBuffer);
		glNamedBufferStorage(drawCmdBuffer,
							 static_cast<GLsizeiptr>(sizeof(DrawElementsIndirectCommand) * commands.size()),
							 static_cast<const void*>(commands.data()), GL_DYNAMIC_STORAGE_BIT);
		commandsBufferDirty = false;
		commandsBufferSize = commands.size();
		commandsBufferMutex.unlock();

		if (instanceIndicesBuffer) glDeleteBuffers(1, &instanceIndicesBuffer);
		glCreateBuffers(1, &instanceIndicesBuffer);
		glNamedBufferData(instanceIndicesBuffer, static_cast<GLsizeiptr>(sizeof(uint32_t) * instanceIndices.size()),
						  static_cast<const void*>(instanceIndices.data()), GL_STATIC_DRAW);

		if (meshIndicesBuffer) glDeleteBuffers(1, &meshIndicesBuffer);
		glCreateBuffers(1, &meshIndicesBuffer);
		glNamedBufferData(meshIndicesBuffer, static_cast<GLsizeiptr>(sizeof(uint32_t) * meshIndices.size()),
						  static_cast<const void*>(meshIndices.data()), GL_STATIC_DRAW);
		meshIndicesBufferDirty = false;


	} else
		i = 0;

	glVertexArrayVertexBuffer(renderer->getVao(), 1, instanceIndicesBuffer, 0, sizeof(uint32_t));
	glVertexArrayVertexBuffer(renderer->getVao(), 2, meshIndicesBuffer, 0, sizeof(uint32_t));

	meshInstanceSsbo.update();
	meshInstanceSsbo.bindBufferBase(1);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawCmdBuffer);
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, nullptr, static_cast<GLsizei>(commandsBufferSize), 0);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}

ReportMessagePtr RenderPass::onInitialize() {
	//


	return Initializable::onInitialize();
}

void RenderPass::onUninitialize() {
	if (drawCmdBuffer) {
		glDeleteBuffers(1, &drawCmdBuffer);
		drawCmdBuffer = 0;
	}
	if (instanceIndicesBuffer) {
		glDeleteBuffers(1, &instanceIndicesBuffer);
		instanceIndicesBuffer = 0;
	}
	meshInstanceSsbo.unintialize();
}

} // namespace mer::sdk