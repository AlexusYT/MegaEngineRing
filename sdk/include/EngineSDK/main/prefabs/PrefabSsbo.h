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
// Created by alexus on 06.12.24.
//

#ifndef PREFABSSBO_H
#define PREFABSSBO_H
#include <memory>
#include <unordered_map>
#include <vector>

#include "EngineSDK/main/render/IRenderable.h"
#include "EngineSDK/renderer/buffers/SSBO.h"

namespace sigc {
struct scoped_connection;
}

namespace mer::sdk::main {
struct PrefabInstanceData;
class PrefabInstance;

class PrefabSsbo : public renderer::SSBO, public IRenderable {
	std::unordered_map<PrefabInstance*, sigc::scoped_connection> instances;
	std::vector<PrefabInstanceData> instancesData;
	void* ssboData{};
	int64_t dataSize{};

	bool dirty{true};

	PrefabSsbo();

public:
	static std::shared_ptr<PrefabSsbo> create();

	void trackInstance(PrefabInstance* pInstance);

	void untrackInstance(PrefabInstance* pInstance);

	void render() override;

private:
	void onDataChanged();
};

} // namespace mer::sdk::main

#endif //PREFABSSBO_H