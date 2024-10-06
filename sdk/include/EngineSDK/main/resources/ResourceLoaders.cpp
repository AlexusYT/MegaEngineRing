//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 02.10.24.
//

#include "ResourceLoaders.h"

#include <filesystem>

#include "EngineUtils/utils/ReportMessage.h"
#include "IResourceBundle.h"
#include "ResourceLoader.h"
#include "models/Model3DLoader.h"
#include "shaders/BuiltInProgramRequest.h"

namespace mer::sdk::main {
utils::ReportMessagePtr ResourceLoaders::load(
	IResourceLoadExecutor* pLoadExecutor, const std::shared_ptr<IResourceBundle> &pBundle,
	const std::string &pResourceUri, std::shared_ptr<IResource> &pResourceOut) {
	auto uri = std::filesystem::path(pResourceUri);
	if (!uri.has_extension()) {
		auto msg = utils::ReportMessage::create();
		msg->setTitle("Unable to load resource");
		msg->setMessage("No resource extension in uri");
		msg->addInfoLine("Resource URI: {}", pResourceUri);
		return msg;
	}
	auto extension = uri.extension().string().substr(1);
	auto iter = loaders.find(extension);
	if (iter == loaders.end()) {
		auto msg = utils::ReportMessage::create();
		msg->setTitle("Unable to load resource");
		msg->setMessage("No loader registered that can load such resource");
		msg->addInfoLine("Resource URI: {}", pResourceUri);
		return msg;
	}

	std::shared_ptr<std::istream> stream;
	if (auto msg = pBundle->getResourceStream(pResourceUri, stream)) {
		msg->setTitle("Unable to load resource");
		return msg;
	}
	std::shared_ptr<IResource> resource;
	if (auto msg = iter->second->load(pLoadExecutor, stream, resource)) {
		msg->setTitle("Unable to load resource");
		return msg;
	}
	pResourceOut = resource;
	return nullptr;
}

void ResourceLoaders::addLoader(const std::shared_ptr<IResourceLoader> &pLoader) {
	loaders.emplace(pLoader->getFileExtension(), pLoader);
}

void ResourceLoaders::initLoaders() {
	addLoader(std::make_shared<Model3DLoader>());
	addLoader(std::make_shared<BuiltInProgramRequest::BuiltInProgramLoader>());
}

} // namespace mer::sdk::main