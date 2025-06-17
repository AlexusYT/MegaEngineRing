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
// Created by alexus on 02.10.24.
//

#ifndef MODEL3DOBJECT_H
#define MODEL3DOBJECT_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "KwasarEngine/render/IInstancedRender.h"
#include "KwasarEngine/render/Initializable.h"
#include "IModel3DObject.h"

namespace ke {
class SSBO;
class Model3DResource;
class RenderInstanceData;
class IRenderInstance;

class Model3DObject : public IModel3DObject, public IInstancedRender, public virtual Initializable {
	IModel3DResource* resource;
	uint32_t dataBuffer{};
	std::vector<float> data;
	uint32_t indexBuffer{};
	std::vector<uint16_t> indices;
	std::string name;

	uint32_t vao{};

	std::vector<IRenderInstance*> instances;
	std::unordered_map<std::shared_ptr<ShaderProgram>, std::vector<RenderInstanceData>> instancesData;
	std::shared_ptr<ShaderProgram> shader;
	std::shared_ptr<SSBO> instancesSsbo;

	Model3DObject() = default;

public:
	~Model3DObject() override;

	static std::shared_ptr<IModel3DObject> create();

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

public:
	void render() override;


	void addRenderInstance(IRenderInstance* pNewInstance) override;

	void removeRenderInstance(IRenderInstance* pOldInstance) override;

	[[nodiscard]] const std::vector<float> &getData() const override { return data; }

	void setData(const std::vector<float> &pData) override { data = pData; }

	[[nodiscard]] const std::vector<uint16_t> &getIndices() const override { return indices; }

	void setIndices(const std::vector<uint16_t> &pIndices) override { indices = pIndices; }

	[[nodiscard]] const std::string &getName() const override { return name; }

	void setName(const std::string &pName) override { name = pName; }

	[[nodiscard]] IModel3DResource* getIModelResource() const override { return resource; }

	void setIModelResource(IModel3DResource* pResource) override { resource = pResource; }

	void setShader(const std::shared_ptr<ShaderProgram> &pShader) override { shader = pShader; }

	const std::shared_ptr<ShaderProgram> &getShader() const override { return shader; }

	bool operator<(const IModel3DObject &pRhs) const override;

private:
	void onInstanceDataChanged(IRenderInstance* pInstance) override;

	void onMaterialDataChanged(RenderInstance* pInstance) override;
};
} // namespace ke

#endif //MODEL3DOBJECT_H