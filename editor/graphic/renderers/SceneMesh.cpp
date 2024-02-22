//
// Created by alexus on 09.12.23.
//
#if 0

	#include "SceneMesh.h"

	#include <graphic/ProgramWideSettings.h>

	#include "Render/opengl/buffers/VertexArrayObject.h"
	#include "graphic/UIUtils.h"

void SceneMesh::init() { initBuffers(); }

void SceneMesh::update() {
	SceneMeshBase::updateBuffers();

	getRenderBuffer()->drawElementsBind(RenderIndicesBuffer::info.data.size());
}

inline void SceneMesh::getColors(std::vector<glm::vec4> &newColors) { newColors.resize(8); }

inline void SceneMesh::getIndices(std::vector<uint16_t> &newIndices) { UIUtils::getCubeIndices(newIndices); }

inline void SceneMesh::getNormals(std::vector<glm::vec3> &newNormals) { newNormals.resize(8); }

inline void SceneMesh::getUVs(std::vector<glm::vec2> &newUVs) { newUVs.resize(8); }

inline void SceneMesh::getVertices(std::vector<glm::vec3> &newVertices) {
	UIUtils::getCubeVertices(newVertices, {-10, -10, -10}, {10, 10, 10});
}
#endif
