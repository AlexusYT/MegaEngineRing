//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 27.02.24.
//

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include <map>
#include <memory>
#include <sigc++/signal.h>

#include "EngineUtils/utils/ReportMessageFwd.h"
#include "ISceneObject.h"

namespace mer::sdk {
class IScript;
}

namespace mer::sdk {
class Extension;
class IScene;

class SceneObject : public ISceneObject {
	std::shared_ptr<UUID> uuid;
	IScene* scene{};
	MainObjectExtension* mainExtension;
	std::map<std::string, std::shared_ptr<Extension>> extensions;
	sigc::signal<void(const std::shared_ptr<Extension> &pNewExt)> onExtensionAddedSignal;
	sigc::signal<void(const std::shared_ptr<Extension> &pExtToRemove)> onExtensionRemovedSignal;
	sigc::signal<void(Extension* pExtension, PropertyBase* pProperty)> onExtensionPropertyChangedSignal;
	bool inited{};
	std::shared_ptr<IScript> script;
	std::string scriptName;

	static inline uint64_t counter;

public:
	SceneObject();

	~SceneObject() override;

	static std::shared_ptr<ISceneObject> create();

	/**
	 * \brief 
	 * \param[in] pName
	 * \param[in] pExtension
	 * \return 
	 */
	ReportMessagePtr addExtension(const std::string &pName,
								  const std::shared_ptr<Extension> &pExtension) override;

	ReportMessagePtr removeExtension(const std::string &pName, std::shared_ptr<Extension> &pExtension) override;

	ReportMessagePtr transferExtensionTo(const std::string &pName, ISceneObject* pTransferTo) override;

	sigc::connection connectOnExtensionAdded(
		const sigc::slot<void(const std::shared_ptr<Extension> &pNewExt)> &pSlot) override {
		return onExtensionAddedSignal.connect(pSlot);
	}

	sigc::connection connectOnExtensionRemoved(
		const sigc::slot<void(const std::shared_ptr<Extension> &pExtToRemove)> &pSlot) override {
		return onExtensionRemovedSignal.connect(pSlot);
	}

	sigc::connection connectOnExtensionPropertyChanged(
		const sigc::slot<void(Extension* pExtension, PropertyBase* pProperty)> &pSlot) final {
		return onExtensionPropertyChangedSignal.connect(pSlot);
	}

	void notifyExtensionPropertyChanged(Extension* pExtension, PropertyBase* pProperty) final {
		onExtensionPropertyChangedSignal(pExtension, pProperty);
	}

	[[nodiscard]] MainObjectExtension* getMainExtension() const final { return mainExtension; }

	[[nodiscard]] const std::map<std::string, std::shared_ptr<Extension>> &getExtensions() const override {
		return extensions;
	}

	[[nodiscard]] IScene* getScene() const { return scene; }

	[[nodiscard]] const std::string &getScriptName() const override { return scriptName; }

	void setScriptName(const std::string &pScriptName) override { scriptName = pScriptName; }

	[[nodiscard]] const std::shared_ptr<IScript> &getScript() const override { return script; }

	void setScript(const std::shared_ptr<IScript> &pScript) override;

	static void resetCounter() { counter = 0; }

private:
	ReportMessagePtr init() override;

	void deinit() override;

	void render() const override;

	void onWindowSizeChanged(int pWidth, int pHeight) const override;

	void onCursorPosChanged(double pX, double pY) const override;

	void onKeyStateChanged(KeyboardKey pKey, bool pPressed, const ModifierKeys &pMods) const override;

	void onMouseButtonStateChanged(MouseButton pButton, bool pPressed, double pX, double pY) const override;

	void setScene(IScene* const pScene) override { scene = pScene; }

	void setUuid(const std::shared_ptr<UUID> &pUuid) override { uuid = pUuid; }

	UUID* getUuid() override { return uuid.get(); }

	bool notifyOnMouseScroll(double pDx, double pDy) final;
};
} // namespace mer::sdk


#endif //SCENEOBJECT_H
