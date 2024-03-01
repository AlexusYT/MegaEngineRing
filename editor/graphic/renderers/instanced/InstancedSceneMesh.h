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

#ifndef MAIN_INSTANCEDSCENEMESH_H
#define MAIN_INSTANCEDSCENEMESH_H
#if 0

	#include <Render/opengl/buffers/SSBOFwd.h>

	#include "InstancedSceneMeshFwd.h"
	#include "MeshInstance.h"
	#include "MeshInstanceData.h"
	#include "graphic/renderers/MeshMaterial.h"
	#include "graphic/renderers/SceneMeshBase.h"
	#include "graphic/viewport/OpenGLFwd.h"

class InstancedSceneMesh : public SceneMeshBase {
	friend MeshInstance;

private:
	std::list<MeshInstanceUPtr> instances;
	std::vector<MeshInstanceData> instancesData;
	glm::vec3 meshSize{};
	glm::vec3 meshOrigin{};
	glm::vec3 meshMin{};
	glm::vec3 meshMax{};
	std::mutex mutex;
	OpenGL* render;

public:
	template<typename T, typename... Args>
	static InstancedSceneMeshUPtr create(Args... args) {
		return InstancedSceneMeshUPtr(new (std::nothrow) T(args...));
	}

	InstancedSceneMesh();

	~InstancedSceneMesh() override;

	bool init(/*ErrorReport &error*/);

	[[maybe_unused]] void update(double deltaTime, bool renderShadow);

	MeshInstance* addInstance(MeshInstanceUPtr meshInstance);

	template<typename T, typename... Args>
	T* addInstance(Args... args) {
		return dynamic_cast<T*>(addInstance(MeshInstance::create<T>(args...)));
	}

	void addInstances(std::vector<MeshInstanceUPtr> meshInstances);

	virtual void onUpdateStarted();

	[[nodiscard]] const std::list<MeshInstanceUPtr> &getInstances() const { return instances; }

	/*template<typename ... Args>
	void addInstances(Args... args){
		std::vector<MeshInstanceUPtr> vec(std::move(args...));
	}*/

	[[nodiscard]] OpenGL* getParentWorld() const { return render; }

	void setParentWorld(OpenGL* newParentWorld);

	[[nodiscard]] const glm::vec3 &getMeshMin() const { return meshMin; }

	[[nodiscard]] const glm::vec3 &getMeshMax() const { return meshMax; }

	[[nodiscard]] const glm::vec3 &getMeshSize() const { return meshSize; }

	[[nodiscard]] const glm::vec3 &getMeshOrigin() const { return meshOrigin; }

	void updateInstancesData();

	virtual std::string getName() = 0;

protected:
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-parameter"

	// NOLINTNEXTLINE(*-unused-parameters)
	virtual bool onInit(/*ErrorReport &error*/) { return true; }

	virtual void onSizeChanged();

	#pragma GCC diagnostic pop

private:
	void onVerticesInvalidated() override {

		glm::vec3 min{std::numeric_limits<float>::max()};
		glm::vec3 max{std::numeric_limits<float>::min()};
		for (const auto &vertex: RenderVertexBuffer::getBufferData()) {
			min.x = std::min(min.x, vertex.x);
			min.y = std::min(min.y, vertex.y);
			min.z = std::min(min.z, vertex.z);

			max.x = std::max(max.x, vertex.x);
			max.y = std::max(max.y, vertex.y);
			max.z = std::max(max.z, vertex.z);
		}
		if (meshMin != min || meshMax != max) {
			meshMin = min;
			meshMax = max;
			sizeChanged();
		}
	}

	void sizeChanged() {
		meshSize = meshMax - meshMin;
		meshOrigin = (meshMax + meshMin) / 2.0f;
		onSizeChanged();
	}
};
#endif


#endif //MAIN_INSTANCEDSCENEMESH_H
