//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 30.09.24.
//

#include "PresenterResourceEditor.h"

#include "EditingResourceList.h"
#include "EngineSDK/main/resources/IResource.h"
#include "EngineSDK/main/resources/LoadedResources.h"
#include "EngineSDK/main/resources/ResourceLoadResult.h"
#include "EngineSDK/main/resources/materials/ColorComponent.h"
#include "EngineSDK/main/resources/materials/IMaterialResource.h"
#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "EngineSDK/main/resources/models/IModel3DResource.h"
#include "EngineSDK/main/resources/textures/ITextureResource.h"
#include "IModelResourceEditor.h"
#include "IViewResourceEditor.h"
#include "mvp/ApplicationController.h"
#include "readers/ObjFileResourceReader.h"
#include "readers/PngFileResourceReader.h"
#include "savers/MaterialResourceSaver.h"
#include "savers/Model3DResourceSaver.h"
#include "savers/TextureResourceSaver.h"
#include "ui/customWidgets/resourceSelector/SourceSelectionColor.h"
#include "ui/customWidgets/resourceSelector/resources/SourceSelectionTexture.h"

namespace mer::editor::mvp {
PresenterResourceEditor::PresenterResourceEditor(const std::shared_ptr<IModelResourceEditor> &pModel) : model(pModel) {

	model->setPresenter(this);
	model->getEditingResources()->connectOnResourceAddedSignal(
		[this](const std::shared_ptr<sdk::main::IResource> &pResource) {
			resourcesInfo.emplace(pResource.get(), ResourceInfo());
			for (auto &[view, info]: viewsInfo) {

				view->appendResource(pResource);
				view->selectResource(pResource);
			}
		});
	model->getEditingResources()->connectOnResourceRemovedSignal(
		[this](const std::shared_ptr<sdk::main::IResource> &pResource) {
			resourcesInfo.erase(pResource.get());
			for (auto &[view, info]: viewsInfo) { view->removeResource(pResource); }
		});
}

void PresenterResourceEditor::chooseFileClicked(IViewResourceEditor* pView) {
	const std::shared_ptr<Gtk::FileDialog> dialog = Gtk::FileDialog::create();
	dialog->set_modal(false);

	pView->showFileDialog(dialog, [dialog, this, pView](std::shared_ptr<Gio::AsyncResult> &pResult) {
		try {
			const auto file = dialog->open_finish(pResult);
			auto path = std::filesystem::path(file->get_path());
			onPathToFileChanged(pView, path);
		} catch (const Gtk::DialogError &err) {
			if (err.code() == Gtk::DialogError::FAILED) {
				const auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to show file dialog");
				msg->setMessage(err.what());
				pView->displayError(msg);
			}
		} catch (const Glib::Error &err) {
			const auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to show file dialog");
			msg->setMessage(err.what());
			pView->displayError(msg);
		}
	});
}

void PresenterResourceEditor::onResourceNameChanged(const std::string & /*pNewName*/) {
	/*if (model->getResourceName() == pNewName) return;
	model->setResourceName(pNewName);
	auto path = model->getPathToResource() / (pNewName + ".enmodel");
	if (exists(path)) {
		view->displayMessage("Resource with the such name is already exists");
		view->setSaveButtonSensitivity(false);
		return;
	}
	view->displayMessage("");
	view->setSaveButtonSensitivity(true);*/
}

void PresenterResourceEditor::onPathToFileChanged(IViewResourceEditor* pView, const std::filesystem::path &pPath) {
	auto resource = getSelectedResource(pView);
	if (!resource) return;
	ResourceInfo* info = getResourceInfo(resource.get());
	if (!info) return;
	auto resourceType = resource->getResourceType();
	if (resourceType == sdk::main::ResourceType::MODEL) {

		auto objReader = std::make_shared<ObjFileResourceReader>(pPath);
		if (auto msg = objReader->checkType(); msg) {
			sdk::utils::Logger::error(msg);
			return;
		}
		info->resourceReader = objReader;
		auto objects = objReader->getObjectsName();
		std::vector<std::shared_ptr<sdk::main::IModel3DObject>> objectsList;
		for (auto objectName: objects) { objectsList.emplace_back(objReader->generateObject(objectName)); }

		auto comp = [](auto pA, auto pB) { return *pA < *pB; };
		std::ranges::sort(objectsList, comp);
		std::vector<std::shared_ptr<sdk::main::IModel3DObject>> tmpList;
		std::ranges::set_difference(objectsList, info->resourceObjects, std::back_inserter(tmpList), comp);
		std::ranges::set_difference(tmpList, info->fileObjects, std::back_inserter(info->fileObjects), comp);
		info->srcFilePath = pPath;
		pView->displayObjects(info->fileObjects, true);
	} else if (resourceType == sdk::main::ResourceType::TEXTURE) {

		auto reader = std::make_shared<PngFileResourceReader>(pPath);
		if (auto msg = reader->checkType(); msg) {
			sdk::utils::Logger::error(msg);
			return;
		}
		reader->generateResourceData(std::dynamic_pointer_cast<sdk::main::ITextureResource>(resource));
		saveResource(resource);
	}
	pView->displayChosenPath(pPath.string());
}

void PresenterResourceEditor::savePathClicked(IViewResourceEditor* pView, const std::string &pNewPath,
											  const std::string &pNewName) {
	const auto resource = getSelectedResource(pView);
	std::string extension;
	switch (resource->getResourceType()) {

		case sdk::main::ResourceType::MODEL: extension = ".enmodel"; break;
		case sdk::main::ResourceType::TEXTURE: extension = ".entex"; break;
		case sdk::main::ResourceType::MATERIAL: extension = ".enmat"; break;
		case sdk::main::ResourceType::SHADER:
		case sdk::main::ResourceType::NONE: return;
	}

	auto res = model->getEditingResources()->getContext()->getResources();
	const auto newPath = std::filesystem::path(pNewPath.starts_with("/") ? pNewPath.substr(1) : pNewPath);
	const auto dataPath = getDataPath();
	auto oldUri = resource->getResourceUri().string();
	res->removeResource(oldUri);
	resource->setResourceUri(newPath / (pNewName + extension));
	saveResource(resource, false);
	res->addResource(resource->getResourceUri().string(), resource);
	auto oldPath = dataPath / (oldUri.starts_with("/") ? oldUri.substr(1) : oldUri);
	if (is_regular_file(oldPath) && exists(oldPath)) remove(oldPath);
}

void PresenterResourceEditor::onCancelClicked() { getAppController()->stop(this); }

void PresenterResourceEditor::run() {}

void PresenterResourceEditor::stop() {}

void PresenterResourceEditor::addView(const std::shared_ptr<IView> &pNewView) {
	auto view = std::dynamic_pointer_cast<IViewResourceEditor>(pNewView);
	view->setPresenter(this);
	view->openView();
	auto &res = model->getEditingResources()->getResources();
	ViewInfo info;
	viewsInfo.emplace(view.get(), info);
	if (!res.empty()) {
		std::shared_ptr<sdk::main::IResource> lastResource;
		for (const auto &resource: res | std::views::values) {
			view->appendResource(resource);
			lastResource = resource;
		}

		info.selectedResource = lastResource;

		view->selectResource(lastResource);
	}
	views.emplace_back(view);
}

void PresenterResourceEditor::removeView(const std::shared_ptr<IView> &pOldView) {
	const auto view = std::dynamic_pointer_cast<IViewResourceEditor>(pOldView);
	viewsInfo.erase(view.get());
	erase(views, pOldView);
	view->closeView();
}

std::shared_ptr<sdk::main::IResource> PresenterResourceEditor::getSelectedResource(IViewResourceEditor* pView) {
	const auto iter = viewsInfo.find(pView);
	if (iter == viewsInfo.end()) { return nullptr; }
	return iter->second.selectedResource;
}

std::filesystem::path PresenterResourceEditor::getDataPath() { return model->getPathToDataDir(); }

void PresenterResourceEditor::onSelectionChanged(const std::shared_ptr<sdk::main::IResource> &pResource,
												 IViewResourceEditor* pView) {
	auto info = getViewInfo(pView);
	if (!info) return;
	info->selectedResource = pResource;
	auto resInfo = getResourceInfo(pResource.get());
	if (!resInfo) return;
	if (auto modelResource = std::dynamic_pointer_cast<sdk::main::IModel3DResource>(pResource)) {
		pView->switchTo("model");
		auto modelObj = modelResource->getModelObjects() | std::views::values;
		resInfo->resourceObjects = std::vector(modelObj.begin(), modelObj.end());
		//for (auto object: modelResource->getModelObjects() | std::views::values) { resInfo->resourceObjects.push_back(object); }
		//std::ranges::sort(resInfo->resourceObjects);
		std::ranges::sort(resInfo->resourceObjects, [](auto pA, auto pB) { return *pA < *pB; });
		pView->displayObjects(resInfo->resourceObjects, false);
		pView->displayObjects(resInfo->fileObjects, true);
		pView->displayChosenPath(resInfo->srcFilePath.string());
	} else if (auto textureResource = std::dynamic_pointer_cast<sdk::main::ITextureResource>(pResource)) {
		pView->switchTo("texture");
		pView->displayChosenPath(resInfo->srcFilePath.string());
	} else if (auto materialResource = std::dynamic_pointer_cast<sdk::main::IMaterialResource>(pResource)) {
		pView->switchTo("material");
		pView->displayChosenPath(resInfo->srcFilePath.string());
		pView->displayMaterial(materialResource);
	}
}

void PresenterResourceEditor::removeObject(const std::shared_ptr<sdk::main::IModel3DObject> &pObjectToRemove,
										   IViewResourceEditor* pView) {
	auto resource = getSelectedResource(pView);
	if (!resource) return;
	auto modelResource = std::dynamic_pointer_cast<sdk::main::IModel3DResource>(resource);
	if (!modelResource) return;
	auto info = getResourceInfo(resource.get());
	if (!info) return;
	modelResource->removeModelObject(pObjectToRemove);
	erase(info->resourceObjects, pObjectToRemove);
	info->fileObjects.emplace_back(pObjectToRemove);
	std::ranges::sort(info->fileObjects, [](auto pA, auto pB) { return *pA < *pB; });
	pView->displayObjects(info->resourceObjects, false);
	pView->displayObjects(info->fileObjects, true);
	saveResource(resource);
}

void PresenterResourceEditor::addObject(const std::shared_ptr<sdk::main::IModel3DObject> &pObjectToAdd,
										IViewResourceEditor* pView) {

	auto resource = getSelectedResource(pView);
	if (!resource) return;
	auto modelResource = std::dynamic_pointer_cast<sdk::main::IModel3DResource>(resource);
	if (!modelResource) return;
	auto info = getResourceInfo(resource.get());
	if (!info) return;
	modelResource->addModelObject(pObjectToAdd);
	erase(info->fileObjects, pObjectToAdd);
	info->resourceObjects.emplace_back(pObjectToAdd);
	std::ranges::sort(info->resourceObjects, [](auto pA, auto pB) { return *pA < *pB; });
	pView->displayObjects(info->resourceObjects, false);
	pView->displayObjects(info->fileObjects, true);
	saveResource(resource);
}

sdk::main::IResourceLoadExecutor* PresenterResourceEditor::getResourceLoader() {

	return model->getEditingResources()->getContext().get();
}

void PresenterResourceEditor::onMaterialBaseColorChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
														 IViewResourceEditor* pView) {
	getComponentFromResult(pResult, [this, pView](const std::shared_ptr<sdk::main::IMaterialComponent> &pComponent) {
		const auto resource = getSelectedResource(pView);
		const auto material = std::dynamic_pointer_cast<sdk::main::IMaterialResource>(resource);
		if (!material) return;
		material->setAlbedo(pComponent);
		saveResource(resource);
	});
}

void PresenterResourceEditor::onMaterialMetallicChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
														IViewResourceEditor* pView) {
	getComponentFromResult(pResult, [this, pView](const std::shared_ptr<sdk::main::IMaterialComponent> &pComponent) {
		const auto resource = getSelectedResource(pView);
		const auto material = std::dynamic_pointer_cast<sdk::main::IMaterialResource>(resource);
		if (!material) return;
		material->setMetallic(pComponent);
		saveResource(resource);
	});
}

void PresenterResourceEditor::onMaterialNormalChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
													  IViewResourceEditor* pView) {
	getComponentFromResult(pResult, [this, pView](const std::shared_ptr<sdk::main::IMaterialComponent> &pComponent) {
		const auto resource = getSelectedResource(pView);
		const auto material = std::dynamic_pointer_cast<sdk::main::IMaterialResource>(resource);
		if (!material) return;
		material->setNormal(pComponent);
		saveResource(resource);
	});
}

void PresenterResourceEditor::onMaterialRoughnessChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
														 IViewResourceEditor* pView) {
	getComponentFromResult(pResult, [this, pView](const std::shared_ptr<sdk::main::IMaterialComponent> &pComponent) {
		const auto resource = getSelectedResource(pView);
		const auto material = std::dynamic_pointer_cast<sdk::main::IMaterialResource>(resource);
		if (!material) return;
		material->setRoughness(pComponent);
		saveResource(resource);
	});
}

void PresenterResourceEditor::onMaterialAOChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
												  IViewResourceEditor* pView) {
	getComponentFromResult(pResult, [this, pView](const std::shared_ptr<sdk::main::IMaterialComponent> &pComponent) {
		const auto resource = getSelectedResource(pView);
		const auto material = std::dynamic_pointer_cast<sdk::main::IMaterialResource>(resource);
		if (!material) return;
		material->setAo(pComponent);
		saveResource(resource);
	});
}

PresenterResourceEditor::ViewInfo* PresenterResourceEditor::getViewInfo(IViewResourceEditor* pView) {

	const auto iter = viewsInfo.find(pView);
	if (iter == viewsInfo.end()) { return nullptr; }
	return &iter->second;
}

PresenterResourceEditor::ResourceInfo* PresenterResourceEditor::getResourceInfo(sdk::main::IResource* pResource) {
	const auto iter = resourcesInfo.find(pResource);
	if (iter == resourcesInfo.end()) { return nullptr; }
	return &iter->second;
}

sdk::utils::ReportMessagePtr PresenterResourceEditor::saveResource(
	const std::shared_ptr<sdk::main::IResource> &pResource, bool pIsComplete) {
	auto uri = pResource->getResourceUri();
	auto savePath = getDataPath() / uri;
	sdk::utils::ReportMessagePtr error{};
	if (auto res = std::dynamic_pointer_cast<sdk::main::IModel3DResource>(pResource)) {
		error = Model3DResourceSaver::saveToFile(savePath, res);
	}
	if (auto res = std::dynamic_pointer_cast<sdk::main::ITextureResource>(pResource)) {
		error = TextureResourceSaver::saveToFile(savePath, res);
	}
	if (auto res = std::dynamic_pointer_cast<sdk::main::IMaterialResource>(pResource)) {
		error = MaterialResourceSaver::saveToFile(savePath, res);
	}
	if (error) return error;

	if (pIsComplete && pResource->isIncomplete()) {

		auto res = model->getEditingResources()->getContext()->getResources();
		res->markResourceComplete(pResource->getResourceUri().string());
	}
	return nullptr;
}

void PresenterResourceEditor::getComponentFromResult(
	const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
	const sigc::slot<void(const std::shared_ptr<sdk::main::IMaterialComponent> &pComponent)> &pSlot) const {
	if (const auto result = std::dynamic_pointer_cast<ui::SourceSelectionTexture::Result>(pResult)) {
		auto texture = result->getSelectionResult();
		auto context = model->getEditingResources()->getContext();
		context->loadResourceAsync(
			texture->asResource()->getResourceUri().string(),
			[pSlot, this](const std::shared_ptr<sdk::main::ResourceLoadResult> &pLoadResult) {
				if (!pLoadResult->isReady()) return;
				pSlot(std::dynamic_pointer_cast<sdk::main::IMaterialComponent>(pLoadResult->getResource()));
			});
		return;
	}
	if (const auto colorResult = std::dynamic_pointer_cast<ui::SourceSelectionColor::Result>(pResult)) {
		pSlot(colorResult->getSelectionResult());
		return;
	}
	pSlot(nullptr);
}
} // namespace mer::editor::mvp