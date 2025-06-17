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
// Created by alexus on 17.02.25.
//

#ifndef PRESENTERONLINEIMPORT_H
#define PRESENTERONLINEIMPORT_H
#include <future>
#include <memory>
#include <thread>

#include "mvp/IPresenter.h"
#include "mvp/scenePreview/ModelScenePreview.h"

namespace ke {
class GltfModel;
}

namespace ked {
class ModelSearchList;
class DownloadableModel;
class DownloadLinks;
class IModelScenePreview;
class IModelOnlineImport;
class IViewOnlineImport;

class IPresenterOnlineImport : public IPresenter {
public:
	~IPresenterOnlineImport() override = default;

	virtual void loginImplicit(const std::string &pUsername, const std::string &pPassword) = 0;

	virtual void loginToken(const std::string &pToken) = 0;

	virtual void nextSearchResult() const = 0;

	virtual void onSearchRequestChanged() = 0;

	virtual void onSearchResultLoaded(const ke::ReportMessagePtr &pError) = 0;

	virtual void selectModel(const std::shared_ptr<ModelSearchList> &pModel) = 0;

	virtual void onSelectedModelChanged() = 0;

	virtual void onAccountSet() = 0;

	virtual const std::vector<std::shared_ptr<ModelSearchList>> &getSearchResult() const = 0;

	virtual bool hasDownloadLinks() = 0;

	virtual void setPreviewModel(const std::shared_ptr<ModelScenePreview> &pPreview) = 0;

	virtual bool isSearching() = 0;
};

class PresenterOnlineImport : public IPresenterOnlineImport {
	std::shared_ptr<IModelOnlineImport> model;
	std::shared_ptr<IViewOnlineImport> view;
	std::shared_ptr<IModelScenePreview> modelPreview;
	std::optional<std::jthread> loadingThread;
	std::optional<std::jthread> searchDelayThread;

	std::atomic<std::chrono::steady_clock::time_point> searchTime{};

	PresenterOnlineImport(const std::shared_ptr<IModelOnlineImport> &pModel,
						  const std::shared_ptr<IViewOnlineImport> &pView);

public:
	static std::shared_ptr<PresenterOnlineImport> create(const std::shared_ptr<IModelOnlineImport> &pModel,
														 const std::shared_ptr<IViewOnlineImport> &pView);

	void loginImplicit(const std::string &pUsername, const std::string &pPassword) override;

	void loginToken(const std::string &pToken) override;

	void onAccountSet() override;

	const std::vector<std::shared_ptr<ModelSearchList>> &getSearchResult() const override;

	void nextSearchResult() const override;

	void onSearchRequestChanged() override;

	void onSearchResultLoaded(const ke::ReportMessagePtr &pError) override;

	void selectModel(const std::shared_ptr<ModelSearchList> &pModel) override;

	void onSelectedModelChanged() override;

	bool hasDownloadLinks() override { return getGltfLinks() != nullptr; }

	void setPreviewModel(const std::shared_ptr<ModelScenePreview> &pPreview) override { modelPreview = pPreview; }

	bool isSearching() override;

	void run() override;

	void stop() override;

	std::string getTypeName() override { return "PresenterOnlineImport"; }

private:
	std::shared_ptr<DownloadableModel> getGlbLink() const;

	std::shared_ptr<DownloadLinks> getGltfLinks() const;
};
} // namespace ked

#endif //PRESENTERONLINEIMPORT_H
