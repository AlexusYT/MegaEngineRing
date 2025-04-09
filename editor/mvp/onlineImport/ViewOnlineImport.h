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

#ifndef VIEWONLINEIMPORT_H
#define VIEWONLINEIMPORT_H
#include "mvp/IView.h"
#include "mvp/editor/Editor.h"

namespace mer::editor::mvp {
class IPresenterScenePreview;
class ModelSearchList;
class IPresenterOnlineImport;

class IViewOnlineImport : public IView {
public:
	~IViewOnlineImport() override = default;

	enum class ProgressMode { NONE, DOWNLOAD_LINKS, DOWNLOAD_MODEL, PARSE_MODEL };

	virtual void showLoginDialog() = 0;

	virtual void hideLoginDialog() = 0;

	virtual void loginError(const std::string &pError, const std::string &pMessage) = 0;

	virtual void setLoginInProgress() = 0;

	virtual void showModelLoading() = 0;

	virtual void hideModelLoading() = 0;

	virtual void setProgressMode(ProgressMode pProgressMode) = 0;

	virtual std::atomic<float> &getProgressAtomic() = 0;

	[[nodiscard]] virtual IPresenterOnlineImport* getPresenter() const = 0;

	virtual void setPresenter(IPresenterOnlineImport* pPresenter) = 0;
};

class ViewOnlineImport : public IViewOnlineImport, public EditorTool {
	std::shared_ptr<IWidgetContext> context;
	IPresenterOnlineImport* presenter{};
	bool shouldLoginDialog{};
	std::atomic<bool> loginInProgress{};
	std::string loginErrorStr{};
	std::string loginMessageStr;
	bool shouldSearch{};
	bool modelLoading{};
	std::optional<float> searchScrollPos{};
	std::atomic<ProgressMode> progressMode;
	std::atomic<float> progress{};

	explicit ViewOnlineImport(const std::shared_ptr<IWidgetContext> &pContext);

public:
	static std::shared_ptr<ViewOnlineImport> create(const std::shared_ptr<IWidgetContext> &pContext);

	void showLoginDialog() override { shouldLoginDialog = true; }

	void hideLoginDialog() override { shouldLoginDialog = false; }

	void loginError(const std::string &pError, const std::string &pMessage) override;

	void setLoginInProgress() override;

	void showModelLoading() override { modelLoading = true; }

	void hideModelLoading() override { modelLoading = false; }

	void setProgressMode(ProgressMode pProgressMode) override { progressMode = pProgressMode; }

	std::atomic<float> &getProgressAtomic() override { return progress; }

	void onUpdate(bool pVisible) override;

	//void customRender() override;

	void openView() override;

	void closeView() override;

	[[nodiscard]] IPresenterOnlineImport* getPresenter() const override { return presenter; }

	void setPresenter(IPresenterOnlineImport* pPresenter) override { presenter = pPresenter; }

private:
	void renderLoginDialog();

	void renderSearchDialog();

	void renderResult(const std::shared_ptr<ModelSearchList> &pResult, const glm::vec2 &pImageSize);

	void renderImage(ImDrawList* pDl, const std::shared_ptr<ModelSearchList> &pResult, const glm::vec2 &pImageSize);
};

class OnlineImportWorkspace : public Editor {
	std::shared_ptr<IPresenterOnlineImport> searchPresenter;
	std::shared_ptr<IPresenterScenePreview> scenePresenter;

public:
	OnlineImportWorkspace();

	~OnlineImportWorkspace() override;

	const char* getType() const override { return "OnlineImportWorkspace"; }

	void loadPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize, ImGuiDir pPanelDir) override;
};
} // namespace mer::editor::mvp

#endif //VIEWONLINEIMPORT_H
