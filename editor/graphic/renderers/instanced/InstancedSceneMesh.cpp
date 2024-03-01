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
// Created by alexus on 29.07.23.
//

#include "InstancedSceneMesh.h"
#if 0

	#include <Render/opengl/buffers/SSBO.h>
	#include <Render/opengl/buffers/VertexArrayObject.h>

	#include <graphic/ProgramWideSettings.h>

	#include "graphic/viewport/OpenGL.h"

InstancedSceneMesh::InstancedSceneMesh() = default;

InstancedSceneMesh::~InstancedSceneMesh() = default;

bool InstancedSceneMesh::init(/*ErrorReport &error*/) {
	initBuffers();
	if (!onInit(/*error*/)) return false;
	invalidateAll();
	return true;
}

void InstancedSceneMesh::update(double deltaTime, bool /*renderShadow*/) {
	//if (renderShadow && renderShadow != isCastsShadow()) return;


	SceneMeshBase::updateBuffers();

	onUpdateStarted();
	for (const auto &instance: instances) {
		if (instance->isVisible()) instance->onUpdateStarted();
	}

	const auto settings = render->getProgramWideSettings()->getSSBO();
	const auto bufSize = static_cast<int64_t>(instancesData.size() * sizeof(MeshInstanceData));
	if (settings->getSize() < bufSize) {
		settings->reallocate(bufSize, instancesData.data());
	} else
		settings->bufferSubDataBind(0, bufSize, instancesData.data());

	getRenderBuffer()->drawElementsInstancedBind(RenderIndicesBuffer::getBufferData().size(), instancesData.size());
	for (const auto &instance: instances) {
		if (instance->isVisible()) instance->onUpdate(deltaTime);
	}
	for (const auto &instance: instances) {
		if (instance->isVisible()) instance->onUpdateStopped();
	}
}

MeshInstance* InstancedSceneMesh::addInstance(MeshInstanceUPtr meshInstance) {
	meshInstance->setMesh(this);
	meshInstance->setWorld(render);
	meshInstance->onInstanceAdded();
	//if (parentWorld) parentWorld->getScene()->onInstanceAdded(meshInstance.get());
	auto result = instances.emplace_back(std::move(meshInstance)).get();
	updateInstancesData();
	return result;
}

void InstancedSceneMesh::addInstances(std::vector<MeshInstanceUPtr> meshInstances) {
	for (auto &meshInstance: meshInstances) {
		meshInstance->setMesh(this);
		meshInstance->setWorld(render);
		meshInstance->onInstanceAdded();
		//if (parentWorld) parentWorld->getScene()->onInstanceAdded(meshInstance.get());
		instances.emplace_back(std::move(meshInstance));
	}
	updateInstancesData();
}

void InstancedSceneMesh::onUpdateStarted() { /*Scene::getFallbackTexture()->bind(0x84C1);*/
}

void InstancedSceneMesh::setParentWorld(OpenGL* newParentWorld) {
	if (InstancedSceneMesh::render == newParentWorld) return;
	InstancedSceneMesh::render = newParentWorld;
	for (const auto &instance: instances) { instance->setWorld(newParentWorld); }
}

void InstancedSceneMesh::updateInstancesData() {
	std::lock_guard lock(mutex);
	instancesData.clear();
	for (const auto &instance: instances) {
		if (!instance->isVisible()) continue;
		instancesData.emplace_back(instance->data);
	}
}

void InstancedSceneMesh::onSizeChanged() {}

#endif
