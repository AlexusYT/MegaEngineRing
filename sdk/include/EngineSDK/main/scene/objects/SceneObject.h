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
class Extension;
class IScene;

class SceneObject : public ISceneObject {
	IScene* scene{};
	std::map<std::string, std::shared_ptr<Extension>> extensions;
	bool inited{};

	glm::vec3 position{};
	sigc::signal<void()> onPositionChangedSignal;

public:
	virtual ~SceneObject() = default;
	/**
	 * \brief 
	 * \param[in] pName
	 * \param[in] pExtension
	 * \return 
	 */
	utils::ReportMessagePtr addExtension(const std::string &pName, const std::shared_ptr<Extension> &pExtension);

	utils::ReportMessagePtr removeExtension(const std::string &pName, std::shared_ptr<Extension> &pExtension);

	utils::ReportMessagePtr transferExtensionTo(const std::string &pName, SceneObject* pTransferTo);

	[[nodiscard]] IScene* getScene() const { return scene; }

	[[nodiscard]] const glm::vec3 &getPosition() const { return position; }

	void setPosition(const glm::vec3 &pPosition) {
		if (pPosition == position) return;
		position = pPosition;
		onPositionChangedSignal();
	}

	[[nodiscard]] sigc::signal<void()> &getOnPositionChangedSignal() { return onPositionChangedSignal; }


private:
	utils::ReportMessagePtr init() override;

	void render() const override;

	void onWindowSizeChanged(int pWidth, int pHeight) const override;

	void onCursorPosChanged(double pX, double pY) const override;

	void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) const override;

	void setScene(IScene* const pScene) override { scene = pScene; }
};
} // namespace mer::sdk::main


#endif //SCENEOBJECT_H
