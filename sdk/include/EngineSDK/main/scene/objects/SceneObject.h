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
// Created by alexus on 27.02.24.
//

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include <glm/vec3.hpp>
#include <map>
#include <memory>
#include <sigc++/signal.h>

#include "EngineUtils/utils/ReportMessageFwd.h"
#include "ISceneObject.h"
#include "extensions/Extension.h"

namespace mer::sdk::main {
class IScript;
}

namespace mer::sdk::main {
class Extension;
class IScene;

class SceneObject : public ISceneObject {
	std::shared_ptr<UUID> uuid;
	std::string name;
	sigc::signal<void(const std::string &pOldName, const std::string &pNewName)> onNameChangedSignal;
	IScene* scene{};
	std::map<std::string, std::shared_ptr<Extension>> extensions;
	sigc::signal<void(const std::shared_ptr<Extension> &pNewExt)> onExtensionAddedSignal;
	sigc::signal<void(const std::shared_ptr<Extension> &pExtToRemove)> onExtensionRemovedSignal;
	bool inited{};
	std::shared_ptr<IScript> script;
	std::string scriptName;

	glm::vec3 position{};
	sigc::signal<void()> onPositionChangedSignal;
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
	utils::ReportMessagePtr addExtension(const std::string &pName,
										 const std::shared_ptr<Extension> &pExtension) override;

	utils::ReportMessagePtr removeExtension(const std::string &pName, std::shared_ptr<Extension> &pExtension) override;

	utils::ReportMessagePtr transferExtensionTo(const std::string &pName, ISceneObject* pTransferTo) override;

	sigc::connection connectOnExtensionAdded(
		const sigc::slot<void(const std::shared_ptr<Extension> &pNewExt)> &pSlot) override {
		return onExtensionAddedSignal.connect(pSlot);
	}

	sigc::connection connectOnExtensionRemoved(
		const sigc::slot<void(const std::shared_ptr<Extension> &pExtToRemove)> &pSlot) override {
		return onExtensionRemovedSignal.connect(pSlot);
	}

	[[nodiscard]] const std::map<std::string, std::shared_ptr<Extension>> &getExtensions() const override {
		return extensions;
	}

	[[nodiscard]] IScene* getScene() const { return scene; }

	[[nodiscard]] const glm::vec3 &getPosition() const override { return position; }

	void setPosition(const glm::vec3 &pPosition) override {
		if (pPosition == position) return;
		position = pPosition;
		onPositionChangedSignal();
	}

	[[nodiscard]] sigc::signal<void()> &getOnPositionChangedSignal() { return onPositionChangedSignal; }

	[[nodiscard]] const std::string &getName() const override { return name; }

	void setName(const std::string &pName) override {
		if (name == pName) return;
		const auto oldName = name;
		name = pName;
		onNameChangedSignal(oldName, name);
	}

	sigc::connection connectOnNameChanged(
		const sigc::slot<void(const std::string &pOldName, const std::string &pNewName)> &pSlot) override {
		pSlot("", name);
		return onNameChangedSignal.connect(pSlot);
	}

	[[nodiscard]] const std::string &getScriptName() const override { return scriptName; }

	void setScriptName(const std::string &pScriptName) override { scriptName = pScriptName; }

	static void resetCounter() { counter = 0; }

private:
	utils::ReportMessagePtr init() override;

	void deinit() override;

	void render() const override;

	void onWindowSizeChanged(int pWidth, int pHeight) const override;

	void onCursorPosChanged(double pX, double pY) const override;

	void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) const override;

	void onMouseButtonStateChanged(utils::MouseButton pButton, bool pPressed, double pX, double pY) const override;

	void setScene(IScene* const pScene) override { scene = pScene; }

	void setUuid(const std::shared_ptr<UUID> &pUuid) override { uuid = pUuid; }

	UUID* getUuid() override { return uuid.get(); }
};
} // namespace mer::sdk::main


#endif //SCENEOBJECT_H
