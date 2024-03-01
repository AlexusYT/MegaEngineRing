//
// Created by alexus on 31.01.24.
//

#ifndef ISCENE_H
#define ISCENE_H
#include <sigc++/slot.h>

#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::sdk::utils {
class ModifierKeys;
enum class KeyboardKey;
} // namespace mer::sdk::utils

namespace mer::editor::mvp {
class PresenterSceneEditor;
} // namespace mer::editor::mvp

namespace mer::sdk::main {
class ResourceRequest;
class IResources;
class ProgramWideShaderBuffer;

class IScene {
	friend class MainWindow;
	friend class editor::mvp::PresenterSceneEditor;

public:
	virtual ~IScene() = default;

	virtual sdk::utils::ReportMessagePtr initScene() = 0;

	[[nodiscard]] virtual IResources* getResources() const = 0;


	virtual void onResourceLoadingError(const std::shared_ptr<ResourceRequest> &pRequest,
										const sdk::utils::ReportMessagePtr &pError) = 0;

	[[nodiscard]] virtual const std::shared_ptr<ProgramWideShaderBuffer> &getProgramBuffer() const = 0;
	virtual void enqueueResourceLoading(
		const std::shared_ptr<ResourceRequest> &pRequest,
		const sigc::slot<void(const std::shared_ptr<IResource> &pResource, const utils::ReportMessagePtr &pError)>
			&pSlot) const = 0;

private:
	virtual void setResources(IResources* pResources) = 0;

	virtual void render() = 0;

	virtual void resize(int pWidth, int pHeight) = 0;

	virtual void onCursorPosChanged(double pX, double pY) = 0;

	virtual void onKeyChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) = 0;
};
} // namespace mer::sdk::main


#endif //ISCENE_H
