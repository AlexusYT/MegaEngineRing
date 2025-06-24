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

#ifndef RENDERPASS_H
#define RENDERPASS_H
#include <unordered_map>
#include <KwasarEngine/buffers/SSBO.h>
#include <KwasarEngine/gltf/Node.h>

#include "DrawElementsIndirectCommand.h"
#include "Initializable.h"


namespace sigc {
struct scoped_connection;
}

namespace ke {
class Renderer;
class Node;
class Mesh;

class RenderPass : public Initializable {
	friend Renderer;
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
} // ke

#endif //RENDERPASS_H
