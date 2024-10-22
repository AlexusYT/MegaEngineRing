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

#ifndef IMODEL3DOBJECT_H
#define IMODEL3DOBJECT_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

#include "EngineSDK/main/render/IRenderable.h"
#include "EngineSDK/renderer/shaders/ShaderProgram.h"

namespace mer::sdk::main {
class IModel3DResource;
class IRenderInstance;

class IModel3DObject : public IRenderable {
public:
	~IModel3DObject() override = default;

	virtual void addRenderInstance(IRenderInstance* pNewInstance) = 0;

	virtual void removeRenderInstance(IRenderInstance* pOldInstance) = 0;

	[[nodiscard]] virtual const std::vector<glm::vec3> &getVertices() const = 0;

	virtual void setVertices(const std::vector<glm::vec3> &pVertices) = 0;

	[[nodiscard]] virtual const std::vector<glm::vec2> &getUvs() const = 0;

	virtual void setUvs(const std::vector<glm::vec2> &pUvs) = 0;

	[[nodiscard]] virtual const std::vector<glm::vec3> &getNormals() const = 0;

	virtual void setNormals(const std::vector<glm::vec3> &pNormals) = 0;

	[[nodiscard]] virtual const std::vector<uint16_t> &getIndices() const = 0;

	virtual void setIndices(const std::vector<uint16_t> &pIndices) = 0;

	[[nodiscard]] virtual const std::string &getName() const = 0;

	virtual void setName(const std::string &pName) = 0;

	[[nodiscard]] virtual IModel3DResource* getIModelResource() const = 0;

	virtual void setIModelResource(IModel3DResource* pResource) = 0;

	virtual void setShader(const std::shared_ptr<renderer::ShaderProgram> &pShader) = 0;

	virtual const std::shared_ptr<renderer::ShaderProgram> &getShader() const = 0;

	virtual bool operator<(const IModel3DObject &pElement) const = 0;
};

} // namespace mer::sdk::main

#endif //IMODEL3DOBJECT_H
