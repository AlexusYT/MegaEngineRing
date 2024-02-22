//
// Created by alexus on 29.07.23.
//
#if 0

	#include "MeshInstance.h"

	#include "InstancedSceneMesh.h"

void MeshInstance::addChild(MeshInstance* child) {
	child->setParentMesh(this);
	childrenInterior.emplace_back(child);
	children.emplace_back(child);
}

void MeshInstance::updateData() {
	if (mesh) mesh->updateInstancesData();
}

#endif
