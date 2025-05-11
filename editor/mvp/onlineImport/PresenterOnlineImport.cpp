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

#include "PresenterOnlineImport.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <thread>

#include "EngineSDK/gltf/GltfModel.h"
#include "EngineSDK/gltf/Node.h"
#include "EngineSDK/render/Renderer.h"
#include "EngineSDK/scene/Scene3D.h"
#include "Globals.h"
#include "ModelOnlineImport.h"
#include "Utils.h"
#include "ViewOnlineImport.h"
#include "mvp/scenePreview/ViewScenePreview.h"
#include "mvp/scenePreview/prefab/ModelPrefabPreview.h"
#include "sketchfab/SketchfabAccount.h"
#include "sketchfab/SketchfabSearch.h"

#define SKETCHFAB_SITE "https://sketchfab.com"
#define SKETCHFAB_OAUTH SKETCHFAB_SITE "/oauth2/token/"
#define CLIENT_ID "hGC7unF4BHyEB0s7Orz5E1mBd3LluEG0ILBiZvF9"

namespace Microsoft::glTF {
class GLTFResourceReader;
}

namespace mer::editor::mvp {
PresenterOnlineImport::PresenterOnlineImport(const std::shared_ptr<IModelOnlineImport> &pModel,
											 const std::shared_ptr<IViewOnlineImport> &pView)
	: model(pModel), view(pView) {

	view->setPresenter(this);
	model->setPresenter(this);
	sdk::ReportMessagePtr message;
	auto path = Globals::getConfigPath() / "sketchfab-account.json";
	if (exists(path)) {
		sdk::Logger::info("Loading Sketchfab account");
		if (auto account = SketchfabAccount::createFromFile(path, message)) {
			model->setAccount(account);
		} else {
			sdk::Logger::error(message);
			sdk::Logger::info("Re-login to Sketchfab account required");
			view->loginError("invalid_save", "Re-login required");
			view->showLoginDialog();
		}
	} else {
		sdk::Logger::info("No Sketchfab account found. Login required");
		view->showLoginDialog();
	}
}

std::shared_ptr<PresenterOnlineImport> PresenterOnlineImport::create(const std::shared_ptr<IModelOnlineImport> &pModel,
																	 const std::shared_ptr<IViewOnlineImport> &pView) {
	return std::shared_ptr<PresenterOnlineImport>(new PresenterOnlineImport(pModel, pView));
}

void PresenterOnlineImport::loginImplicit(const std::string &pUsername, const std::string &pPassword) {

	std::jthread thread([this, pPassword, pUsername]() {
		auto str =
			std::format("grant_type=password&client_id={}&username={}&password={}", CLIENT_ID, pUsername, pPassword);
		try {
			std::unique_ptr<CURL, void (*)(CURL*)> request(curl_easy_init(), curl_easy_cleanup);
			view->setLoginInProgress();
			curl_easy_setopt(request.get(), CURLOPT_URL, SKETCHFAB_OAUTH);

			std::list<std::string> header = {"Content-Type: application/x-www-form-urlencoded"};
			auto curlList = Utils::getCurlList(header);
			curl_easy_setopt(request.get(), CURLOPT_HTTPHEADER, curlList.get());
			curl_easy_setopt(request.get(), CURLOPT_POSTFIELDSIZE, str.size());
			curl_easy_setopt(request.get(), CURLOPT_POSTFIELDS, str.c_str());
			std::stringstream ss;
			curl_easy_setopt(request.get(), CURLOPT_WRITEFUNCTION, Utils::streamWriteCallback);
			curl_easy_setopt(request.get(), CURLOPT_WRITEDATA, &ss);

			curl_easy_perform(request.get());
			request.reset();
			curlList.reset();

			//std::cout << "Response code: " << curlpp::infos::ResponseCode::get(request) << std::endl;
			nlohmann::json j = nlohmann::json::parse(ss.str());
			if (auto it = j.find("error"); it != j.end()) {
				auto it1 = j.find("error_description");
				view->loginError(it.value().get<std::string>(), std::string(it1 != j.end() ? it1.value() : ""));
				return;
			}
			if (j.contains("access_token")) {
				auto account = SketchfabAccount::create();
				account->deserialize(j);
				if (auto msg = account->saveToFile(Globals::getConfigPath() / "sketchfab-account.json")) {
					sdk::Logger::error(msg);
				} else {
					sdk::Logger::info("Sketchfab account saved");
				}
				model->setAccount(account);
				view->hideLoginDialog();


				return;
			}
			view->loginError("invalid_response", "Internal error");
		}

		catch (...) {
			view->loginError("runtime_error", "");
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Login failed");
			msg->setMessage("Exception occurred during login");
			msg->addInfoLine("Address: {}", SKETCHFAB_OAUTH);
			auto pass = "password=" + pPassword;
			str = str.replace(str.find(pass), pass.size(), "password=<REDACTED>");
			auto user = "username=" + pUsername;
			str = str.replace(str.find(user), user.size(), "username=<REDACTED>");
			msg->addInfoLine("Data: {}", str);
			sdk::Logger::error(msg);
			return;
		}
		view->loginError("unknown_error", "Unknown error");
	});
	thread.detach();
}

void PresenterOnlineImport::loginToken(const std::string & /*pToken*/) { view->hideLoginDialog(); }

void PresenterOnlineImport::onAccountSet() {}

const std::vector<std::shared_ptr<ModelSearchList>> &PresenterOnlineImport::getSearchResult() const {
	return model->getSearchResult();
}

std::future<std::shared_ptr<sdk::ReportMessage>> PresenterOnlineImport::nextSearchResult() const {
	return model->nextSearchResult();
}

void PresenterOnlineImport::selectModel(const std::shared_ptr<ModelSearchList> &pModel) {
	model->setSelectedModel(pModel);
}

void PresenterOnlineImport::onSelectedModelChanged() {
	auto selectedModel = model->getSelectedModel();
	if (selectedModel) {
		view->showModelLoading();
		modelPreview->setScene(nullptr);
		/*if (auto scene = modelPreview->getScene()) {
			auto pass = scene->getRenderer();
			pass->removeAllMaterials();
			//pass->removeAllMeshInstances();
		}*/
		loadingThread = std::jthread([this, selectedModel](const std::stop_token &pToken) {
			using namespace std::chrono_literals;
			std::shared_ptr<std::iostream> stream;
			bool forceDownload = false;
			while (!pToken.stop_requested()) {
				if (!model->isModelInCache() || forceDownload) {
					forceDownload = false;
					if (!hasDownloadLinks()) {
						view->setProgressMode(IViewOnlineImport::ProgressMode::DOWNLOAD_LINKS);
						if (auto msg = selectedModel->downloadLinks(view->getProgressAtomic())) {
							sdk::Logger::error(msg);
							if (pToken.stop_requested()) return;
							std::this_thread::sleep_for(1s);
							continue;
						}
					}
					if (pToken.stop_requested()) return;
					auto link = getGlbLink();
					if (!link) continue;
					if (!link->getStream()) {
						view->setProgressMode(IViewOnlineImport::ProgressMode::DOWNLOAD_MODEL);
						if (auto msg = link->download(view->getProgressAtomic())) {
							sdk::Logger::error(msg);
							if (pToken.stop_requested()) return;
							std::this_thread::sleep_for(1s);
							continue;
						}
						stream = link->getStream();
						link->resetStream();
						break;
					}

				} else {
					if (auto msg = model->loadModelFromCache(stream)) {
						sdk::Logger::error(msg);
						forceDownload = true;
					} else
						break;
				}
			}
			if (pToken.stop_requested()) return;
			if (!stream) {

				view->setProgressMode(IViewOnlineImport::ProgressMode::NONE);
				return;
			}
			view->setProgressMode(IViewOnlineImport::ProgressMode::PARSE_MODEL);
			view->getProgressAtomic().store(-1.0f);
			sdk::ReportMessagePtr errorMsg;
			auto gltfModel = sdk::GltfModel::createFromStream(stream, errorMsg);
			if (!gltfModel) {
				view->setProgressMode(IViewOnlineImport::ProgressMode::NONE);
				view->hideModelLoading();
				errorMsg->setTitle("Model parsing error");
				sdk::Logger::error(errorMsg);
				return;
			}
			auto scene = sdk::Scene3D::create();
			for (auto material: gltfModel->getMaterials()) { scene->addMaterial(material); }
			for (auto light: gltfModel->getLights()) { scene->addLightSource(light); }
			for (auto mesh: gltfModel->getMeshes()) { scene->addMesh(mesh); }

			scene->mergeNodes(gltfModel->getNodes());
			modelPreview->setScene(scene);
			/*for (auto material: gltfModel->getMaterials()) { modelPreview->addMaterial(material); }

			for (auto node: gltfModel->getNodes()) {
				if (auto meshInstance = std::dynamic_pointer_cast<sdk::MeshInstance>(node))
					modelPreview->addNode(nullptr, meshInstance);
			}*/
			view->setProgressMode(IViewOnlineImport::ProgressMode::NONE);
			view->hideModelLoading();
			modelPreview->getPresenter()->setFocus();
		});
		loadingThread->detach();
		/*std::filesystem::path path = "/home/alexus/Downloads/models/CompareMetallic.glb";
		sdk::ReportMessagePtr errorMsg;
		auto gltfModel = sdk::GltfModel::createFromFile(path, errorMsg);
		if (!gltfModel) { sdk::Logger::error(errorMsg); }

		auto scene = gltfModel->getDefaultScene();
		if (auto msg = scene->initialize()) { sdk::Logger::error(msg); }
		modelPreview->setScene(scene);*/
	} else {
		modelPreview->setScene(nullptr);
		loadingThread->request_stop();
		loadingThread.reset();
		view->hideModelLoading();
	}
}

void PresenterOnlineImport::run() {
	view->openView();
	/*auto scenePreview = ViewScenePreview::create("PrefabScenePreview", "Prefab preview",
												 UiWinChildContext::create(view->getSubWindows()));
	modelPreview = std::make_shared<ModelPrefabPreview>();
	auto presenter = PresenterOnlineImportPreview::create(scenePreview, modelPreview);
	getAppController()->run(presenter);*/
}

void PresenterOnlineImport::stop() { view->closeView(); }

std::shared_ptr<DownloadableModel> PresenterOnlineImport::getGlbLink() const {
	auto links = getGltfLinks();
	if (!links) return nullptr;
	return links->glb;
}

std::shared_ptr<DownloadLinks> PresenterOnlineImport::getGltfLinks() const {
	auto &selectedModel = model->getSelectedModel();
	if (!selectedModel) return nullptr;
	return selectedModel->getLinks();
}

} // namespace mer::editor::mvp