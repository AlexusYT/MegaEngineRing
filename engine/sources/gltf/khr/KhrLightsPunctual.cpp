//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 17.04.25.
//

#include "KwasarEngine/gltf/khr/KhrLightsPunctual.h"

#include <nlohmann/json.hpp>

#include "KwasarEngine/gltf/Light.h"
#include "GLTFSDK/Document.h"
#include "GLTFSDK/ExtensionHandlers.h"

namespace ke {
void KhrLightsPunctual::addHandlers(Microsoft::glTF::ExtensionDeserializer &pDeserializer) {
	pDeserializer.AddHandler<KhrLightsPunctual, Microsoft::glTF::Document>(EXTENSION_NAME, deserializeDocument);
	pDeserializer.AddHandler<KhrLightsPunctual, Microsoft::glTF::Node>(EXTENSION_NAME, deserializeNode);
}

std::unique_ptr<Microsoft::glTF::Extension> KhrLightsPunctual::deserializeDocument(
	const nlohmann::json &pJson, const std::shared_ptr<Microsoft::glTF::ExtensionDeserializer> &) {
	auto ext = std::make_unique<KhrLightsPunctual>();
	try { pJson.at("lights").get_to(ext->lights); }
	catch (...) { ext->lights.clear(); }
	return ext;
}

std::unique_ptr<Microsoft::glTF::Extension> KhrLightsPunctual::deserializeNode(
	const nlohmann::json &pJson, const std::shared_ptr<Microsoft::glTF::ExtensionDeserializer> &) {
	auto ext = std::make_unique<Node>();
	try { pJson.at("light").get_to(ext->light); }
	catch (...) { ext->light = -1; }
	return ext;
}
} // namespace ke
