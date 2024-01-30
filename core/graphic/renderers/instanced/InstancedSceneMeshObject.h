//
// Created by alexus on 30.07.23.
//

#ifndef MAIN_INSTANCEDSCENEMESHOBJECT_H
#define MAIN_INSTANCEDSCENEMESHOBJECT_H

#include <Render/engine/models/Model.h>
#include <Render/engine/models/ModelObject.h>

#include "InstancedSceneMesh.h"

class InstancedSceneMeshObject : public InstancedSceneMesh {
	rel::engine::ModelObject* modelObject{};

protected:
	bool setModelObject(const std::string &modelName, const std::string &newModelObjectName, ErrorReport &error);
	
	bool setModelObject(rel::engine::Model* model, const std::string &newModelObjectName, ErrorReport &error);

	void setModelObject(rel::engine::ModelObject* newModelObject) {
		InstancedSceneMeshObject::modelObject = newModelObject;
		invalidateAll();
	}

	void getVertices(std::vector<glm::vec3> &newVertices) override;

	void getIndices(std::vector<uint16_t> &newIndices) override;

	void getUVs(std::vector<glm::vec2> &newUVs) override;

	void getColors(std::vector<glm::vec4> &newColors) override;

	void getNormals(std::vector<glm::vec3> &newNormals) override;

	bool setBounds(rel::engine::Model* model, const std::string &newBoundsModelObjectName, ErrorReport &error);

	void setBounds(rel::engine::ModelObject* newModelObject) {
		InstancedSceneMesh::setBounds(ModelBoundingVolume::create(newModelObject));
	}


public:
};


#endif //MAIN_INSTANCEDSCENEMESHOBJECT_H
