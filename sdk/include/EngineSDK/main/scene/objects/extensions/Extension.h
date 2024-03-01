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

#ifndef EXTENSION_H
#define EXTENSION_H
#include <sigc++/signal.h>

#include "EngineSDK/main/resources/Resources.h"
#include "EngineSDK/main/scene/IScene.h"
#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::sdk::utils {
class ModifierKeys;
enum class KeyboardKey;
} // namespace mer::sdk::utils

namespace mer::sdk::main {
class SceneObject;

class Extension {
	friend SceneObject;
	SceneObject* object{};
	std::vector<sigc::connection> connectionStorage;

public:
	using ValueChanged = sigc::signal<void()>;
	template<typename... Args>
	using ValueChangedArgs = sigc::signal<void(Args...)>;
	template<typename... Args>
	using Method = sigc::slot<void(Args...)>;

	virtual ~Extension() = default;

	[[nodiscard]] SceneObject* getObject() const { return object; }

protected:
	template<RequestRegularConcept ClassT>
	void enqueueResourceLoading(const std::shared_ptr<ClassT> &pRequest,
								const sigc::slot<void(const std::shared_ptr<typename ClassT::ResourceT> &pResource,
													  const utils::ReportMessagePtr &pError)> &pSlot) {
		getScene()->enqueueResourceLoading(
			pRequest, [pSlot](const std::shared_ptr<IResource> &pResource, const utils::ReportMessagePtr &pError) {
				pSlot(std::dynamic_pointer_cast<typename ClassT::ResourceT>(pResource), pError);
			});
	}

	IScene* getScene() const;

	void putConnectionToStorage(const sigc::connection &pConnection) { connectionStorage.emplace_back(pConnection); }

	void freeConnectionStorage() {
		for (auto &connection: connectionStorage) {
			if (connection.connected()) connection.disconnect();
		}
	}

	virtual utils::ReportMessagePtr onInit() { return nullptr; }

	virtual utils::ReportMessagePtr onDeinit() {
		freeConnectionStorage();
		return nullptr;
	}

	virtual void onRender() {}

	virtual void onWindowSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods);

private:
	void setObject(SceneObject* const pObject) { object = pObject; }
};
} // namespace mer::sdk::main


#endif //EXTENSION_H
