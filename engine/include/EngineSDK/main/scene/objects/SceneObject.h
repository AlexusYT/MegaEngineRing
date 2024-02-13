//
// Created by alexus on 12.02.24.
//

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "ISceneObject.h"

namespace n::sdk::renderer {
class ShaderProgram;
}

namespace n::sdk::main {

class SceneObject : public ISceneObject {
	std::shared_ptr<Resources> resources;
	std::shared_ptr<renderer::ShaderProgram> shader;

public:
	[[nodiscard]] virtual const std::shared_ptr<renderer::ShaderProgram> &getShader() const { return shader; }

	virtual void setShader(const std::shared_ptr<renderer::ShaderProgram> &pShader) { shader = pShader; }

protected:
	[[nodiscard]] const std::shared_ptr<Resources> &getResources() const final { return resources; }

	void setResources(const std::shared_ptr<Resources> &pResources) final { resources = pResources; }
};
} // namespace n::sdk::main


#endif //SCENEOBJECT_H
