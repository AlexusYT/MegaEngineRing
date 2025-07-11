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
// Created by alexus on 14.03.25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>

#include "KwasarEngine/buffers/SSBO.h"
#include "KwasarEngine/buffers/VertexBufferObject.h"
#include "KwasarEngine/gltf/Light.h"
#include "KwasarEngine/gltf/Material.h"
#include "KwasarEngine/gltf/Node.h"
#include "Initializable.h"

namespace ke {
class Light;
class Primitive;
class Renderer;
class MeshInstanceSsbo;
class Mesh;

struct DrawElementsIndirectCommand {
	//number of indices
	unsigned int count{};
	unsigned int instanceCount{1};
	//an index (not byte) offset into the bound element array buffer to start reading index data.
	unsigned int firstIndex{};
	//interpreted as an addition to whatever index is read from the element array buffer.
	int baseVertex{};
	unsigned int baseInstance{};
};

class RenderPass : public Initializable {
	friend class Renderer;
	Renderer* renderer{};
	std::unordered_map<Mesh*, std::vector<size_t> /*instancesIds*/> meshesToRender;
	uint32_t drawCmdBuffer{};
	std::vector<DrawElementsIndirectCommand> commands;
	bool commandsBufferDirty{true};
	std::mutex commandsBufferMutex;
	size_t commandsBufferSize{};

	std::vector<uint32_t> instanceIndices;
	uint32_t instanceIndicesBuffer{};
	bool instanceIndicesBufferDirty{true};
	std::vector<uint32_t> meshIndices;
	uint32_t meshIndicesBuffer{};
	bool meshIndicesBufferDirty{true};

	Ssbo<std::vector<MeshInstanceData>> meshInstanceSsbo;
	std::unordered_map<Node*, std::pair<size_t /*index*/, sigc::scoped_connection>> instances;
	Ssbo<std::vector<int32_t>> litByInstancesSsbo;

public:
	RenderPass();


	void addNode(Node* pNode);

	void removeNode(Node* pNode);

	void removeAllMeshInstances();

	void changeMesh(Node* pNode, Mesh* pMesh);

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

	void render();

	void removeInstanceFromMesh(Node* pNode);
};

struct MeshMetadata {
	uint32_t uvStartPos{};
	uint32_t uvCount{};
	uint32_t colorStartPos{};
	uint32_t colorCount{};
	uint32_t materialId{};
	uint32_t instanceId{};
};

struct MeshInfo {
	std::vector<uint64_t> commandIndices;
	std::vector<uint64_t> metadataIds;
	std::vector<sigc::connection> materialChangedConnections;
};

class Renderer : public Initializable {
	std::vector<std::shared_ptr<Material>> materials;
	std::unordered_map<Material*, sigc::scoped_connection> materialsConnections;
	std::unordered_map<Material*, size_t /*index*/> materialToIndexMap;
	std::vector<Material*> uninitializedMaterials;
	std::mutex uninitializedMaterialsMutex;
	Ssbo<std::vector<MaterialData>> materialsSsbo;

	std::vector<std::shared_ptr<Mesh>> meshes;
	std::unordered_map<Mesh*, MeshInfo> meshToInfoMap;

	std::vector<DrawElementsIndirectCommand> commands;

	std::vector<std::shared_ptr<Light>> lights;
	std::unordered_map<Light*, sigc::scoped_connection> lightsConnections;
	std::unordered_map<Light*, size_t /*index*/> lightToIndexMap;
	Ssbo<std::vector<LightData>> lightsSsbo;

	std::vector<uint16_t> indices;
	uint32_t ebo{};
	bool eboDirty{true};
	std::mutex indicesMutex;

	VertexBufferObject<float, 3> vbo;
	VertexBufferObject<float, 3> normalsVbo;

	uint32_t vao{};
	Ssbo<std::vector<MeshMetadata>> meshMetadataSsbo;
	Ssbo<std::vector<glm::vec2>> uvSsbo;
	Ssbo<std::vector<glm::vec4>> colorSsbo;
	std::unordered_map<std::string, std::shared_ptr<RenderPass>> renderPasses;

public:
	Renderer();

	void addMaterial(const std::shared_ptr<Material> &pMaterial);

	void removeAllMaterials();

	void addMesh(const std::shared_ptr<Mesh> &pNewMesh);

	void removeMesh(Mesh* pMesh);

	void removeAllMeshes();

	void addLightSource(const std::shared_ptr<Light> &pNewLight);

	[[nodiscard]] const std::vector<std::shared_ptr<Light>> &getLights() const { return lights; }

	void updateBuffers();

	std::vector<DrawElementsIndirectCommand> getCommandsByInfo(const MeshInfo &pMeshInfo);

	std::optional<MeshInfo> getMeshInfo(Mesh* pMesh);

	void addRenderPass(const std::string &pName, const std::shared_ptr<RenderPass> &pPass) {
		auto iter = renderPasses.find(pName);
		if (iter != renderPasses.end()) return;
		pPass->renderer = this;
		renderPasses.emplace_hint(iter, pName, pPass);
	}

	void removeRenderPass(const std::string &pName) {
		auto iter = renderPasses.find(pName);
		if (iter == renderPasses.end()) return;
		iter->second->renderer = nullptr;
		renderPasses.erase(iter);
	}

	void executeMainPass() { executeRenderPass(getMainPassName()); }

	void executeRenderPass(const std::string &pPassName);

	std::shared_ptr<RenderPass> getRenderPass(const std::string &pName) const {
		auto iter = renderPasses.find(pName);
		if (iter == renderPasses.end()) return nullptr;
		return iter->second;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<Mesh>> &getMeshes() const { return meshes; }

	void changeMesh(Node* pNode, Mesh* pMesh);

	void removeNode(Node* pNode);

	static const char* getMainPassName() { return "MainPass"; }

	[[nodiscard]] std::shared_ptr<RenderPass> getMainRenderPass() const { return getRenderPass(getMainPassName()); }

	[[nodiscard]] uint32_t getVao() const { return vao; }

	[[nodiscard]] const std::vector<std::shared_ptr<Material>> &getMaterials() const { return materials; }

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

	void onPrimitiveMaterialChanged(const std::shared_ptr<Material> &pNewMaterial, Mesh* pMesh, Primitive* pPrimitive);
};
} // namespace ke

#endif //RENDERER_H