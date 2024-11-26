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
// Created by alexus on 19.11.24.
//

#include "SourceSelectionTexture.h"

#include "EngineSDK/main/resources/IResource.h"
#include "EngineSDK/main/resources/textures/ITextureResource.h"

namespace mer::editor::ui {
std::shared_ptr<SourceSelectionTexture> SourceSelectionTexture::create(sdk::main::IResourceLoadExecutor* pLoader) {
	return std::shared_ptr<SourceSelectionTexture>(new SourceSelectionTexture(pLoader));
}

void SourceSelectionTexture::onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) {

	const auto result = std::dynamic_pointer_cast<Result>(pResult);
	if (!result) return;

	const auto row = findRow(result->getSelectionResult()->asResource());
	if (!row) return;

	scrollToRow(row);
}

bool SourceSelectionTexture::isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	return std::dynamic_pointer_cast<Result>(pResult) != nullptr;
}

std::string SourceSelectionTexture::getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	if (const auto model = std::dynamic_pointer_cast<Result>(pResult)) {
		const auto resource = model->getSelectionResult();
		return resource->asResource()->getResourceName();
	}
	return "";
}

void SourceSelectionTexture::onElementSelected(const std::shared_ptr<SelectionElement> &pElement) {
	if (const auto objectElement = std::dynamic_pointer_cast<ResourceElement>(pElement)) {
		if (const auto material = objectElement->getTypedResource<sdk::main::ITextureResource>())
			resultCallback(Result::create(material));
	}
}

std::shared_ptr<SelectionElement> SourceSelectionTexture::createElementFromResource(
	const std::shared_ptr<sdk::main::IResource> &pResource) {
	return ResourceElement::create(pResource);
}
} // namespace mer::editor::ui