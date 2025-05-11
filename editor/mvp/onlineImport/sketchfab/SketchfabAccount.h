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

#ifndef SKETCHFABACCOUNT_H
#define SKETCHFABACCOUNT_H
#include <atomic>
#include <nlohmann/json_fwd.hpp>

namespace mer::sdk {
class GltfModel;
}

namespace mer::editor::mvp {
class SketchfabSearch;
class SketchfabCache;
class SketchfabAccount;

/*
class ModelSearchResponse {
	std::string next;
	std::vector<ModelSearchList> results;
};
*/

enum CacheSettings {
	CACHE_DEFAULT = 0,
	CACHE_NO_SAVE = 1 << 0,
	CACHE_NO_LOAD = 1 << 1,
};

class SketchfabAccount {
	static inline std::atomic<int> parallelRequests{0};
	std::string accessToken;
	int expiresIn{};
	std::string tokenType;
	std::string scope;
	std::string refreshToken;
	std::shared_ptr<SketchfabCache> cache;

	bool loaded{false};

	SketchfabAccount();

public:
	static std::shared_ptr<SketchfabAccount> createFromFile(const std::filesystem::path &pFilePath,
															sdk::ReportMessagePtr &pMessage);

	static std::shared_ptr<SketchfabAccount> create();

	sdk::ReportMessagePtr saveToFile(const std::filesystem::path &pFilePath);

	std::shared_ptr<SketchfabSearch> createSearch();

	void deserialize(const nlohmann::json &pRoot);

	void serialize(nlohmann::json &pRoot);

	sdk::ReportMessagePtr getRequest(const std::string &pUrl, const std::string &pData, std::atomic<float> &pProgress,
									 nlohmann::json &pResult);

	sdk::ReportMessagePtr downloadImage(const std::string &pUrl, std::vector<unsigned char> &pUncompressedJpegData,
										const CacheSettings &pCache = CACHE_DEFAULT);

	sdk::ReportMessagePtr downloadModel(const std::string &pUrl, std::shared_ptr<std::iostream> &pStreamOut,
										std::atomic<float> &pProgress, const CacheSettings &pCache = CACHE_NO_LOAD);

	bool isFileCached(const std::string &pUrl) const;


	sdk::ReportMessagePtr loadCachedFile(const std::string &pUrl, std::shared_ptr<std::iostream> &pData) const;

	[[nodiscard]] const std::string &getAccessToken() const { return accessToken; }

	void setAccessToken(const std::string &pAccessToken) { accessToken = pAccessToken; }

	[[nodiscard]] int getExpiresIn() const { return expiresIn; }

	void setExpiresIn(int pExpiresIn) { expiresIn = pExpiresIn; }

	[[nodiscard]] const std::string &getTokenType() const { return tokenType; }

	void setTokenType(const std::string &pTokenType) { tokenType = pTokenType; }

	[[nodiscard]] const std::string &getScope() const { return scope; }

	void setScope(const std::string &pScope) { scope = pScope; }

	[[nodiscard]] const std::string &getRefreshToken() const { return refreshToken; }

	void setRefreshToken(const std::string &pRefreshToken) { refreshToken = pRefreshToken; }

	[[nodiscard]] bool isLoaded() const { return loaded; }

	void setLoaded(bool pLoaded) { loaded = pLoaded; }
};

} // namespace mer::editor::mvp

#endif //SKETCHFABACCOUNT_H
