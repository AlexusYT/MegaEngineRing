// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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
