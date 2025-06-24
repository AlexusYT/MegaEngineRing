//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 18.06.2025.
//

#include "KwasarEngine/render/RenderPass.h"

#include <epoxy/gl.h>

#include "KwasarEngine/extensions/LightExtension.h"
#include "KwasarEngine/extensions/MeshExtension.h"
#include "KwasarEngine/gltf/Node.h"
#include "KwasarEngine/render/DrawElementsIndirectCommand.h"
#include "KwasarEngine/render/Renderer.h"

namespace ke {

RenderPass::RenderPass() { litByInstancesSsbo.addElement(0); }

void RenderPass::addNode(Node* pNode) {
	bool shouldAddInstance{};
	auto newIndex = meshInstanceSsbo.size();
	if (auto modelExt = pNode->getExtension<MeshExtension>()) {
		shouldAddInstance = true;
		auto mesh = modelExt->mesh.getValue().get();
		auto iter = meshesToRender.find(mesh);
		if (iter == meshesToRender.end()) { iter = meshesToRender.emplace_hint(iter, mesh, 0); }

		iter->second.emplace_back(newIndex);
		commandsBufferDirty = true;
	}
	if ([[maybe_unused]] auto modelExt = pNode->getExtension<LightExtension>()) {
		shouldAddInstance = true;

		litByInstancesSsbo.setElement(0, static_cast<const int &>(litByInstancesSsbo.size()));
		litByInstancesSsbo.addElement(static_cast<const int &>(newIndex));
	}
	if (!shouldAddInstance) return;
	auto connection = pNode->connectDataChanged([this](Node* pChangedInstance) {
		const auto &val = instances.at(pChangedInstance);
		meshInstanceSsbo.setElement(val.first, pChangedInstance->getData());
	});
	instances.emplace(pNode, std::make_pair(newIndex, connection));
	meshInstanceSsbo.addElement(pNode->getData());
}

void RenderPass::removeNode(Node* pNode) {
	auto instIter = instances.find(pNode);
	if (instIter == instances.end()) { return; }
	auto instanceIndex = instIter->second.first;
	bool shouldRemoveInstance{};
	if (auto modelExt = pNode->getExtension<MeshExtension>()) {
		shouldRemoveInstance = true;
		if (const auto iter = meshesToRender.find(modelExt->mesh.getValue().get()); iter != meshesToRender.end()) {
			erase(iter->second, instanceIndex);
			if (iter->second.empty()) { meshesToRender.erase(iter); }
			commandsBufferDirty = true;
		}
		for (auto &meshes: meshesToRender) {
			for (auto &index: meshes.second) { if (index > instanceIndex) { index--; } }
		}
	}

	if ([[maybe_unused]] auto modelExt = pNode->getExtension<LightExtension>()) {
		shouldRemoveInstance = true;
		auto litIter = litByInstancesSsbo.find(litByInstancesSsbo.begin() + 1, litByInstancesSsbo.end(),
											   static_cast<const int &>(instanceIndex));
		if (litIter != litByInstancesSsbo.end()) litByInstancesSsbo.removeElement(litIter);
		litByInstancesSsbo.setElement(0, static_cast<const int &>(litByInstancesSsbo.size()));
	}
	if (!shouldRemoveInstance) return;

	meshInstanceSsbo.removeElement(meshInstanceSsbo.begin() + static_cast<long int>(instanceIndex));
	for (auto &instance: instances) { if (instance.second.first > instanceIndex) { instance.second.first--; } }
	for (bool firstSkipped = false; auto &index: litByInstancesSsbo) {
		if (!firstSkipped) {
			firstSkipped = true;
			continue;
		}

		if (index > static_cast<int32_t>(instanceIndex)) { index--; }
	}
	litByInstancesSsbo.markDirty();

	instances.erase(instIter);
}

void RenderPass::removeAllMeshInstances() {
	commands.clear();
	instances.clear();
	meshInstanceSsbo.clear();
}

void RenderPass::changeMesh(Node* pNode, Mesh* pNewMesh) {
	auto ext = pNode->getExtension<MeshExtension>();
	if (!ext) return;
	auto instIter = instances.find(pNode);
	if (instIter == instances.end()) return;
	std::lock_guard lock(commandsBufferMutex);

	bool isDirty{};
	if (const auto oldMesh = ext->mesh.getValue().get()) {
		if (const auto oldMeshIter = meshesToRender.find(oldMesh); oldMeshIter != meshesToRender.end()) {
			erase(oldMeshIter->second, instIter->second.first);
			if (oldMeshIter->second.empty()) { meshesToRender.erase(oldMeshIter); }

			for (auto &meshes: meshesToRender) {
				for (auto &index: meshes.second) { if (index > instIter->second.first) { index--; } }
			}
			isDirty = true;
		}
	}
	if (pNewMesh) {
		auto newMeshIter = meshesToRender.find(pNewMesh);
		if (newMeshIter == meshesToRender.end()) newMeshIter = meshesToRender.emplace(pNewMesh, 0).first;
		newMeshIter->second.emplace_back(instIter->second.first);
		isDirty = true;
	}
	commandsBufferDirty = isDirty;
}

void RenderPass::render() {
	if (meshesToRender.empty()) return;
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
			for (auto instanceId: mesh.second) { instanceIndices.emplace_back(instanceId); }
		}
		if (drawCmdBuffer)
			glDeleteBuffers(1, &drawCmdBuffer);
		glCreateBuffers(1, &drawCmdBuffer);
		glNamedBufferStorage(drawCmdBuffer,
							 static_cast<GLsizeiptr>(sizeof(DrawElementsIndirectCommand) * commands.size()),
							 static_cast<const void*>(commands.data()), GL_DYNAMIC_STORAGE_BIT);
		commandsBufferDirty = false;
		commandsBufferSize = commands.size();
		commandsBufferMutex.unlock();

		if (instanceIndicesBuffer)
			glDeleteBuffers(1, &instanceIndicesBuffer);
		glCreateBuffers(1, &instanceIndicesBuffer);
		glNamedBufferData(instanceIndicesBuffer, static_cast<GLsizeiptr>(sizeof(uint32_t) * instanceIndices.size()),
						  static_cast<const void*>(instanceIndices.data()), GL_STATIC_DRAW);

		if (meshIndicesBuffer)
			glDeleteBuffers(1, &meshIndicesBuffer);
		glCreateBuffers(1, &meshIndicesBuffer);
		glNamedBufferData(meshIndicesBuffer, static_cast<GLsizeiptr>(sizeof(uint32_t) * meshIndices.size()),
						  static_cast<const void*>(meshIndices.data()), GL_STATIC_DRAW);
		meshIndicesBufferDirty = false;
	}

	glVertexArrayVertexBuffer(renderer->getVao(), 1, instanceIndicesBuffer, 0, sizeof(uint32_t));
	glVertexArrayVertexBuffer(renderer->getVao(), 2, meshIndicesBuffer, 0, sizeof(uint32_t));

	meshInstanceSsbo.update();
	meshInstanceSsbo.bindBufferBase(1);
	if (litByInstancesSsbo.tryUpdate()) litByInstancesSsbo.bindBufferBase(6);
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
} // ke