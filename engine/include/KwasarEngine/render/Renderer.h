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
#include <KwasarEngine/buffers/SSBO.h>
#include <KwasarEngine/gltf/Material.h>
#include <sigc++/scoped_connection.h>

#include "DrawElementsIndirectCommand.h"
#include "Initializable.h"
#include "IRenderable.h"
#include "KwasarEngine/buffers/VertexBufferObject.h"
#include "KwasarEngine/gltf/Light.h"

namespace ke {
class ProgramWideShaderBuffer;
class Node;
class Mesh;
class Primitive;
class RenderPass;

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

/**
 * @brief You can inherit this class to customize your geometry rendering process.
 *
 * Override method render() from IRenderable in your Renderer class:
 *
 * @code
 *	class CustomRenderer : public Renderer{
 *	public:
 *		void render() override{
 *			//Call executeMainPass or executeRenderPass to run render passes.
 *			//Also, you can switch opengl state here.
 *		}
 *	}
 * @endcode
 * @see DefaultRenderer for renderer that used by default
 */
class Renderer : public Initializable, public IRenderable {
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

protected:
	std::shared_ptr<ProgramWideShaderBuffer> programBuffer;

	Renderer();

public:
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

	[[nodiscard]] const std::shared_ptr<ProgramWideShaderBuffer> &getProgramBuffer() const { return programBuffer; }

	void addRenderPass(const std::string &pName, const std::shared_ptr<RenderPass> &pPass);

	void removeRenderPass(const std::string &pName);

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
	void addMainPass(const std::shared_ptr<RenderPass> &pPass) { addRenderPass(getMainPassName(), pPass); }

	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

	void onPrimitiveMaterialChanged(const std::shared_ptr<Material> &pNewMaterial, Mesh* pMesh, Primitive* pPrimitive);
};
} // namespace ke

#endif //RENDERER_H
