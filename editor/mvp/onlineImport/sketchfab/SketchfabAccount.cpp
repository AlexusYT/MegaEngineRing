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

#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>

#include "EngineSDK/gltf/GltfModel.h"
#include "GLTFSDK/IStreamReader.h"
#include "Globals.h"
#include "SketchfabCache.h"
#include "SketchfabSearch.h"

namespace mer::editor::mvp {


SketchfabAccount::SketchfabAccount() {
	cache = std::make_shared<SketchfabCache>(Globals::getCachePath() / "SketchfabCache");
	handle = std::make_unique<curlpp::internal::CurlHandle>();
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
	} catch (...) {
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
	} catch (...) {

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
	try {
		pRoot.at("expires_in").get_to(expiresIn);
	} catch (...) { expiresIn = -1; }
	try {
		pRoot.at("token_type").get_to(tokenType);
	} catch (...) {}
	try {
		pRoot.at("scope").get_to(scope);
	} catch (...) {}
	try {
		pRoot.at("refresh_token").get_to(refreshToken);
	} catch (...) {}

	try {
		pRoot.at("access_token").get_to(accessToken);
		loaded = true;
	} catch (...) {}
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
		curlpp::Easy request(handle->clone());
		request.setOpt(curlpp::options::Url(pUrl + "?" + pData));

		std::list<std::string> header = {"Content-Type: application/x-www-form-urlencoded"};
		header.emplace_back("Authorization: " + tokenType + " " + accessToken);
		request.setOpt(curlpp::options::HttpHeader(header));
		std::stringstream ss;
		request.setOpt(curlpp::options::WriteStream(&ss));
		curlpp::options::ProgressFunction progressBar(
			[&pProgress](double pDltotal, double pDlnow, double /*ultotal*/, double /*ulnow*/) {
				auto progress = static_cast<float>(pDlnow / pDltotal);
				//sdk::Logger::out("Progress: {}", progress);
				pProgress.store(progress);
				return 0;
			});
		request.setOpt(curlpp::options::NoProgress(false));
		request.setOpt(progressBar);
		request.perform();
		auto endTime = std::chrono::steady_clock::now();
		auto duration = endTime - startTime;

		sdk::Logger::out("Response in: {}ms", duration.count() / 1000000);
		ss >> pResult;
		return nullptr;

	} catch (...) {
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
		//curlpp::
		curlpp::Easy request(handle->clone());
		request.setOpt(curlpp::options::Url(pUrl));

		/*std::list<std::string> header = {"Content-Type: application/x-www-form-urlencoded"};
				request.setOpt(new curlpp::options::HttpHeader(header));*/
		request.setOpt(curlpp::options::WriteFunction(
			[&pUncompressedJpegData](const char* pBuf, const size_t pSize, const size_t pNmemb) -> size_t {
				std::string str(pBuf, pSize * pNmemb);
				pUncompressedJpegData.insert(pUncompressedJpegData.cend(), str.begin(), str.end());
				return pSize * pNmemb;
			}));

		request.perform();

	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Image downloading error");
		msg->setMessage("Exception occurred during getting an image");
		msg->addInfoLine("Address: {}", pUrl);
		return msg;
	}
	if ((pCache & CACHE_NO_SAVE) != CACHE_NO_SAVE) {
		if (auto msg = cache->saveCache(pathToCache, pUncompressedJpegData)) { sdk::Logger::error(msg); }
	}
	/*uint64_t jpegSize = uncompressedJpegData.size();
	std::vector<unsigned char> data;
	unsigned char* compressedImage = uncompressedJpegData.data();
	if (auto msg = decompressJpeg(compressedImage, jpegSize, data)) {
		msg->setTitle("Image downloading error");
		msg->addInfoLine("Address: {}", pUrl);
		return msg;
	}*/
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
			curlpp::Easy request(handle->clone());
			request.setOpt(curlpp::options::Url(pUrl));

			/*std::list<std::string> header = {"Content-Type: application/x-www-form-urlencoded"};
				request.setOpt(new curlpp::options::HttpHeader(header));*/
			pStreamOut = std::make_shared<std::stringstream>();
			request.setOpt(curlpp::options::WriteStream(pStreamOut.get()));
			curlpp::options::ProgressFunction progressBar(
				[&pProgress](double pDltotal, double pDlnow, double /*ultotal*/, double /*ulnow*/) {
					pProgress = static_cast<float>(pDlnow / pDltotal);
					return 0;
				});
			request.setOpt(curlpp::options::NoProgress(false));
			request.setOpt(progressBar);

			//TODO use curl_multi_perform
			request.perform();

		} catch (...) {
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