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
// Created by alexus on 02.10.24.
//

#include "KwasarEngine/resources/FileSystemResourceBundle.h"

#include <fstream>

#include "KwasarEngine/utils/ReportMessage.h"

namespace mer::sdk {
std::shared_ptr<IResourceBundle> FileSystemResourceBundle::create(const std::filesystem::path &pSearchPath) {
	return std::shared_ptr<FileSystemResourceBundle>(new FileSystemResourceBundle(pSearchPath));
}

ReportMessagePtr FileSystemResourceBundle::getResourceStream(const std::string &pResourceUri,
															 std::shared_ptr<std::istream> &pStream) {
	auto uri = std::filesystem::path(pResourceUri);
	std::filesystem::path resourcePath = !uri.has_root_directory() ? searchPath / pResourceUri : uri;

	auto resourceName = resourcePath.filename();
	std::vector<std::filesystem::path> candidates;
	for (const auto &entry: std::filesystem::directory_iterator(resourcePath.parent_path())) {
		auto path = entry.path();
		if (path.filename() == resourceName) { candidates.push_back(path); }
	}
	if (candidates.empty()) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to get resource stream");
		msg->setMessage("Resource not found");
		msg->addInfoLine("Search path: {}", searchPath.string());
		msg->addInfoLine("Resource URI to search: {}", pResourceUri);
		return msg;
	}
	if (candidates.size() > 1) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to get resource stream");
		msg->setMessage("Found more than one candidate to load");
		msg->addInfoLine("Search path: {}", searchPath.string());
		msg->addInfoLine("Resource URI to search: {}", pResourceUri);
		msg->addInfoLine("Candidates:");
		for (auto candidate: candidates) { msg->addInfoLine(candidate.string()); }
		return msg;
	}
	auto resource = candidates.at(0);
	std::shared_ptr<std::ifstream> stream{};
	try {
		stream = std::make_shared<std::ifstream>(resource);
		stream->exceptions(std::_S_badbit | std::_S_failbit);
		pStream = stream;
	}
	catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to get resource stream");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("Search path: {}", searchPath.string());
		msg->addInfoLine("Resource URI to search: {}", pResourceUri);
		return msg;
	}

	return nullptr;
}

void FileSystemResourceBundle::listResources(std::vector<std::string> &pUris) const {
	for (const auto &entry: std::filesystem::recursive_directory_iterator(searchPath)) {
		auto path = entry.path();
		auto extension = path.extension();
		if (extension == ".enmodel" || extension == ".enmat" || extension == ".entex") {
			pUris.emplace_back(relative(path, searchPath).string());
		}
	}
}
} // namespace mer::sdk
