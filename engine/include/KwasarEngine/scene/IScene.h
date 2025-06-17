//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 31.01.24.
//

#ifndef ISCENE_H
#define ISCENE_H
#include <glm/fwd.hpp>
#include <sigc++/signal.h>

#include "KwasarEngine/utils/ReportMessageFwd.h"

namespace ke {
enum class MouseButton;
class ModifierKeys;
enum class KeyboardKey;
} // namespace ke

namespace ke {
class ResourceLoadResult;
class IApplication;
class ICamera;
class ISceneObject;
class ResourceRequest;
class IResource;
class IResourceLoadExecutor;
class ProgramWideShaderBuffer;

class IScene {
public:
	virtual ~IScene() = default;

	virtual void setViewProjMatrix(const glm::mat4 &pViewProjMatrix) const = 0;

	virtual ke::ReportMessagePtr initScene() = 0;

	virtual void deinitScene() = 0;

	[[nodiscard]] virtual IResourceLoadExecutor* getResourceExecutor() const = 0;

	virtual void setResourceExecutor(IResourceLoadExecutor* pResources) = 0;

	virtual std::shared_ptr<IResource> loadResourceSync(const std::string &pName) = 0;

	virtual void loadResourceAsync(
		const std::string &pResourceUri,
		const sigc::slot<void(const std::shared_ptr<ResourceLoadResult> &pResult)> &pSlot) const = 0;

	virtual void onResourceLoadingError(const std::string &pResourceUri, const ReportMessagePtr &pError) = 0;

	virtual void addObject(const std::shared_ptr<ISceneObject> &pObject) = 0;

	virtual void removeObject(ISceneObject* pObjectToRemove) = 0;

	[[nodiscard]] virtual const std::shared_ptr<ProgramWideShaderBuffer> &getProgramBuffer() const = 0;

	[[nodiscard]] virtual const std::vector<std::shared_ptr<ISceneObject>> &getObjects() const = 0;

	[[nodiscard]] virtual sigc::signal<void(ISceneObject* pObject)> &getOnObjectAddedSignal() = 0;

	[[nodiscard]] virtual sigc::signal<void(ISceneObject* pObject)> &getOnObjectRemovedSignal() = 0;

	[[nodiscard]] virtual ICamera* getCurrentCamera() const = 0;

	virtual void switchCamera(ICamera* pNewCamera) = 0;

	[[nodiscard]] virtual IApplication* getApplication() const = 0;

	virtual void setApplication(IApplication* pApplication) = 0;

	virtual void render() = 0;

	virtual void resize(int pWidth, int pHeight) = 0;

	virtual void onCursorPosChanged(double pX, double pY) = 0;

	virtual void onKeyChanged(KeyboardKey pKey, bool pPressed, const ModifierKeys &pMods) = 0;

	virtual void onMouseButtonStateChanged(MouseButton pButton, bool pPressed, double pX, double pY) = 0;

	virtual bool notifyOnMouseScroll(double pDx, double pDy) = 0;

protected:
	virtual bool onMouseScroll(double pDx, double pDy);
};

inline bool IScene::onMouseScroll(double /*pDx*/, double /*pDy*/) { return false; }
} // namespace ke


#endif //ISCENE_H
