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
// Created by alexus on 17.02.25.
//

#include "ModelOnlineImport.h"

#include <nlohmann/json.hpp>

#include "PresenterOnlineImport.h"
#include "sketchfab/SketchfabAccount.h"
#include "sketchfab/SketchfabSearch.h"

namespace ked {
void ModelOnlineImport::setAccount(const std::shared_ptr<SketchfabAccount> &pAccount) {
	account = pAccount;
	search = account->createSearch();
	if (presenter) presenter->onAccountSet();
	nextSearchResult();
}

const std::vector<std::shared_ptr<ModelSearchList>> &ModelOnlineImport::getSearchResult() const {
	return search->getResults();
}

void ModelOnlineImport::setSearchRequest(const SearchRequest &pRequest) {
	search->clear();
	search->setRequest(pRequest);
}

void ModelOnlineImport::nextSearchResult() const {
	if (!presenter) return;
	search->next([this](const ke::ReportMessagePtr &pError) { presenter->onSearchResultLoaded(pError); });
}

void ModelOnlineImport::setSelectedModel(const std::shared_ptr<ModelSearchList> &pSelectedModel) {
	selectedModel = pSelectedModel;
	if (presenter) presenter->onSelectedModelChanged();
	if (!pSelectedModel) return;
	/*nlohmann::json j;
	if (auto msg =
			account->getRequest("https://api.sketchfab.com/v3/models/" + selectedModel->uid + "/download", "", j)) {
		ke::Logger::error(msg);
		return;
	}
	ke::Logger::out(j.dump(2));*/
}

bool ModelOnlineImport::isModelInCache() const {
	if (!selectedModel) return false;
	return account->isFileCached(selectedModel->uid);
}

ke::ReportMessagePtr ModelOnlineImport::loadModelFromCache(std::shared_ptr<std::iostream> &pData) const {
	if (!selectedModel) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Unable to load model from cache");
		msg->setMessage("No selected model to load. It is a programmer error");
		return msg;
	}
	return account->loadCachedFile(selectedModel->uid, pData);
}

bool ModelOnlineImport::isSearching() { return search->isLoadingInProgress(); }
} // namespace ked
