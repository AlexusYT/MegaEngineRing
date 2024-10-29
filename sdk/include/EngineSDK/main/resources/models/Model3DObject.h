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

#ifndef MODEL3DOBJECT_H
#define MODEL3DOBJECT_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "EngineSDK/main/render/IInstancedRender.h"
#include "IModel3DObject.h"

namespace mer::sdk::main {
class Model3DResource;
}

namespace mer::sdk::main {
class RenderInstanceData;
class IRenderInstance;

class Model3DObject : public IModel3DObject, public IInstancedRender {
	IModel3DResource* resource;
	uint32_t vertexBuffer{};
	std::vector<glm::vec3> vertices;
	uint32_t uvBuffer{};
	std::vector<glm::vec2> uvs;
	uint32_t normalsBuffer{};
	std::vector<glm::vec3> normals;
	uint32_t indexBuffer{};
	std::vector<uint16_t> indices;
	std::string name;

	uint32_t vao{};

	std::vector<IRenderInstance*> instances;
	std::unordered_map<std::shared_ptr<renderer::ShaderProgram>,
					   std::pair<std::vector<RenderInstanceData>, std::vector<MaterialData>>>
		instancesData;
	std::shared_ptr<renderer::ShaderProgram> shader;
	std::shared_ptr<renderer::ISSBO> instancesSsbo;
	std::shared_ptr<renderer::ISSBO> materialsSsbo;

	Model3DObject() = default;

public:
	~Model3DObject() override;

	static std::shared_ptr<IModel3DObject> create();

	void setupRender() override;

	void render() override;

	void destroyRender() override;

	void addRenderInstance(IRenderInstance* pNewInstance) override;

	void removeRenderInstance(IRenderInstance* pOldInstance) override;

	[[nodiscard]] const std::vector<glm::vec3> &getVertices() const override { return vertices; }

	void setVertices(const std::vector<glm::vec3> &pVertices) override { vertices = pVertices; }

	[[nodiscard]] const std::vector<glm::vec2> &getUvs() const override { return uvs; }

	void setUvs(const std::vector<glm::vec2> &pUvs) override { uvs = pUvs; }

	[[nodiscard]] const std::vector<glm::vec3> &getNormals() const override { return normals; }

	void setNormals(const std::vector<glm::vec3> &pNormals) override { normals = pNormals; }

	[[nodiscard]] const std::vector<uint16_t> &getIndices() const override { return indices; }

	void setIndices(const std::vector<uint16_t> &pIndices) override { indices = pIndices; }

	[[nodiscard]] const std::string &getName() const override { return name; }

	void setName(const std::string &pName) override { name = pName; }

	[[nodiscard]] IModel3DResource* getIModelResource() const override { return resource; }

	void setIModelResource(IModel3DResource* pResource) override { resource = pResource; }

	void setShader(const std::shared_ptr<renderer::ShaderProgram> &pShader) override { shader = pShader; }

	const std::shared_ptr<renderer::ShaderProgram> &getShader() const override { return shader; }

	bool operator<(const IModel3DObject &pRhs) const override;


private:
	void onInstanceDataChanged(IRenderInstance* pInstance) override;

	void onMaterialDataChanged(RenderInstance* pInstance) override;
};


} // namespace mer::sdk::main

#endif //MODEL3DOBJECT_H
