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
// Created by alexus on 27.11.24.
//

#ifndef PREFABELEMENTSSSBO_H
#define PREFABELEMENTSSSBO_H
#include <unordered_map>

#include "EngineSDK/main/render/IRenderable.h"
#include "EngineSDK/renderer/buffers/SSBO.h"

namespace sigc {
struct scoped_connection;
}

namespace mer::sdk::main {
class PrefabElementInstance;
class PrefabElementInstanceData;
} // namespace mer::sdk::main

namespace mer::sdk::main {

class PrefabElementsSsbo : public renderer::SSBO, public IRenderable {
	std::unordered_map<PrefabElementInstance*, sigc::scoped_connection> instances;
	std::vector<PrefabElementInstanceData> instancesData;
	uint32_t elementsPerInstance{};
	void* ssboData{};
	int64_t dataSize{};

	bool dirty{true};

	PrefabElementsSsbo();

public:
	static std::shared_ptr<PrefabElementsSsbo> create();

	~PrefabElementsSsbo() override;

	void trackInstance(PrefabElementInstance* pInstance);

	void untrackInstance(PrefabElementInstance* pInstance);

	void untrackAll();

	void render() override;

	[[nodiscard]] uint32_t getElementsPerInstance() const { return elementsPerInstance; }

	void setElementsPerInstance(uint32_t pElementsPerInstance) {
		if (elementsPerInstance == pElementsPerInstance) return;
		elementsPerInstance = pElementsPerInstance;
		onDataChanged();
	}

private:
	void onDataChanged();
};


} // namespace mer::sdk::main

#endif //PREFABELEMENTSSSBO_H
