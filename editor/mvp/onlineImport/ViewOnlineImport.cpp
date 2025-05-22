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

#include <nlohmann/json.hpp>
#include <thread>
#include <unordered_map>

#include "ModelOnlineImport.h"
#include "PresenterOnlineImport.h"
#include "imgui_internal.h"
#include "mvp/contexts/IWidgetContext.h"
#include "mvp/contexts/UiWindowContext.h"
#include "mvp/scenePreview/ModelScenePreview.h"
#include "mvp/scenePreview/ViewScenePreview.h"
#include "mvp/scenePreview/onlineImport/PresenterOnlineImportPreview.h"
#include "sketchfab/SketchfabSearch.h"

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

	// ReSharper disable once CppDFAUnreachableCode
	if (shouldLoginDialog) {
		renderLoginDialog();
		return;
	}
	renderSearchDialog();
	renderResults();
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
	ImVec2 contentStart;
	float filtersWidth = 250;
	ImGui::Spacing();
	ImGui::Indent(6);
	if (searchFiltersToggled) {
		auto filtersStart = ImGui::GetCursorScreenPos();
		filtersStart.x -= 6;
		filtersStart.y -= 6;
		ImGui::SetCursorScreenPos(filtersStart);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {6, 6});
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12);

		auto visible = ImGui::BeginChild("SearchWindow", {filtersWidth + 12, 0},
										 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
											 ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_Borders);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
		drawSearchFiltersButton(filtersWidth, true);
		ImGui::Separator();
		if (visible) {

			std::unordered_map<std::string, std::string> sortTypes = {
				{"likeCount", "Least liked"},		 {"-likeCount", "Most liked"},
				{"viewCount", "Least viewed"},		 {"-viewCount", "Most viewed"},
				{"publishedAt", "Latest published"}, {"-publishedAt", "Oldest published"},
				{"processedAt", "Latest processed"}, {"-processedAt", "Oldest processed"}};

			ImGui::SetNextItemWidth(-1);

			if (ImGui::BeginCombo("##sortBy",
								  request.sortBy.empty() ? "Relevancy" : sortTypes.at(request.sortBy).c_str(), 0)) {
				bool isSelected = request.sortBy == "";
				if (ImGui::Selectable("Relevancy##None", isSelected)) {
					request.sortBy = "";
					presenter->onSearchRequestChanged();
				}
				for (const auto &[type, matName]: sortTypes) {
					isSelected = request.sortBy == type;
					if (ImGui::Selectable(matName.c_str(), isSelected)) {
						request.sortBy = type;
						presenter->onSearchRequestChanged();
					}

					if (isSelected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputTextWithHint("##UserInput", "Models from user...", request.user)) {
				presenter->onSearchRequestChanged();
			}


			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputTextWithHint("##CollectionInput", "Collection (uid)...", request.collection)) {
				presenter->onSearchRequestChanged();
			}

			int maxFaces = request.maxFaceCount.has_value() ? request.maxFaceCount.value() : 0;
			int minFaces = request.minFaceCount.has_value() ? request.minFaceCount.value() : 0;

			ImGui::SetNextItemWidth(-1);
			int lightInnerConeMax = std::min(INT_MAX, maxFaces == 0 ? INT_MAX : maxFaces);
			if (ImGui::DragInt("##MinFaces", &minFaces, 1, 0, lightInnerConeMax, "Min Faces: %d")) {
				if (minFaces > 0) request.minFaceCount = minFaces;
				else
					request.minFaceCount.reset();
				presenter->onSearchRequestChanged();
			}
			ImGui::SetNextItemWidth(-1);
			if (ImGui::DragInt("##MaxFaces", &maxFaces, 1, std::max(0, minFaces), INT_MAX, "Max Faces: %d")) {
				if (maxFaces > 0) request.maxFaceCount = maxFaces;
				else
					request.maxFaceCount.reset();
				presenter->onSearchRequestChanged();
			}

			if (bool value = request.staffpicked.has_value() && request.staffpicked.value();
				ImGui::Checkbox("Staff Picked", &value)) {
				request.staffpicked = value;
				presenter->onSearchRequestChanged();
			}
			//by,by-sa, by-nd, by-nc, by-nc-sa, by-nc-nd, cc0, ed, st
			std::unordered_map<std::string /*type*/, std::pair<std::string /*name*/, std::string /*tooltip*/>> license =
				{{"by", {"Creative Commons: Attribution", "Author must be credited. Commercial use is allowed."}},
				 {"by-sa",
				  {"Creative Commons: ShareAlike", "Author must be credited. Modified versions must have the same "
												   "license. Commercial use is allowed."}},
				 {"by-nd",
				  {"Creative Commons: NoDerivatives",
				   "Author must be credited. Modified versions can not be distributed. Commercial use is allowed."}},
				 {"by-nc", {"Creative Commons: NonCommercial", "Author must be credited. No commercial use."}},
				 {"by-nc-sa",
				  {"Creative Commons: NonCommercial-ShareAlike",
				   "Author must be credited. No commercial use. Modified versions must have the same license."}},
				 {"by-nc-nd",
				  {"Creative Commons: NonCommercial-NoDerivatives",
				   "Author must be credited. No commercial use. Modified versions can not be distributed."}},
				 {"cc0", {"Creative Commons: Zero", "Credit is not mandatory. Commercial use is allowed."}},
				 {"ed", {"Editorial", "Use only in connection with events that are newsworthy or of public interest"}},
				 {"st",
				  {"Standard",
				   "Under basic restrictions, use worldwide, on all types of media, commercially or not, and in "
				   "all types of derivative works"}}};
			ImGui::SetNextItemWidth(-1);
			if (ImGui::BeginCombo("##License", request.license.empty() ? "Any License"
																	   : license.at(request.license).first.c_str())) {
				bool isSelected = request.license == "";
				if (ImGui::Selectable("Any License##None", isSelected)) {
					request.license = "";
					presenter->onSearchRequestChanged();
				}

				if (isSelected) ImGui::SetItemDefaultFocus();
				for (const auto &[type, data]: license) {
					isSelected = request.license == type;
					if (ImGui::Selectable(data.first.c_str(), isSelected)) {
						request.license = type;
						presenter->onSearchRequestChanged();
					}
					ImGui::SetItemTooltip("%s", data.second.c_str());

					if (isSelected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			if (ImGui::CollapsingHeader("Not recommended to change")) {
				bool downloadable = request.downloadable.has_value() && request.downloadable.value();
				if (ImGui::Checkbox("Downloadable", &downloadable)) {
					request.downloadable = downloadable;
					presenter->onSearchRequestChanged();
				}

				std::unordered_map<std::string, std::string> types = {{"false", "Simple"},
																	  {"true", "Physically based only"},
																	  {"metalness", "Metalness/Roughness"},
																	  {"specular", "Specular/Glossiness"}};

				if (ImGui::BeginCombo("Material", types.at(request.pbrType).c_str(), ImGuiComboFlags_WidthFitPreview)) {
					for (const auto &[type, matName]: types) {
						bool isSelected = request.pbrType == type;
						if (ImGui::Selectable(matName.c_str(), isSelected)) {
							request.pbrType = type;
							presenter->onSearchRequestChanged();
						}

						if (isSelected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

			if (ImGui::CollapsingHeader("Not supported by engine")) {
				if (bool value = request.animated.has_value() && request.animated.value();
					ImGui::Checkbox("Animated", &value)) {
					request.animated = value;
					presenter->onSearchRequestChanged();
				}
				if (bool value = request.staffpicked.has_value() && request.staffpicked.value();
					ImGui::Checkbox("Has sound", &value)) {
					request.staffpicked = value;
					presenter->onSearchRequestChanged();
				}
				if (bool value = request.rigged.has_value() && request.rigged.value();
					ImGui::Checkbox("Rigged", &value)) {
					request.rigged = value;
					presenter->onSearchRequestChanged();
				}
			}
		}
		ImGui::EndChild();
		filtersStart.x += 6;
		filtersStart.y += 6;
		ImGui::SetCursorScreenPos(filtersStart);
		ImGui::Dummy({filtersWidth, 0});
		ImGui::SameLine(filtersWidth + 30);
		contentStart = ImGui::GetCursorScreenPos();
	} else {
		contentStart = ImGui::GetCursorScreenPos();
		drawSearchFiltersButton(0, false);
		ImGui::SameLine();
	}
	ImGui::SetNextItemWidth(350);
	if (ImGui::InputTextWithHint("##QueryInput", "Enter search keywords", request.query)) {
		presenter->onSearchRequestChanged();
	}
	contentStart.y += ImGui::GetItemRectSize().y + ImGui::GetStyle().ItemSpacing.y;
	ImGui::SetCursorScreenPos(contentStart);
}

void ViewOnlineImport::renderResults() {
	static std::shared_ptr<ModelSearchList> tmp;
	ImVec2 lastPos{};
	ImGui::BeginChild("SearchResults", ImVec2(0, 0));
	float itemWidth = 330;
	auto count = results.size();

	for (size_t i = 0; i < count; i++) {
		auto result = results.at(i);

		std::string id1 = std::to_string(i);
		ImGui::SetNextItemWidth(itemWidth);
		if (renderResult(result->name + "##" + id1, result) && !modelLoading) {
			searchScrollPos = ImGui::GetScrollY();
			presenter->selectModel(result);
			tmp = result;
		}
		if (tmp == result) lastPos = ImGui::GetItemRectMin();
		ImGui::SameLine();
		if (itemWidth > ImGui::GetContentRegionAvail().x) ImGui::NewLine();
	}
	if (!modelLoading) tmp.reset();
	ImGui::SetCursorScreenPos(lastPos);
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
	if (mode != ProgressMode::NONE) ImGui::ProgressBar(prog, ImVec2(itemWidth, 0.0f), progressStr.c_str());

	auto maxScroll = ImGui::GetScrollMaxY();
	if (maxScroll > 100 && ImGui::GetScrollY() + 100.0f >= maxScroll) {

		if (!presenter->isSearching() && !resultsInvalidated) presenter->nextSearchResult();
	}
	resultsInvalidated = false;
	ImGui::EndChild();
}

bool ViewOnlineImport::renderResult(const std::string &pId, const std::shared_ptr<ModelSearchList> &pResult) {
	constexpr float aspectRatio = 9.0f / 16.0f;
	constexpr float rounding = 8;
	auto screenCursor = ImGui::GetCursorScreenPos();
	glm::vec2 thumbnailImageStart = screenCursor;
	auto itemWidth = ImGui::CalcItemWidth();
	ImVec2 thumbnailImageEnd{thumbnailImageStart.x + itemWidth, thumbnailImageStart.y + aspectRatio * itemWidth};
	const float itemHeight = ImGui::GetTextLineHeightWithSpacing();
	glm::vec2 bgStart(thumbnailImageStart.x, thumbnailImageEnd.y - itemHeight - 3);

	glm::vec2 avatarImageStart{thumbnailImageStart.x, thumbnailImageEnd.y + 5};
	glm::vec2 avatarImageEnd = avatarImageStart + glm::vec2(32);

	ImRect bb(thumbnailImageStart, {thumbnailImageEnd.x, avatarImageEnd.y});

	auto id = ImGui::GetID(pId.c_str());
	ImGui::ItemSize(bb, ImGui::GetStyle().FramePadding.y);
	if (!ImGui::ItemAdd(bb, id)) return false;

	bool hovered{};
	bool held{};
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
	uint32_t thumbnailTexId;
	if (auto thumbnail = pResult->getSmallThumbnail()) {
		thumbnailTexId = thumbnail->getImageId();
	} else
		thumbnailTexId = 0;
	ImDrawList* dl = ImGui::GetWindowDrawList();

	ImU32 thumbnailColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
	if (hovered) thumbnailColor = ImGui::GetColorU32(ImGuiCol_FrameBgHovered);
	if (held) thumbnailColor = ImGui::GetColorU32(ImGuiCol_FrameBgActive);
	dl->AddRectFilled(bb.Min, bb.Max, thumbnailColor, rounding);

	dl->AddImageRounded(thumbnailTexId, thumbnailImageStart, thumbnailImageEnd, ImVec2(0, 0), ImVec2(1, 1),
						IM_COL32_WHITE, rounding);


	dl->AddRectFilled(bgStart, thumbnailImageEnd, ImColor(0.0f, 0.0f, 0.0f, 0.5f), rounding);

	dl->AddText(
		{
			bgStart.x + 5,
			bgStart.y + 3,
		},
		ImGui::GetColorU32(ImGuiCol_Text), pResult->name.c_str(), nullptr);


	uint32_t texId;
	if (auto avatar = pResult->user->getSmallAvatar()) {
		texId = avatar->getImageId();
	} else
		texId = 0;
	dl->AddImageRounded(texId, avatarImageStart, avatarImageEnd, ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE, rounding);


	dl->AddText(
		{
			avatarImageEnd.x + 10,
			avatarImageStart.y + 8,
		},
		ImGui::GetColorU32(ImGuiCol_Text), pResult->user->displayName.c_str(), nullptr);
	return pressed;
}

void ViewOnlineImport::drawSearchFiltersButton(float pWidth, bool pTransparent) {

	if (pTransparent) {
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	}
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.18f, 0.18f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

	if (ImGui::Button("Search filters", {pWidth, 0})) toggleSearchFilters();
	if (pTransparent) {
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
	} else
		ImGui::PopStyleColor(2);
}

OnlineImportWorkspace::OnlineImportWorkspace() : Editor("OnlineImportWorkspace") {
	selection = std::make_shared<NodeSelectionHelper>();
	auto previewView = ViewScenePreview::create("PrefabScenePreview", EditorContext::create(this));
	auto previewModel = std::make_shared<ModelScenePreview>(selection.get());
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