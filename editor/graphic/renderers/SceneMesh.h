//
// Created by alexus on 09.12.23.
//

#ifndef SCENEMESH_H
#define SCENEMESH_H
#if 0

	#include "SceneMeshBase.h"
	#include "SceneMeshFwd.h"
	#include "graphic/viewport/OpenGL.h"

class SceneMesh : public SceneMeshBase {
	OpenGL* render;

	SceneMesh() = default;

public:
	static SceneMeshUPtr create() { return SceneMeshUPtr(new (std::nothrow) SceneMesh()); }

	void init();

	void update();

protected:
	void getColors(std::vector<glm::vec4> &newColors) override;
	void getIndices(std::vector<uint16_t> &newIndices) override;
	void getNormals(std::vector<glm::vec3> &newNormals) override;
	void getUVs(std::vector<glm::vec2> &newUVs) override;
	void getVertices(std::vector<glm::vec3> &newVertices) override;

public:
	[[nodiscard]] OpenGL* getRender() const { return render; }

	void setRender(OpenGL* newRender) { this->render = newRender; }
};
#endif


#endif //SCENEMESH_H
