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

#ifndef PREFAB_H
#define PREFAB_H
#include <cstdint>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "EngineSDK/render/IRenderable.h"
#include "EngineSDK/render/Initializable.h"
#include "PrefabInstance.h"

namespace mer::sdk::main {
class ShaderProgram;
class PrefabSsbo;
class IMaterialResource;
class PrefabElementInstance;
class PrefabElementsSsbo;
class PrefabElement;
class PrefabInstance;

class Prefab : public Initializable, public IRenderable {
	std::unordered_map<std::string, std::shared_ptr<PrefabElement>> elements;
	std::list<PrefabElement*> elementsList;
	uint32_t dataBuffer{};
	std::vector<float> data;
	uint32_t indexBuffer{};
	std::vector<uint16_t> indices;
	bool buffersDirty{true};
	uint32_t vao{};

	std::vector<std::shared_ptr<PrefabInstance>> instances;
	std::shared_ptr<PrefabElementsSsbo> prefabElementsSsbo;
	std::shared_ptr<PrefabSsbo> prefabSsbo;
	std::shared_ptr<ShaderProgram> shaderProgram;

protected:
	Prefab();

public:
	static std::shared_ptr<Prefab> create();

	utils::Property<bool> visible;
	utils::Property<std::shared_ptr<IMaterialResource>> material;

	~Prefab() override;

	void addElement(const std::shared_ptr<PrefabElement> &pPrefabElement);

	void removeElement(const std::shared_ptr<PrefabElement> &pPrefabElement);

	std::shared_ptr<PrefabElement> getElement(const std::string &pElementName);

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<PrefabElement>> &getElements() const {
		return elements;
	}

	[[nodiscard]] const std::vector<float> &getData() const { return data; }

	[[nodiscard]] const std::vector<uint16_t> &getIndices() const { return indices; }

	std::shared_ptr<PrefabInstance> createInstance();

	void addInstance(const std::shared_ptr<PrefabInstance> &pInstance);

	[[nodiscard]] const std::shared_ptr<ShaderProgram> &getShaderProgram() const { return shaderProgram; }

	void setShaderProgram(const std::shared_ptr<ShaderProgram> &pShaderProgram) {
		shaderProgram = pShaderProgram;
	}

protected:
	utils::ReportMessagePtr onInitialize() override;

	void render() override;

	void onUninitialize() override;

private:
};


} // namespace mer::sdk::main

#endif //PREFAB_H
