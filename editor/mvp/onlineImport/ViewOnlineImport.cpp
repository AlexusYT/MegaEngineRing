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

#include "ViewOnlineImport.h"

#include <nlohmann/json.hpp>
#include <thread>
#include <unordered_map>

#include "ModelOnlineImport.h"
#include "PresenterOnlineImport.h"
#include "imgui_internal.h"
#include "mvp/contexts/UiWindowContext.h"
#include "mvp/scenePreview/ModelScenePreview.h"
#include "mvp/scenePreview/ViewScenePreview.h"
#include "mvp/scenePreview/onlineImport/PresenterOnlineImportPreview.h"
#include "sketchfab/SketchfabSearch.h"

namespace ked {
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

void ViewOnlineImport::openView() { context->add(this); }

void ViewOnlineImport::closeView() { context->remove(); }

void ViewOnlineImport::renderLoginDialog() {
	static std::string username{};
	static std::string password{};
	static std::string token{};

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	static glm::vec2 windowSize{350, 204};
	[[maybe_unused]] glm::vec2 contentSize = ImGui::GetContentRegionAvail();
	//ImGui::SetCursorPos((contentSize - windowSize) / 2.0f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, {0.5f, 0.5f});
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
	if (ImGui::BeginChild("Login", {0, 0},
						  ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY)) {
		ImGui::PopStyleVar();
		ImGui::TextWrapped("%s", tr("In order to use this feature, you need to login to Sketchfab"));
		ImGui::TextWrapped("%s ", tr("Website here:"));
		ImGui::SameLine(0, 0);
		ImGui::TextLinkOpenURL("https://sketchfab.com");
		ImGui::Separator();
		//ImGui::Dummy({10, 10});
		ImGui::TextWrapped("%s", tr("Login using:"));
		static int e = 0;
		ImGui::RadioButton(tr("LoginWithUsernamePassword"), &e, 0);
		ImGui::SameLine();
		ImGui::HelpMarker(tr("Use username and password from your Sketchfab.com account"));
		ImGui::BeginDisabled(true);
		ImGui::RadioButton(tr("LoginWithAPIToken"), &e, 1);
		ImGui::EndDisabled();
		ImGui::SetItemTooltip("%s", tr("Not implemented yet"));
		ImGui::SameLine();
		ImGui::HelpMarker(tr("Use API token from your Sketchfab.com account"));
		bool loginDisabled = false;
		if (e == 0) {
			ImGui::InputTextErrored("##Username", tr("Username"), tr("Username cannot be empty"), username.empty(),
									username);
			ImGui::InputTextErrored("##Password", tr("Password"), tr("Password cannot be empty"), password.empty(),
									password, ImGuiInputTextFlags_Password);
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
			ImGui::ProgressBar(-1.0f * static_cast<float>(ImGui::GetTime()), ImVec2(0.0f, 0.0f), tr("In progress..."));
		}

		ImGui::BeginDisabled(loginDisabled);
		if (ImGui::Button(tr("ActionLogin"))) {
			if (e == 0) { if (presenter) presenter->loginImplicit(username, password); } else {
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
			ImGui::SetNextItemWidth(-1);
			if (ImGui::BeginCombo("##sortBy", I18n::trSketchfabSortTypesMap.at(request.sortBy).c_str(), 0)) {
				for (const auto &type: I18n::trSketchfabSortTypes) {
					bool isSelected = request.sortBy == type;
					if (ImGui::Selectable(I18n::trSketchfabSortTypesMap.at(type).c_str(), isSelected)) {
						request.sortBy = type;
						presenter->onSearchRequestChanged();
					}

					if (isSelected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputTextWithHint("##UserInput", tr("Author name"), request.user)) {
				presenter->onSearchRequestChanged();
			}


			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputTextWithHint("##CollectionInput", tr("Collection (uid)"), request.collection)) {
				presenter->onSearchRequestChanged();
			}

			int maxFaces = request.maxFaceCount.has_value() ? request.maxFaceCount.value() : 0;
			int minFaces = request.minFaceCount.has_value() ? request.minFaceCount.value() : 0;

			ImGui::SetNextItemWidth(-1);
			int lightInnerConeMax = std::min(INT_MAX, maxFaces == 0 ? INT_MAX : maxFaces);
			//translators: "Faces" is the polygons (triangles), not human face.
			if (ImGui::DragInt("##MinFaces", &minFaces, 1, 0, lightInnerConeMax, tr("Min Faces: %d"))) {
				if (minFaces > 0) request.minFaceCount = minFaces;
				else
					request.minFaceCount.reset();
				presenter->onSearchRequestChanged();
			}
			ImGui::SetNextItemWidth(-1);
			//translators: "Faces" is the polygons (triangles), not human face.
			if (ImGui::DragInt("##MaxFaces", &maxFaces, 1, std::max(0, minFaces), INT_MAX, tr("Max Faces: %d"))) {
				if (maxFaces > 0) request.maxFaceCount = maxFaces;
				else
					request.maxFaceCount.reset();
				presenter->onSearchRequestChanged();
			}

			if (bool value = request.staffpicked.has_value() && request.staffpicked.value();
				ImGui::Checkbox(tr("Staff Picked"), &value)) {
				request.staffpicked = value;
				presenter->onSearchRequestChanged();
			}
			ImGui::SetNextItemWidth(-1);
			if (ImGui::BeginCombo("##License", I18n::trSketchfabLicensesMap.at(request.license).first.c_str())) {
				for (const auto &type: I18n::trSketchfabLicenses) {
					const bool isSelected = request.license == type;
					const auto &[title, tooltip] = I18n::trSketchfabLicensesMap[type];
					if (ImGui::Selectable(title.c_str(), isSelected)) {
						request.license = type;
						presenter->onSearchRequestChanged();
					}
					ImGui::SetItemTooltip("%s", tooltip.c_str());

					if (isSelected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			if (ImGui::CollapsingHeader(tr("Not recommended to change"))) {
				bool downloadable = request.downloadable.has_value() && request.downloadable.value();
				if (ImGui::Checkbox(tr("Downloadable"), &downloadable)) {
					request.downloadable = downloadable;
					presenter->onSearchRequestChanged();
				}

				if (ImGui::BeginCombo(tr("FilterMaterial"),
									  I18n::trSketchfabMaterialTypesMap.at(request.pbrType).c_str(),
									  ImGuiComboFlags_WidthFitPreview)) {
					for (const auto &type: I18n::trSketchfabMaterialTypes) {
						bool isSelected = request.pbrType == type;
						if (ImGui::Selectable(I18n::trSketchfabMaterialTypesMap.at(type).c_str(), isSelected)) {
							request.pbrType = type;
							presenter->onSearchRequestChanged();
						}

						if (isSelected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

			if (ImGui::CollapsingHeader(tr("Not yet supported by the engine"))) {
				if (bool value = request.animated.has_value() && request.animated.value();
					ImGui::Checkbox(tr("FilterAnimated"), &value)) {
					request.animated = value;
					presenter->onSearchRequestChanged();
				}
				if (bool value = request.staffpicked.has_value() && request.staffpicked.value();
					ImGui::Checkbox(tr("FilterHasSound"), &value)) {
					request.staffpicked = value;
					presenter->onSearchRequestChanged();
				}
				if (bool value = request.rigged.has_value() && request.rigged.value();
					ImGui::Checkbox(tr("FilterRigged"), &value)) {
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
	if (ImGui::InputTextWithHint("##QueryInput", tr("Enter search keywords"), request.query)) {
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
		case ProgressMode::DOWNLOAD_LINKS: progressStr = "Getting download links...";
			break;
		case ProgressMode::DOWNLOAD_MODEL: progressStr = "Downloading model...";
			break;
		case ProgressMode::PARSE_MODEL: progressStr = "Parsing model...";
			break;
	}
	if (mode != ProgressMode::NONE) ImGui::ProgressBar(prog, ImVec2(itemWidth, 0.0f), progressStr.c_str());
	else { presenter->initScene(); }
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
	if (auto thumbnail = pResult->getSmallThumbnail()) { thumbnailTexId = thumbnail->getImageId(); } else
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
	if (auto avatar = pResult->user->getSmallAvatar()) { texId = avatar->getImageId(); } else
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

	if (ImGui::Button(tr("Search filters"), {pWidth, 0})) toggleSearchFilters();
	if (pTransparent) {
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
	} else
		ImGui::PopStyleColor(2);
}

OnlineImportWorkspace::OnlineImportWorkspace()
	: Editor("OnlineImportWorkspace") {
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
} // namespace ked
