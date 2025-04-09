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

#ifndef MODELONLINEIMPORT_H
#define MODELONLINEIMPORT_H
#include <future>

namespace mer::sdk {
class GltfModel;
}

namespace mer::editor::mvp {
class SketchfabSearch;
class ModelSearchList;
class IPresenterOnlineImport;
class SketchfabAccount;

class IModelOnlineImport {
public:
	virtual ~IModelOnlineImport() = default;

	[[nodiscard]] virtual bool isLoggedIn() const = 0;

	virtual void setLoggedIn(bool pLoggedIn) = 0;

	[[nodiscard]] virtual const std::shared_ptr<SketchfabAccount> &getAccount() const = 0;

	virtual void setAccount(const std::shared_ptr<SketchfabAccount> &pAccount) = 0;

	virtual const std::vector<std::shared_ptr<ModelSearchList>> &getSearchResult() const = 0;

	virtual std::future<std::shared_ptr<sdk::ReportMessage>> nextSearchResult() const = 0;

	[[nodiscard]] virtual const std::shared_ptr<ModelSearchList> &getSelectedModel() const = 0;

	virtual void setSelectedModel(const std::shared_ptr<ModelSearchList> &pSelectedModel) = 0;

	[[nodiscard]] virtual bool isModelInCache() const = 0;

	virtual sdk::ReportMessagePtr loadModelFromCache(std::shared_ptr<std::iostream> &pData) const = 0;

	[[nodiscard]] virtual IPresenterOnlineImport* getPresenter() const = 0;

	virtual void setPresenter(IPresenterOnlineImport* pPresenter) = 0;
};

class ModelOnlineImport : public IModelOnlineImport {
	bool loggedIn{};
	std::shared_ptr<SketchfabAccount> account;
	std::shared_ptr<SketchfabSearch> search;
	std::shared_ptr<ModelSearchList> selectedModel;
	std::shared_ptr<sdk::GltfModel> loadedGltfModel;
	IPresenterOnlineImport* presenter{};

public:
	[[nodiscard]] bool isLoggedIn() const override { return loggedIn; }

	void setLoggedIn(bool pLoggedIn) override { loggedIn = pLoggedIn; }

	[[nodiscard]] const std::shared_ptr<SketchfabAccount> &getAccount() const override { return account; }

	void setAccount(const std::shared_ptr<SketchfabAccount> &pAccount) override;

	const std::vector<std::shared_ptr<ModelSearchList>> &getSearchResult() const override;

	std::future<std::shared_ptr<sdk::ReportMessage>> nextSearchResult() const override;

	[[nodiscard]] const std::shared_ptr<ModelSearchList> &getSelectedModel() const override { return selectedModel; }

	void setSelectedModel(const std::shared_ptr<ModelSearchList> &pSelectedModel) override;

	[[nodiscard]] bool isModelInCache() const override;

	sdk::ReportMessagePtr loadModelFromCache(std::shared_ptr<std::iostream> &pData) const;

	[[nodiscard]] IPresenterOnlineImport* getPresenter() const override { return presenter; }

	void setPresenter(IPresenterOnlineImport* pPresenter) override { presenter = pPresenter; }
};
} // namespace mer::editor::mvp

#endif //MODELONLINEIMPORT_H
