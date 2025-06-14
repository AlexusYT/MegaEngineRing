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
// Created by alexus on 19.02.25.
//

#include "SketchfabAccount.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "EngineSDK/gltf/GltfModel.h"
#include "GLTFSDK/IStreamReader.h"
#include "Globals.h"
#include "SketchfabCache.h"
#include "SketchfabSearch.h"
#include "Utils.h"

namespace mer::editor::mvp {
SketchfabAccount::SketchfabAccount() {
	cache = std::make_shared<SketchfabCache>(Globals::getCachePath() / "SketchfabCache");
}

std::shared_ptr<SketchfabAccount> SketchfabAccount::createFromFile(const std::filesystem::path &pFilePath,
																   sdk::ReportMessagePtr &pMessage) {
	try {
		std::ifstream file(pFilePath);
		file.exceptions(std::_S_badbit | std::_S_failbit);
		if (!file.is_open()) {
			pMessage = sdk::ReportMessage::create();
			pMessage->setTitle("Failed to read account data");
			pMessage->setMessage("File opening failed");
			pMessage->addInfoLine("File path: {}", pFilePath.string());
			return nullptr;
		}
		auto account = create();
		nlohmann::json json;
		file >> json;
		account->deserialize(json);
		return account;
	}
	catch (...) {
		pMessage = sdk::ReportMessage::create();
		pMessage->setTitle("Failed to read account data");
		pMessage->setMessage("Exception occurred");
		pMessage->addInfoLine("File path: {}", pFilePath.string());
		return nullptr;
	}
}

std::shared_ptr<SketchfabAccount> SketchfabAccount::create() {
	return std::shared_ptr<SketchfabAccount>(new SketchfabAccount());
}

sdk::ReportMessagePtr SketchfabAccount::saveToFile(const std::filesystem::path &pFilePath) {
	try {
		std::ofstream file(pFilePath, std::ios::binary | std::ios::trunc);
		file.exceptions(std::_S_badbit | std::_S_failbit);
		if (!file.is_open()) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to save account data");
			msg->setMessage("File opening failed");
			msg->addInfoLine("File path: {}", pFilePath.string());
			return msg;
		}

		nlohmann::json json;
		serialize(json);
		file << json;
	}
	catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to save account data");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("File path: {}", pFilePath.string());
		return msg;
	}
	return nullptr;
}

std::shared_ptr<SketchfabSearch> SketchfabAccount::createSearch() { return std::make_shared<SketchfabSearch>(this); }

void SketchfabAccount::deserialize(const nlohmann::json &pRoot) {
	try { pRoot.at("expires_in").get_to(expiresIn); }
	catch (...) { expiresIn = -1; }
	try { pRoot.at("token_type").get_to(tokenType); }
	catch (...) {}
	try { pRoot.at("scope").get_to(scope); }
	catch (...) {}
	try { pRoot.at("refresh_token").get_to(refreshToken); }
	catch (...) {}

	try {
		pRoot.at("access_token").get_to(accessToken);
		loaded = true;
	}
	catch (...) {}
}

void SketchfabAccount::serialize(nlohmann::json &pRoot) {
	pRoot.emplace("expires_in", expiresIn);
	pRoot.emplace("token_type", tokenType);
	pRoot.emplace("scope", scope);
	pRoot.emplace("refresh_token", refreshToken);
	pRoot.emplace("access_token", accessToken);
}

sdk::ReportMessagePtr SketchfabAccount::getRequest(const std::string &pUrl, const std::string &pData,
												   std::atomic<float> &pProgress, nlohmann::json &pResult) {
	try {
		pProgress.store(-1.0f);
		sdk::Logger::out("Request: {}?{}", pUrl, pData);
		auto startTime = std::chrono::steady_clock::now();
		std::unique_ptr<CURL, void (*)(CURL*)>
			request(curl_easy_init(), curl_easy_cleanup);
		curl_easy_setopt(request.get(), CURLOPT_URL, (pUrl + "?" + pData).c_str());

		std::list<std::string> header = {"Content-Type: application/x-www-form-urlencoded"};
		header.emplace_back("Authorization: " + tokenType + " " + accessToken);
		auto curlList = Utils::getCurlList(header);
		curl_easy_setopt(request.get(), CURLOPT_HTTPHEADER, curlList.get());
		std::stringstream ss;
		curl_easy_setopt(request.get(), CURLOPT_WRITEFUNCTION, Utils::streamWriteCallback);
		curl_easy_setopt(request.get(), CURLOPT_WRITEDATA, static_cast<void*>(&ss));
		curl_easy_setopt(request.get(), CURLOPT_NOPROGRESS, false);
		curl_easy_setopt(request.get(), CURLOPT_XFERINFODATA, &pProgress);
		curl_easy_setopt(request.get(), CURLOPT_XFERINFOFUNCTION,
						 static_cast<curl_xferinfo_callback>(
							 [](void* clientp, curl_off_t pDltotal, curl_off_t pDlnow, curl_off_t /*ultotal*/,
								 curl_off_t /*ulnow*/) {
							 auto progress = static_cast<float>(pDlnow) / static_cast<float>(pDltotal);
							 static_cast<std::atomic<float>*>(clientp)->store(progress);
							 return 0;
							 }));
		curl_easy_perform(request.get());
		curlList.reset();
		request.reset();
		auto endTime = std::chrono::steady_clock::now();
		auto duration = endTime - startTime;

		sdk::Logger::out("Response in: {}ms", duration.count() / 1000000);
		ss >> pResult;
		return nullptr;
	}
	catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("GET request failed");
		msg->setMessage("Exception occurred during getting an image");
		msg->addInfoLine("Address: {}", pUrl);
		msg->addInfoLine("Data: {}", pData);
		return msg;
	}
}

sdk::ReportMessagePtr SketchfabAccount::downloadImage(
	const std::string &pUrl, std::vector<unsigned char> &pUncompressedJpegData, const CacheSettings &pCache) {
	std::string pathToCache;

	if (size_t index = pUrl.find("thumbnails/"); index != std::string::npos) { pathToCache = pUrl.substr(index + 11); }
	if (size_t index = pUrl.find("avatars/"); index != std::string::npos) { pathToCache = pUrl.substr(index + 8); }
	if ((pCache & CACHE_NO_LOAD) != CACHE_NO_LOAD) {
		if (auto msg = cache->loadCache(pathToCache, pUncompressedJpegData)) {
			if (msg->getMessage() != "No cache file") sdk::Logger::error(msg);
		} else
			return nullptr;
	}
	try {
		sdk::Logger::out("Downloading image: {}", pUrl);
		std::unique_ptr<CURL, void (*)(CURL*)>
			request(curl_easy_init(), curl_easy_cleanup);
		curl_easy_setopt(request.get(), CURLOPT_URL, pUrl.c_str());

		/*std::list<std::string> header = {"Content-Type: application/x-www-form-urlencoded"};
				request.setOpt(new curlpp::options::HttpHeader(header));*/
		curl_easy_setopt(request.get(), CURLOPT_WRITEFUNCTION, Utils::rawDataWriteCallback);
		curl_easy_setopt(request.get(), CURLOPT_WRITEDATA, &pUncompressedJpegData);

		curl_easy_perform(request.get());
	}
	catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Image downloading error");
		msg->setMessage("Exception occurred during getting an image");
		msg->addInfoLine("Address: {}", pUrl);
		return msg;
	}
	if ((pCache & CACHE_NO_SAVE) != CACHE_NO_SAVE) {
		if (auto msg = cache->saveCache(pathToCache, pUncompressedJpegData)) { sdk::Logger::error(msg); }
	}
	return nullptr;
}

sdk::ReportMessagePtr SketchfabAccount::downloadModel(const std::string &pUrl,
													  std::shared_ptr<std::iostream> &pStreamOut,
													  std::atomic<float> &pProgress, const CacheSettings &pCache) {
	pProgress.store(-1.0f);
	std::string pathToCache;

	if (size_t index = pUrl.find("archives/"); index != std::string::npos) {
		pathToCache = pUrl.substr(index + 9);
		pathToCache = pathToCache.substr(0, pathToCache.find('/'));
	}

	bool hasCache = false;
	if ((pCache & CACHE_NO_LOAD) != CACHE_NO_LOAD) {
		if (auto msg = cache->loadCache(pathToCache, pStreamOut)) {
			if (msg->getMessage() != "No cache file") sdk::Logger::error(msg);
		} else
			hasCache = true;
	}
	if (!hasCache) {
		try {
			sdk::Logger::out("Downloading model: {}", pUrl);
			std::unique_ptr<CURL, void (*)(CURL*)>
				request(curl_easy_init(), curl_easy_cleanup);
			curl_easy_setopt(request.get(), CURLOPT_URL, pUrl.c_str());

			/*std::list<std::string> header = {"Content-Type: application/x-www-form-urlencoded"};
				request.setOpt(new curlpp::options::HttpHeader(header));*/
			pStreamOut = std::make_shared<std::stringstream>();

			curl_easy_setopt(request.get(), CURLOPT_WRITEFUNCTION, Utils::streamWriteCallback);
			curl_easy_setopt(request.get(), CURLOPT_WRITEDATA, pStreamOut.get());
			curl_easy_setopt(request.get(), CURLOPT_NOPROGRESS, false);
			curl_easy_setopt(request.get(), CURLOPT_XFERINFODATA, &pProgress);
			curl_easy_setopt(request.get(), CURLOPT_XFERINFOFUNCTION,
							 static_cast<curl_xferinfo_callback>(
								 [](void* clientp, curl_off_t pDltotal, curl_off_t pDlnow, curl_off_t /*ultotal*/,
									 curl_off_t /*ulnow*/) {
								 auto progress = static_cast<float>(pDlnow) / static_cast<float>(pDltotal);
								 *static_cast<float*>(clientp) = progress;
								 return 0;
								 }));

			//TODO use curl_multi_perform
			curl_easy_perform(request.get());
		}
		catch (...) {
			pStreamOut.reset();
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Image downloading error");
			msg->setMessage("Exception occurred during getting an image");
			msg->addInfoLine("Address: {}", pUrl);
			return msg;
		}
		if ((pCache & CACHE_NO_SAVE) != CACHE_NO_SAVE) {
			if (const auto msg = cache->saveCache(pathToCache, *pStreamOut.get())) { sdk::Logger::error(msg); }
			pStreamOut->seekg(0);
		}
	}
	return nullptr;
}

bool SketchfabAccount::isFileCached(const std::string &pUrl) const { return cache->isCached(pUrl); }

sdk::ReportMessagePtr SketchfabAccount::loadCachedFile(const std::string &pUrl,
													   std::shared_ptr<std::iostream> &pData) const {
	return cache->loadCache(pUrl, pData);
}
} // namespace mer::editor::mvp
