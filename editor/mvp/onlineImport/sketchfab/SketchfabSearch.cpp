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

#include "SketchfabSearch.h"

#include <epoxy/gl.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <turbojpeg.h>

#include "Globals.h"
#include "SketchfabAccount.h"

#define API_ADDRESS "https://api.sketchfab.com"
#define API_V3 API_ADDRESS "/v3"

namespace mer::editor::mvp {
std::string flat(const std::vector<std::string> &pElems, const std::string &pDelimiter) {
	if (pDelimiter.empty()) return "";
	return std::accumulate(std::next(pElems.begin()), pElems.end(), pElems[0],
						   [pDelimiter](const std::string &pA, const std::string &pB) { return pA + pDelimiter + pB; });
}

std::shared_ptr<Image> get_closest_image(const std::vector<std::shared_ptr<Image>> &pImages, const int pRequestedWidth,
										 const int pRequestedHeight) {
	std::vector<std::pair<int, std::shared_ptr<Image>>> scores;
	for (const auto &thumbnail: pImages) {
		const int width = thumbnail->width.value_or(0);
		const int height = thumbnail->height.value_or(0);
		if (width == 0 || height == 0) continue;
		const auto score = width - pRequestedWidth + height - pRequestedHeight;
		if (score < 0) continue;
		scores.emplace_back(std::make_pair(score, thumbnail));
	}

	std::ranges::sort(scores, [](const std::pair<int, std::shared_ptr<Image>> &pA,
								 const std::pair<int, std::shared_ptr<Image>> &pB) {
		return pA.first < pB.first;
	});
	return scores.front().second;
}

std::string SearchRequest::getRequestData(int pCursor, int pCount) const {
	using namespace std::string_literals;
	std::vector<std::string> fields;
	fields.emplace_back(std::format("type=models&cursor={}&count={}", pCursor, pCount));
	if (!query.empty()) fields.emplace_back("q=" + query);
	if (!user.empty()) fields.emplace_back("user=" + user);
	if (!tags.empty()) fields.emplace_back("tags=" + flat(tags, "%2C"));
	if (!categories.empty()) fields.emplace_back("categories=" + flat(categories, "%2C"));
	if (date.has_value()) fields.emplace_back("date=" + std::to_string(date.value()));
	if (downloadable.has_value()) fields.emplace_back("downloadable="s + (downloadable.value() ? "true" : "false"));
	if (animated.has_value()) fields.emplace_back("animated="s + (animated.value() ? "true" : "false"));
	if (staffpicked.has_value()) fields.emplace_back("staffpicked="s + (staffpicked.value() ? "true" : "false"));
	if (hasSound.has_value()) fields.emplace_back("sound="s + (hasSound.value() ? "true" : "false"));
	if (minFaceCount.has_value()) fields.emplace_back("min_face_count=" + std::to_string(minFaceCount.value()));
	if (maxFaceCount.has_value()) fields.emplace_back("max_face_count=" + std::to_string(maxFaceCount.value()));
	if (!pbrType.empty()) fields.emplace_back("pbr_type=" + pbrType);
	if (rigged.has_value()) fields.emplace_back("rigged="s + (rigged.value() ? "true" : "false"));
	if (!collection.empty()) fields.emplace_back("collection=" + collection);
	if (!sortBy.empty()) fields.emplace_back("sort_by=" + sortBy);
	if (!fileFormat.empty()) fields.emplace_back("file_format=" + fileFormat);
	if (!license.empty()) fields.emplace_back("license=" + license);

	return flat(fields, "&");
}

void Image::serialize(const nlohmann::json &pJson) {
	try { pJson.at("uid").get_to(uid); }
	catch (...) { uid.clear(); }
	try { pJson.at("url").get_to(url); }
	catch (...) { url.clear(); }

	try { width = pJson.at("width").get<int>(); }
	catch (...) { width.reset(); }
	try { height = pJson.at("height").get<int>(); }
	catch (...) { height.reset(); }
	try { size = pJson.at("size").get<int>(); }
	catch (...) { size.reset(); }
}

void UserRelated::serialize(const nlohmann::json &pJson) {
	try { pJson.at("username").get_to(username); }
	catch (...) { username.clear(); }
	try { pJson.at("profileUrl").get_to(profileUrl); }
	catch (...) { profileUrl.clear(); }
	try { pJson.at("account").get_to(accountType); }
	catch (...) { accountType.clear(); }
	try { pJson.at("displayName").get_to(displayName); }
	catch (...) { displayName.clear(); }
	try { pJson.at("uid").get_to(uid); }
	catch (...) { uid.clear(); }
	try { pJson.at("uri").get_to(uri); }
	catch (...) { uri.clear(); }
	try { pJson.at("avatar").at("images").get_to(avatarImages); }
	catch (...) { avatarImages.clear(); }
	if (!avatarImages.empty()) { smallAvatar = get_closest_image(avatarImages, 32, 32); }
}

const std::shared_ptr<Image> &UserRelated::getSmallAvatar() const { return smallAvatar; }

void UserRelated::setAccount(SketchfabAccount* pAccount) {
	account = pAccount;
	for (auto avatarImage: avatarImages) avatarImage->setAccount(pAccount);
}

bool DownloadableModel::isExpired() const {
	using namespace std::chrono_literals;
	if (!expiresAt.has_value()) return false;
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	return expiresAt.value() + 2s <= now;
}

void DownloadableModel::serialize(const nlohmann::json &pJson) {
	try { pJson.at("url").get_to(url); }
	catch (...) { url.clear(); }
	try { size = pJson.at("animationCount").get<size_t>(); }
	catch (...) { size.reset(); }
	try {
		int expiresIn;
		pJson.at("expires").get_to<>(expiresIn);
		if (expiresIn > 0) {
			std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
			expiresAt = now + std::chrono::seconds(expiresIn);
		} else
			expiresAt.reset();
	}
	catch (...) { expiresAt.reset(); }
}

std::shared_ptr<ke::ReportMessage> DownloadableModel::download(std::atomic<float> &pProgress) {
	if (auto msg = account->downloadModel(url, stream, pProgress)) {
		//addReportInfo(msg);
		return msg;
	}

	return nullptr;
}

void DownloadLinks::serialize(const nlohmann::json &pJson) {
	try { pJson.at("glb").get_to(glb); }
	catch (...) { glb.reset(); }

	try { pJson.at("gltf").get_to(gltf); }
	catch (...) { gltf.reset(); }
	try { pJson.at("usdz").get_to(usdz); }
	catch (...) { usdz.reset(); }

	try { pJson.at("source").get_to(source); }
	catch (...) { source.reset(); }
}

ke::ReportMessagePtr Image::loadImage() {
	std::vector<unsigned char> jpegData;
	if (auto msg = account->downloadImage(url, jpegData)) {
		msg->addInfoLine("Address: {}", url);
		return msg;
	}

	dataLoaded = false;
	if (auto msg = Utils::decompressJpeg(jpegData.data(), jpegData.size(), data)) {
		msg->addInfoLine("Address: {}", url);
		return msg;
	}
	dataLoaded = true;
	return nullptr;
}

uint32_t Image::getImageId() {
	if (imageId == 0 && dataLoaded) {
		glGenTextures(1, &imageId);
		glBindTexture(GL_TEXTURE_2D, imageId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(width.value()), static_cast<int>(height.value()), 0,
					 GL_RGB, GL_UNSIGNED_BYTE, data.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return imageId;
}

void ModelSearchList::deserialize(nlohmann::json &pJson) const {
	pJson["uid"] = uid;
	pJson["animationCount"] = animationCount.value();
}

void ModelSearchList::serialize(const nlohmann::json &pJson) {
	try { pJson.at("uid").get_to(uid); }
	catch (...) { uid.clear(); }

	try { animationCount = pJson.at("animationCount").get<int>(); }
	catch (...) { animationCount.reset(); }
	try { pJson.at("viewerUrl").get_to(viewerUrl); }
	catch (...) { viewerUrl.clear(); }
	try { pJson.at("publishedAt").get_to(publishedAt); }
	catch (...) { publishedAt.clear(); }
	try { likeCount = pJson.at("likeCount").get<int>(); }
	catch (...) { likeCount.reset(); }
	try { commentCount = pJson.at("commentCount").get<int>(); }
	catch (...) { commentCount.reset(); }
	try { pJson.at("user").get_to(user); }
	catch (...) { user.reset(); }
	try { isDownloadable = pJson.at("isDownloadable").get<bool>(); }
	catch (...) { isDownloadable.reset(); }
	try { pJson.at("name").get_to(name); }
	catch (...) { name.clear(); }
	try { viewCount = pJson.at("viewCount").get<int>(); }
	catch (...) { viewCount.reset(); }
	try { pJson.at("thumbnails").at("images").get_to(thumbnails); }
	catch (...) { thumbnails.clear(); }
	if (!thumbnails.empty()) { smallThumbnail = get_closest_image(thumbnails, 330, 200); }

	try { pJson.at("license").get_to(license); }
	catch (...) { license.clear(); }
	try { isPublished = pJson.at("isPublished").get<bool>(); }
	catch (...) { isPublished.reset(); }
	try { pJson.at("staffpickedAt").get_to(staffpickedAt); }
	catch (...) { staffpickedAt.clear(); }
	/*try {
		archives = pJson.at("archives").get<inline_model_7>();
	} catch (...) { archives.reset(); }*/
	try { pJson.at("embedUrl").get_to(embedUrl); }
	catch (...) { embedUrl.clear(); }
	try { pJson.at("tags").get_to(tags); }
	catch (...) { tags.clear(); }
	try { pJson.at("categories").get_to(categories); }
	catch (...) { categories.clear(); }
	try { pJson.at("createdAt").get_to(createdAt); }
	catch (...) { createdAt.clear(); }
	try { pJson.at("description").get_to(description); }
	catch (...) { description.clear(); }
	try { faceCount = pJson.at("faceCount").get<int>(); }
	catch (...) { faceCount.reset(); }
	try { isAgeRestricted = pJson.at("isAgeRestricted").get<bool>(); }
	catch (...) { isAgeRestricted.reset(); }
	try { pJson.at("uri").get_to(uri); }
	catch (...) { uri.clear(); }
	try { vertexCount = pJson.at("vertexCount").get<int>(); }
	catch (...) { vertexCount.reset(); }
}

void ModelSearchList::setAccount(SketchfabAccount* pAccount) {
	account = pAccount;
	for (const auto &thumbnail: thumbnails) {
		thumbnail->setAccount(pAccount);
		user->setAccount(pAccount);
	}
	std::jthread thread([this] {
		if (const auto msg = smallThumbnail->loadImage()) ke::Logger::error(msg);
		if (const auto msg = user->getSmallAvatar()->loadImage()) ke::Logger::error(msg);
	});
	thread.detach();
}

std::shared_ptr<ke::ReportMessage> ModelSearchList::downloadLinks(std::atomic<float> &pProgress) {
	std::string address = API_V3 "/models/" + uid + "/download";
	nlohmann::json j;
	if (auto msg = account->getRequest(address, "", pProgress, j)) {
		addReportInfo(msg);
		return msg;
	}

	if (auto it = j.find("error"); it != j.end()) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to get download links");
		msg->setMessage("Server returned an error");
		addReportInfo(msg);
		msg->addInfoLine("Address: {}", address);
		msg->addInfoLine("Error: {}", it.value().get<std::string>());
		if (auto errorIt = j.find("error_description"); it != j.end())
			msg->addInfoLine("Error description: {}", errorIt.value().get<std::string>());
		return msg;
	}
	if (auto it = j.find("detail"); it != j.end()) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to get download links");
		msg->setMessage("Server returned an error.");
		//TODO renew an expired token
		msg->addInfoLine("THIS IS A BUG. Token expired, but I'm unable to renew it automatically. You can delete "
			"sketchfab-account.json file to force re-login and update the token.");
		msg->addInfoLine("File sketchfab-account.json located at {}", Globals::getConfigPath().string());
		addReportInfo(msg);
		msg->addInfoLine("Address: {}", address);
		msg->addInfoLine("Error: {}", it.value().get<std::string>());
		return msg;
	}

	try {
		j.get_to(links);
		links->setAccount(account);
	}
	catch (...) {
		links.reset();
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to get download links");
		msg->setMessage("Parsing error");
		addReportInfo(msg);
		msg->addInfoLine("Address: {}", address);
		msg->addInfoLine("Response: {}", j.dump(2));
		return msg;
	}
	//ke::Logger::out("{}", j.dump(2));

	return nullptr;
}

void ModelSearchList::addReportInfo(const ke::ReportMessagePtr &pMsg) const {
	pMsg->addInfoLine("Model UID: {}", uid);
	pMsg->addInfoLine("Model name: {}", name);
	pMsg->addInfoLine("Model viewer url: {}", viewerUrl);
}

void SketchfabSearch::next(const std::function<void(const ke::ReportMessagePtr &pError)> &pCallback) {
	{
		std::lock_guard lock(loadingInProgressMutex);
		if (loadingInProgress) return;
	}
	loadingThread = std::jthread([this, pCallback](const std::stop_token & /*pToken*/) {
		std::lock_guard lock1(loadingInProgressMutex);
		if (loadingInProgress) return;
		loadingInProgress = true;
		std::string address = API_V3 "/search";
		std::string requestData = request.getRequestData(static_cast<int>(results.size()));
		nlohmann::json j;
		std::atomic<float> progress;
		if (auto msg = account->getRequest(address, requestData, progress, j)) {
			pCallback(msg);
			loadingInProgress = false;
			return;
		}
		if (auto it = j.find("error"); it != j.end()) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Search Error");
			msg->setMessage("Server returned an error");
			msg->addInfoLine("Address: {}", address);
			msg->addInfoLine("Data: {}", requestData);
			msg->addInfoLine("Error: {}", it.value().get<std::string>());
			if (auto errorIt = j.find("error_description"); it != j.end())
				msg->addInfoLine("Error description: {}", errorIt.value().get<std::string>());
			pCallback(msg);
			loadingInProgress = false;
			return;
		}
		//ke::Logger::out("{}", j.dump(2));
		//j.at("next")
		std::vector<std::shared_ptr<ModelSearchList>> searchList;
		/*for (auto result: j.at("results")) {
			auto list = std::make_shared<ModelSearchList>();
			list->serialize(result);
			searchList.emplace_back();
		}*/
		j.at("results").get_to(searchList);
		for (const auto &result: searchList) {
			result->setAccount(account);
			/*for (auto modelSearchList: results) {
				if (modelSearchList->uid == result->uid) continue;
				results.emplace_back(modelSearchList);
			}*/
		}
		results.insert(results.end(), searchList.begin(), searchList.end());
		/*if (results.size() > 48) {
			results.erase(results.begin(), results.begin() + static_cast<long int>(results.size() - 48ul));
		}*/
		pCallback(nullptr);
		loadingInProgress = false;
	});
}
} // namespace mer::editor::mvp
