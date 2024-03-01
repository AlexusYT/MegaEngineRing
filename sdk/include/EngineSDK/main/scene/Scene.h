//
// Created by alexus on 02.02.24.
//

#ifndef SCENE_H
#define SCENE_H
#include <EngineSDK/main/buffers/ProgramWideShaderBuffer.h>
#include <EngineSDK/main/resources/LazyResource.h>

#include <sigc++/signal.h>

#include "EngineSDK/main/resources/IResources.h"
#include "EngineSDK/main/resources/ResourceRequest.h"
#include "IScene.h"

namespace mer::sdk::main {
class ISceneObject;

class Scene : public IScene {
	IResources* resources;
	std::shared_ptr<ProgramWideShaderBuffer> programBuffer;
	std::vector<std::shared_ptr<ISceneObject>> objects;
	sigc::signal<void(int pWidth, int pHeight)> onWindowSizeChangedSignal;

public:
	Scene();

	void setViewProjMatrix(const glm::mat4 &pViewProjMatrix) const;

	[[nodiscard]] IResources* getResources() const final { return resources; }

	void onResourceLoadingError(const std::shared_ptr<ResourceRequest> &pRequest,
								const sdk::utils::ReportMessagePtr &pError) override;

	void enqueueResourceLoading(const std::shared_ptr<ResourceRequest> &pRequest,
								const IResources::ResourceSlot &pSlot) const override {
		resources->enqueueResourceLoading(pRequest, pSlot);
	}

	[[nodiscard]] const std::shared_ptr<ProgramWideShaderBuffer> &getProgramBuffer() const override {
		return programBuffer;
	}

protected:
	virtual void beforeRender();

	virtual void afterRender() {}

	sdk::utils::ReportMessagePtr initScene() override;

	void addObject(const std::shared_ptr<ISceneObject> &pObject);

private:
	void setResources(IResources* pResources) final;

	void render() final;

	void resize(int pWidth, int pHeight) override;

	void onCursorPosChanged(double pX, double pY) override;

	void onKeyChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) override;
};
} // namespace mer::sdk::main


#endif //SCENE_H