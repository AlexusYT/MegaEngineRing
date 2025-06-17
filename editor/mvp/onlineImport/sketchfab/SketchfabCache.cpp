//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 21.02.25.
//

#include "SketchfabCache.h"

namespace ked {
SketchfabCache::SketchfabCache(const std::filesystem::path &pPath) : pathToCache(pPath) {}

ke::ReportMessagePtr SketchfabCache::saveCache(const std::string &pUrl, const std::vector<unsigned char> &pData) {
	std::stringstream ss;
	ss.write(reinterpret_cast<const std::ostream::char_type*>(pData.data()),
			 static_cast<std::streamsize>(pData.size()));
	if (auto msg = saveCache(pUrl, ss)) { return msg; }
	return nullptr;
}

ke::ReportMessagePtr SketchfabCache::saveCache(const std::string &pUrl, const std::ostream &pData) {
	std::lock_guard lock(mutex);

	std::filesystem::path path;
	if (auto msg = buildPath(pUrl, path)) { return msg; }
	std::error_code ec;
	create_directories(path.parent_path(), ec);
	if (ec) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to save cache");
		msg->setMessage("Unable to create directory");
		msg->addInfoLine("Error: {}", ec.message());
		msg->addInfoLine("Url: {}", pUrl);
		msg->addInfoLine("Path to file: {}", path.string());
		return msg;
	}

	try {
		std::ofstream outFile(path);
		outFile.exceptions(std::_S_badbit | std::_S_failbit);
		outFile << pData.rdbuf();
		outFile.close();

		return nullptr;
	}
	catch (...) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to save cache");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("Url: {}", pUrl);
		return msg;
	}
}

ke::ReportMessagePtr SketchfabCache::loadCache(const std::string &pUrl, std::vector<unsigned char> &pData) {
	std::shared_ptr<std::iostream> stream;
	if (auto msg = loadCache(pUrl, stream)) {
		//
		return msg;
	}

	auto len = stream->rdbuf()->in_avail();
	pData.resize(static_cast<long unsigned int>(len));
	stream->read(reinterpret_cast<char*>(pData.data()), len);
	return nullptr;
}

ke::ReportMessagePtr SketchfabCache::loadCache(const std::string &pUrl, std::shared_ptr<std::iostream> &pData) {
	std::lock_guard lock(mutex);
	std::filesystem::path path;
	if (auto msg = buildPath(pUrl, path)) { return msg; }
	if (!exists(path)) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to load cache");
		msg->setMessage("No cache file");
		msg->addInfoLine("Path to file: {}", path.string());
		return msg;
	}
	try {
		pData = std::make_shared<std::fstream>(path, std::ios_base::binary | std::ios_base::in);
		pData->exceptions(std::_S_badbit | std::_S_failbit);
		return nullptr;
	}
	catch (...) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to load cache");
		msg->setMessage("Exception occurred while reading");
		msg->addInfoLine("Path to file: {}", path.string());
		return msg;
	}
}

bool SketchfabCache::isCached(const std::string &pUrl) const {
	std::filesystem::path path;
	if (buildPath(pUrl, path)) { return false; }
	return exists(path);
}

ke::ReportMessagePtr SketchfabCache::buildPath(const std::string &pUrl, std::filesystem::path &pPathOut) const {
	if (pUrl.size() < 2) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to load cache");
		msg->setMessage("Invalid url");
		msg->addInfoLine("Url: {}", pUrl);
		return msg;
	}
	pPathOut = pathToCache / pUrl.substr(0, 2) / pUrl;
	return nullptr;
}
} // namespace ked
