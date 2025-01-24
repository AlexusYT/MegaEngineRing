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
// Created by alexus on 10.11.24.
//

#include "EditorFileSystemResourceBundle.h"

namespace mer::editor::project {
std::shared_ptr<sdk::IResourceBundle> EditorFileSystemResourceBundle::create(
	const std::filesystem::path &pSearchPath) {
	return std::shared_ptr<EditorFileSystemResourceBundle>(new EditorFileSystemResourceBundle(pSearchPath));
}

sdk::ReportMessagePtr EditorFileSystemResourceBundle::getResourceStream(const std::string &pResourceUri,
																			   std::shared_ptr<std::istream> &pStream) {

	if (pResourceUri.starts_with("_builtin_")) {
		if (!Gio::Resource::get_file_exists_global_nothrow("/" + pResourceUri)) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to get resource stream");
			msg->setMessage("Builtin resource not found");
			msg->addInfoLine("Resource URI to search: {}", pResourceUri);
			return msg;
		}
		auto resourceBytes = Gio::Resource::lookup_data_global("/" + pResourceUri);
		auto ss = std::make_shared<std::stringstream>();
		unsigned long size = 0;
		auto buf = static_cast<const char*>(resourceBytes->get_data(size));
		ss->write(buf, static_cast<std::streamsize>(size));
		pStream = ss;
		return nullptr;
	}
	return FileSystemResourceBundle::getResourceStream(pResourceUri, pStream);
}

void EditorFileSystemResourceBundle::listResources(std::vector<std::string> &pUris) const {
	FileSystemResourceBundle::listResources(pUris);
	try {
		listBuiltinResources("/_builtin_/", pUris);
	} catch (Gio::ResourceError &e) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to list resource uris");
		sdk::Logger::error(msg);
	}
}

void EditorFileSystemResourceBundle::listBuiltinResources(const std::string &pCurrentUri,
														  std::vector<std::string> &pUris) {
	auto builtIns = Gio::Resource::enumerate_children_global(pCurrentUri);
	for (auto builtIn: builtIns) {
		auto newUri = pCurrentUri + builtIn;
		if (builtIn.find('.') != std::string::npos) {
			pUris.emplace_back(newUri.substr(1));
		} else
			listBuiltinResources(newUri, pUris);
	}
}

} // namespace mer::editor::project