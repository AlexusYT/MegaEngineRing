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

#include "ViewOnlineImport.h"

#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <nlohmann/json.hpp>
#include <thread>

#include "ModelOnlineImport.h"
#include "PresenterOnlineImport.h"
#include "imgui_internal.h"
#include "mvp/contexts/IWidgetContext.h"
#include "mvp/contexts/UiWindowContext.h"
#include "mvp/scenePreview/ModelScenePreview.h"
#include "mvp/scenePreview/ViewScenePreview.h"
#include "mvp/scenePreview/onlineImport/PresenterOnlineImportPreview.h"
#include "sketchfab/SketchfabSearch.h"

#define SKETCHFAB_SITE "https://sketchfab.com"
#define SKETCHFAB_OAUTH SKETCHFAB_SITE "/oauth2/token/"
#define CLIENT_ID "hGC7unF4BHyEB0s7Orz5E1mBd3LluEG0ILBiZvF9"

namespace mer::editor::mvp {

void OnlineImportWorkspace::loadPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize, ImGuiDir /*pPanelDir*/) {

	ImGuiID dock_main_id = pDockspaceId;
	ImGui::DockBuilderAddNode(dock_main_id, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dock_main_id, pDockspaceSize);

	for (auto editorTool: getTools())
		ImGui::DockBuilderDockWindow(editorTool->getWindowName(currDockspaceId).c_str(), dock_main_id);
	ImGui::DockBuilderFinish(pDockspaceId);
}

ViewOnlineImport::ViewOnlineImport(const std::shared_ptr<IWidgetContext> &pContext)
	: EditorTool("SearchWindow"), context(pContext) {}

std::shared_ptr<ViewOnlineImport> ViewOnlineImport::create(const std::shared_ptr<IWidgetContext> &pContext) {
	return std::shared_ptr<ViewOnlineImport>(new ViewOnlineImport(pContext));
}

void ViewOnlineImport::loginError(const std::string &pError, const std::string &pMessage) {
	loginErrorStr = pError;
	loginMessageStr = pMessage;
	loginInProgress = false;
}

void ViewOnlineImport::setLoginInProgress() { loginInProgress = true; }

void ViewOnlineImport::onUpdate(bool pVisible) {
	if (!presenter || !pVisible) return;

	if (shouldLoginDialog) {
		renderLoginDialog();
		return;
	}
	//if (shouldSearch) {
	renderSearchDialog();
	//}
}

void ViewOnlineImport::openView() { context->addTool(this); }

void ViewOnlineImport::closeView() { context->removeWidget(); }

void ViewOnlineImport::renderLoginDialog() {
	static std::string username{};
	static std::string password{};
	static std::string token{};

	//ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	static glm::vec2 windowSize{350, 204};
	glm::vec2 contentSize = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPos((contentSize - windowSize) / 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
	if (ImGui::BeginChild("Login", {0, 0},
						  ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY)) {
		ImGui::PopStyleVar();
		ImGui::TextWrapped("In order to use this feature, you need to login to Sketchfab");
		ImGui::TextWrapped("Website here: ");
		ImGui::SameLine(0, 0);
		ImGui::TextLinkOpenURL("https://sketchfab.com");
		ImGui::Separator();
		//ImGui::Dummy({10, 10});
		ImGui::TextWrapped("Login using: ");
		static int e = 0;
		ImGui::RadioButton("Username/Password", &e, 0);
		ImGui::SameLine();
		ImGui::HelpMarker("Use username and password from your Sketchfab.com account");
		ImGui::RadioButton("API Token", &e, 1);
		ImGui::SameLine();
		ImGui::HelpMarker("Use API token from your Sketchfab.com account");
		bool loginDisabled = false;
		if (e == 0) {
			ImGui::InputTextErrored("##Username", "Username", "Username cannot be empty", username.empty(), username);
			ImGui::InputTextErrored("##Password", "Password", "Password cannot be empty", password.empty(), password,
									ImGuiInputTextFlags_Password);
			if (!loginErrorStr.empty()) ImGui::TextUnformatted(loginErrorStr.c_str());
			loginDisabled = username.empty() || password.empty();
		} else {
			ImGui::TextWrapped("You can obtain it ");
			ImGui::SameLine(0, 0);
			ImGui::TextLinkOpenURL("here", "https://sketchfab.com/settings/password");
			ImGui::InputTextErrored("##Token", "Private API Token", "Token cannot be empty", token.empty(), token,
									ImGuiInputTextFlags_Password);
			loginDisabled = token.empty();
		}
		if (loginInProgress) {
			ImGui::ProgressBar(-1.0f * static_cast<float>(ImGui::GetTime()), ImVec2(0.0f, 0.0f), "In progress...");
		}

		ImGui::BeginDisabled(loginDisabled);
		if (ImGui::Button("Login")) {
			if (e == 0) {
				if (presenter) presenter->loginImplicit(username, password);
			} else {
				if (presenter) presenter->loginToken(token);
			}
		}
		ImGui::EndDisabled();
		windowSize = ImGui::GetWindowSize();
	} else {
		ImGui::PopStyleVar();
		username.clear();
		password.clear();
		token.clear();
	}
	ImGui::EndChild();
}

void ViewOnlineImport::renderSearchDialog() {
	glm::vec2 imageSize{330, 200};
	ImVec2 buttonSz(imageSize.x, 250);
	ImGuiStyle &style = ImGui::GetStyle();
	float window_visible_x2 = ImGui::GetContentRegionAvail().x;
	auto results = presenter->getSearchResult();
	auto count = results.size();
	size_t elementsInRow = static_cast<size_t>(window_visible_x2 / (buttonSz.x + style.ItemSpacing.x));
	if (elementsInRow == 0) return;
	if (searchScrollPos.has_value()) {
		ImGui::SetScrollY(searchScrollPos.value());
		searchScrollPos.reset();
	}
	for (size_t i = 0; i < count; i++) {
		auto result = results.at(i);

		std::string id1 = std::to_string(i);
		if (ImGui::BeginChild((id1 + "Window").c_str(), buttonSz)) { renderResult(result, imageSize); }
		ImGui::EndChild();
		if (i % elementsInRow < elementsInRow - 1) ImGui::SameLine();
	}
	using namespace std::chrono_literals;
	static std::optional<std::future<sdk::ReportMessagePtr>> future;
	auto maxScroll = ImGui::GetScrollMaxY();
	if (maxScroll > 100 && ImGui::GetScrollY() + 100.0f >= maxScroll) {

		if (!future.has_value()) {
			future = presenter->nextSearchResult();
		} else if (future.value().wait_for(1ms) == std::future_status::ready) {
			if (auto msg = future->get()) { sdk::Logger::error(msg); }

			future.reset();
		}
	}
	//sdk::Logger::out("{}. {}", ImGui::GetScrollMaxY(), ImGui::GetScrollY());
}

void ViewOnlineImport::renderResult(const std::shared_ptr<ModelSearchList> &pResult, const glm::vec2 &pImageSize) {
	//ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 5);
	ImDrawList* dl = ImGui::GetWindowDrawList();
	renderImage(dl, pResult, pImageSize);
	ImGui::TextUnformatted(pResult->name.c_str());
	auto screenCursor = ImGui::GetCursorScreenPos();
	glm::vec2 imageStart = screenCursor + glm::vec2(0, 5);
	glm::vec2 imageEnd = imageStart + glm::vec2(32);
	uint32_t texId;
	if (auto avatar = pResult->user->getSmallAvatar()) {
		texId = avatar->getImageId();
	} else
		texId = 0;
	dl->AddImageRounded(texId, imageStart, imageEnd, ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE, 16);
	ImGui::SetCursorScreenPos(ImVec2(imageStart.x + 40, imageStart.y + 10));
	ImGui::TextUnformatted(pResult->user->displayName.c_str());
}

void ViewOnlineImport::renderImage(ImDrawList* pDl, const std::shared_ptr<ModelSearchList> &pResult,
								   const glm::vec2 &pImageSize) {
	const float itemHeight = ImGui::GetTextLineHeightWithSpacing();
	uint32_t texId;
	if (auto thumbnail = pResult->getSmallThumbnail()) {
		texId = thumbnail->getImageId();
	} else
		texId = 0;
	auto screenCursor = ImGui::GetCursorScreenPos();
	glm::vec2 imageStart = screenCursor;
	glm::vec2 imageEnd = imageStart + pImageSize;
	pDl->AddImageRounded(texId, imageStart, imageEnd, ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE, 8);

	static std::shared_ptr<ModelSearchList> tmp{};
	if (modelLoading && tmp == pResult) {
		auto prog = progress.load();
		prog = prog >= 0 ? prog : -1.0f * static_cast<float>(ImGui::GetTime());
		std::string progressStr;
		auto mode = progressMode.load();
		switch (mode) {
			case ProgressMode::NONE: break;
			case ProgressMode::DOWNLOAD_LINKS: progressStr = "Getting download links..."; break;
			case ProgressMode::DOWNLOAD_MODEL: progressStr = "Downloading model..."; break;
			case ProgressMode::PARSE_MODEL: progressStr = "Parsing model..."; break;
		}
		if (mode != ProgressMode::NONE) ImGui::ProgressBar(prog, ImVec2(0.0f, 0.0f), progressStr.c_str());
	}
	if (!modelLoading) tmp.reset();
	if (!modelLoading && ImGui::IsMouseHoveringRect(imageStart, imageEnd) && ImGui::IsWindowHovered()) {
		ImGui::Text("Click to view");
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		if (ImGui::IsMouseClicked(0)) {
			searchScrollPos = ImGui::GetScrollY();
			presenter->selectModel(pResult);
			tmp = pResult;
		}
	}

	glm::vec2 bgStart(imageStart.x, imageEnd.y - itemHeight - 3);
	pDl->AddRectFilled(bgStart, imageEnd, ImColor(0.0f, 0.0f, 0.0f, 0.5f), 8);
	screenCursor.y += pImageSize.y - itemHeight;
	screenCursor.x += 5;
	ImGui::SetCursorScreenPos(screenCursor);
}

OnlineImportWorkspace::OnlineImportWorkspace() : Editor("OnlineImportWorkspace") {
	auto previewView = ViewScenePreview::create("PrefabScenePreview", EditorContext::create(this));
	auto previewModel = std::make_shared<ModelScenePreview>();
	scenePresenter = PresenterOnlineImportPreview::create(previewView, previewModel);
	scenePresenter->run();
	auto searchView = ViewOnlineImport::create(EditorContext::create(this));
	auto searchModel = std::make_shared<ModelOnlineImport>();
	searchPresenter = PresenterOnlineImport::create(searchModel, searchView);
	searchPresenter->setPreviewModel(previewModel);
	searchPresenter->run();
}

OnlineImportWorkspace::~OnlineImportWorkspace() {
	scenePresenter->stop();
	searchPresenter->stop();
}
} // namespace mer::editor::mvp