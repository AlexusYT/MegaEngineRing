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

#include "SceneObject.h"

#include <dlfcn.h>

#include "EngineSDK/main/scripting/IScript.h"
#include "EngineUtils/utils/ReportMessage.h"
#include "EngineUtils/utils/UUID.h"

namespace mer::sdk::main {

SceneObject::SceneObject() {
	uuid = UUID::create();
	//TODO make this smarter (avoid dups and missing names)

	name = std::format("SceneObject{}", counter);
	counter++;
}

utils::ReportMessagePtr SceneObject::addExtension(const std::string &pName,
												  const std::shared_ptr<Extension> &pExtension) {
	if (!pExtension) {
		auto msg = utils::ReportMessage::create();
		msg->setTitle("Unable to add an extension");
		msg->setMessage("Extension cannot be nullptr");
		msg->addInfoLine("Extension name: {}", pName);
		msg->addInfoLine("Note that one ext instance can be added to one SceneObject instance");
		return msg;
	}
	if (const auto iter = extensions.find(pName); iter != extensions.end()) {
		auto msg = utils::ReportMessage::create();
		msg->setTitle("Unable to add an extension");
		msg->setMessage("Extension with that name already added");
		msg->addInfoLine("Extension name: {}", pName);
		msg->addInfoLine("Already stored ext typename: {}", Utils::getTypeName(iter->second.get()));
		msg->addInfoLine("New ext typename: {}", Utils::getTypeName(pExtension.get()));
		return msg;
	} else {
		pExtension->setObject(this);
		if (inited) {
			try {
				if (auto msg = pExtension->onInit()) {
					pExtension->setObject(nullptr);
					msg->addInfoLine("Extension name: {}", pName);
					msg->addInfoLine("Extension typename: {}", Utils::getTypeName(pExtension.get()));
					return msg;
				}
			} catch (...) {
				pExtension->setObject(nullptr);
				auto msg = utils::ReportMessage::create();
				msg->setTitle("Unable to add an extension");
				msg->setMessage("Exception occurred in onInit()");
				msg->addInfoLine("Extension name: {}", pName);
				msg->addInfoLine("Extension typename: {}", Utils::getTypeName(pExtension.get()));
				return msg;
			}
		}
		pExtension->setName(pName);
		extensions.emplace_hint(iter, pName, std::move(pExtension));
	}
	return nullptr;
}

utils::ReportMessagePtr SceneObject::removeExtension(const std::string &pName, std::shared_ptr<Extension> &pExtension) {
	if (const auto iter = extensions.find(pName); iter == extensions.end()) {
		auto msg = utils::ReportMessage::create();
		msg->setTitle("Unable to remove an extension");
		msg->setMessage("Extension with that name doesn't exists");
		msg->addInfoLine("Extension name: {}", pName);
		return msg;
	} else {
		std::shared_ptr<Extension> ext = std::move(iter->second);
		extensions.erase(iter);
		try {
			if (auto msg = ext->onDeinit()) {
				ext->setObject(nullptr);
				msg->addInfoLine("Extension name: {}", pName);
				msg->addInfoLine("Extension typename: {}", Utils::getTypeName(ext.get()));
				return msg;
			}
		} catch (...) {
			ext->setObject(nullptr);
			auto msg = utils::ReportMessage::create();
			msg->setTitle("Unable to remove an extension. It will be destroyed");
			msg->setMessage("Exception occurred in onDeinit()");
			msg->addInfoLine("Extension name: {}", pName);
			msg->addInfoLine("Extension typename: {}", Utils::getTypeName(ext.get()));
			return msg;
		}
		ext->setObject(nullptr);
		pExtension = std::move(ext);
	}
	return nullptr;
}

utils::ReportMessagePtr SceneObject::transferExtensionTo(const std::string &pName, ISceneObject* pTransferTo) {
	std::shared_ptr<Extension> tmp;
	if (auto msg = this->removeExtension(pName, tmp)) { return msg; }
	if (auto msg = pTransferTo->addExtension(pName, tmp)) { return msg; }
	return nullptr;
}

utils::ReportMessagePtr SceneObject::init() {

	for (const auto &extension: extensions) {
		if (auto msg = extension.second->onInit()) return msg;
	}

	if (!scriptName.empty()) {

		auto main = dlopen(nullptr, RTLD_LAZY);

		auto createFunc =
			reinterpret_cast<std::shared_ptr<IScript> (*)()>(dlsym(main, ("create" + scriptName).c_str()));
		script = createFunc();
		script->setObject(this);
		script->setup();
	}
	inited = true;
	return nullptr;
}

void SceneObject::render() const {
	for (const auto &extension: extensions) { extension.second->onRender(); }
}

void SceneObject::onWindowSizeChanged(const int pWidth, const int pHeight) const {
	for (const auto &extension: extensions) {
		if (!extension.second->isEnabled()) continue;
		extension.second->onWindowSizeChanged(pWidth, pHeight);
	}
}

void SceneObject::onCursorPosChanged(const double pX, const double pY) const {
	for (const auto &extension: extensions) {
		if (!extension.second->isEnabled()) continue;
		extension.second->onCursorPosChanged(pX, pY);
	}
}

void SceneObject::onKeyStateChanged(const utils::KeyboardKey pKey, const bool pPressed,
									const utils::ModifierKeys &pMods) const {
	for (const auto &extension: extensions) {
		if (!extension.second->isEnabled()) continue;
		extension.second->onKeyStateChanged(pKey, pPressed, pMods);
	}
}

void SceneObject::onMouseButtonStateChanged(const utils::MouseButton pButton, const bool pPressed, const double pX,
											const double pY) const {
	for (const auto &extension: extensions) {
		if (!extension.second->isEnabled()) continue;
		extension.second->onMouseButtonStateChanged(pButton, pPressed, pX, pY);
	}
}
} // namespace mer::sdk::main