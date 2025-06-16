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

#ifndef SKETCHFABSEARCH_H
#define SKETCHFABSEARCH_H
#include <future>
#include <nlohmann/json_fwd.hpp>
#include <thread>

#include "KwasarEngine/utils/IReportable.h"

namespace mer::sdk {
class GltfModel;
}

namespace mer::editor::mvp {
class SketchfabAccount;

class SearchRequest {
public:
	//Space separated keywords
	std::string query;
	//Searches models by a user (username)
	std::string user;
	//Searches models by tags (slug)
	std::vector<std::string> tags;
	//Searches models by categories (slug)
	std::vector<std::string> categories;
	//Limit search to a specific period only (in days)
	std::optional<int> date;
	std::optional<bool> downloadable = true;
	std::optional<bool> animated;
	std::optional<bool> staffpicked;
	std::optional<bool> hasSound;
	//Minimum number of faces
	std::optional<int> minFaceCount;
	//Maximum number of faces
	std::optional<int> maxFaceCount;
	//Filter by PBR type.
	//Set to "metalness" to search Metalness/Roughness models only.
	//Set to "specular" to search Specular/Glossiness models only.
	//Set to "true" to search PBR models only.
	//Set to "false" to search non-PBR models only.
	std::string pbrType = "metalness";
	//"false" by default.
	//Set to "true" to search rigged models only.
	//Set to "false" to search all models
	std::optional<bool> rigged;
	//Searches models by collection (uid)
	std::string collection;
	//How to sort results. When omitted, results are sorted by relevance.
	//likeCount, -likeCount, viewCount, -viewCount, publishedAt, -publishedAt, processedAt, -processedAt
	std::string sortBy = "-likeCount";
	//Searches models by file format. Ex: obj, fbx, blend...
	std::string fileFormat;
	//by,by-sa, by-nd, by-nc, by-nc-sa, by-nc-nd, cc0, ed, st
	std::string license;

	std::string getRequestData(int pCursor, int pCount = 24) const;
};

class Image {
	uint32_t imageId{};
	bool dataLoaded{};
	std::vector<unsigned char> data;
	SketchfabAccount* account;

public:
	std::string url;
	std::string uid;
	std::optional<int> width;
	std::optional<int> height;
	std::optional<int> size;

	friend void from_json(const nlohmann::json &pJson, std::shared_ptr<Image> &pValue) {
		pValue = std::make_shared<Image>();
		pValue->serialize(pJson);
	}

	void serialize(const nlohmann::json &pJson);

	sdk::ReportMessagePtr loadImage();

	[[nodiscard]] bool isLoaded() const { return !data.empty(); }

	[[nodiscard]] uint32_t getImageId();

	[[nodiscard]] SketchfabAccount* getAccount() const { return account; }

	void setAccount(SketchfabAccount* pAccount) { account = pAccount; }
};

class UserRelated {
	std::shared_ptr<Image> smallAvatar;
	SketchfabAccount* account;

public:
	std::string username;
	std::string profileUrl;
	std::string accountType;
	std::string displayName;
	std::string uid;
	std::string uri;
	std::vector<std::shared_ptr<Image>> avatarImages;

	friend void from_json(const nlohmann::json &pJson, std::shared_ptr<UserRelated> &pValue) {
		pValue = std::make_shared<UserRelated>();
		pValue->serialize(pJson);
	}

	void serialize(const nlohmann::json &pJson);

	[[nodiscard]] const std::shared_ptr<Image> &getSmallAvatar() const;

	[[nodiscard]] SketchfabAccount* getAccount() const { return account; }

	void setAccount(SketchfabAccount* pAccount);
};

class ArchiveNested {
	std::optional<int> faceCount;
	std::optional<int> textureCount;
	std::optional<int> size;

	/*archive size (in bytes)
	,*/
	std::optional<int> vertexCount;
	std::optional<int> textureMaxResolution;
};

class inline_model_7 {
	std::optional<ArchiveNested> source;
	std::optional<ArchiveNested> glb;
	std::optional<ArchiveNested> usdz;
	std::optional<ArchiveNested> gltf;
};

class DownloadableModel {
	SketchfabAccount* account;
	std::shared_ptr<std::iostream> stream;

public:
	std::string url;
	std::optional<std::chrono::system_clock::time_point> expiresAt;
	std::optional<size_t> size;

	bool isExpired() const;

	friend void from_json(const nlohmann::json &pJson, std::shared_ptr<DownloadableModel> &pValue) {
		pValue = std::make_shared<DownloadableModel>();
		pValue->serialize(pJson);
	}

	void serialize(const nlohmann::json &pJson);

	std::shared_ptr<sdk::ReportMessage> download(std::atomic<float> &pProgress);

	[[nodiscard]] SketchfabAccount* getAccount() const { return account; }

	void setAccount(SketchfabAccount* pAccount) { account = pAccount; }

	[[nodiscard]] const std::shared_ptr<std::iostream> &getStream() const { return stream; }

	void resetStream() { stream.reset(); }
};

class DownloadLinks {
	SketchfabAccount* account;

public:
	std::shared_ptr<DownloadableModel> glb;
	std::shared_ptr<DownloadableModel> gltf;
	std::shared_ptr<DownloadableModel> usdz;
	std::shared_ptr<DownloadableModel> source;

	friend void from_json(const nlohmann::json &pJson, std::shared_ptr<DownloadLinks> &pValue) {
		pValue = std::make_shared<DownloadLinks>();
		pValue->serialize(pJson);
	}

	void serialize(const nlohmann::json &pJson);

	[[nodiscard]] SketchfabAccount* getAccount() const { return account; }

	void setAccount(SketchfabAccount* pAccount) {
		account = pAccount;
		if (glb) glb->setAccount(account);
		if (source) source->setAccount(account);
		if (gltf) gltf->setAccount(account);
		if (usdz) usdz->setAccount(account);
	}
};

class ModelSearchList : public sdk::IReportable {
	std::shared_ptr<Image> smallThumbnail;
	SketchfabAccount* account;
	//Call downloadLinks() first
	std::shared_ptr<DownloadLinks> links;

public:
	std::string uid;
	std::optional<int> animationCount;
	std::string viewerUrl;
	std::string publishedAt;
	std::optional<int> likeCount;
	std::optional<int> commentCount;
	std::shared_ptr<UserRelated> user;
	std::optional<bool> isDownloadable;
	std::string name;
	std::optional<int> viewCount;
	std::vector<std::shared_ptr<Image>> thumbnails;
	std::string license;
	std::optional<bool> isPublished;
	std::string staffpickedAt;
	std::string createdAt;
	std::optional<inline_model_7> archives;
	std::string embedUrl;
	std::vector<std::string> tags;
	std::vector<std::string> categories;
	std::string description;
	std::optional<int> faceCount;
	std::optional<bool> isAgeRestricted;
	std::string uri;
	std::optional<int> vertexCount;

	friend void to_json(nlohmann::json &pJson, const ModelSearchList &pValue) { pValue.deserialize(pJson); }

	friend void from_json(const nlohmann::json &pJson, std::shared_ptr<ModelSearchList> &pValue) {
		pValue = std::make_shared<ModelSearchList>();
		pValue->serialize(pJson);
	}

	void deserialize(nlohmann::json &pJson) const;

	void serialize(const nlohmann::json &pJson);

	[[nodiscard]] const std::shared_ptr<Image> &getSmallThumbnail() const { return smallThumbnail; }

	[[nodiscard]] SketchfabAccount* getAccount() const { return account; }

	void setAccount(SketchfabAccount* pAccount);

	std::shared_ptr<sdk::ReportMessage> downloadLinks(std::atomic<float> &pProgress);

	void addReportInfo(const sdk::ReportMessagePtr &pMsg) const override;

	//Call downloadLinks() first
	[[nodiscard]] const std::shared_ptr<DownloadLinks> &getLinks() const { return links; }
};

class SketchfabSearch {
	SearchRequest request;
	SketchfabAccount* account;
	std::vector<std::shared_ptr<ModelSearchList>> results;
	std::optional<std::jthread> loadingThread;
	bool loadingInProgress{};
	std::mutex loadingInProgressMutex;

public:
	explicit SketchfabSearch(SketchfabAccount* pAccount) : account(pAccount) {}

	void setRequest(const SearchRequest &pRequest) { request = pRequest; }

	void next(const std::function<void(const sdk::ReportMessagePtr &pError)> &pCallback);

	[[nodiscard]] const std::vector<std::shared_ptr<ModelSearchList>> &getResults() const { return results; }

	[[nodiscard]] bool isLoadingInProgress() const { return loadingInProgress; }

	void clear() { results.clear(); }
};
} // namespace mer::editor::mvp

#endif //SKETCHFABSEARCH_H
