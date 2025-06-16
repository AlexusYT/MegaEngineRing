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
// Created by alexus on 15.03.24.
//

#include "KwasarEngine/extensions/ExtensionRegistry.h"

#include "KwasarEngine/extensions/BasicRenderExtension.h"
#include "KwasarEngine/extensions/LightExtension.h"
#include "KwasarEngine/extensions/MainObjectExtension.h"
#include "KwasarEngine/extensions/MeshExtension.h"
#include "KwasarEngine/extensions/MouseButtonExtension.h"
#include "KwasarEngine/extensions/cameras/CameraExtension.h"
#include "KwasarEngine/extensions/cameras/CameraKeyboardExtension.h"
#include "KwasarEngine/extensions/cameras/CameraMouseExtension.h"
#include "KwasarEngine/extensions/cameras/OrbitCameraExtension.h"
#include "KwasarEngine/utils/ReportMessage.h"
#include "KwasarEngine/utils/Utils.h"

namespace mer::sdk {
void ExtensionRegistry::init() {
	/*registerClass<MainObjectExtension>();
	registerClass<BasicRenderExtension>();
	registerClass<CameraExtension>();
	registerClass<CameraKeyboardExtension>();
	registerClass<CameraMouseExtension>();
	registerClass<OrbitCameraExtension>();
	registerClass<MouseButtonExtension>();*/
	registerClass<MeshExtension>();
	registerClass<LightExtension>();
}

void ExtensionRegistry::deinit() { extensions.clear(); }

std::shared_ptr<Extension> ExtensionRegistry::newInstance(const std::string &pTypeName) {
	if (const auto iter = extensions.find(pTypeName); iter != extensions.end()) {
		const auto &createFunc = iter->second.createFunc;
		auto ext = createFunc();
		return ext;
	}
	return nullptr;
}

ReportMessagePtr ExtensionRegistry::getTypeNameFor(Extension* pExt, std::string &pNameOut) {
	auto &type = typeid(*pExt);
	if (const auto iter = extensionByType.find(std::type_index(type)); iter != extensionByType.end()) {
		pNameOut = iter->second.typeName;
		return nullptr;
	}
	pNameOut = Utils::demangle(type.name());
	auto msg = ReportMessage::create();
	msg->setTitle("Failed to get name for type");
	msg->setMessage("No extension registered with such type");
	msg->addInfoLine("Extension type: {}", pNameOut);
	return msg;
}
} // namespace mer::sdk
