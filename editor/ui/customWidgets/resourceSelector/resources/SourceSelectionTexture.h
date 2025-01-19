//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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

#ifndef SOURCESELECTIONTEXTURE_H
#define SOURCESELECTIONTEXTURE_H
#include "SourceSelectionResource.h"
#include "TypedSourceSelectionResult.h"

namespace mer::sdk::main {
class ITextureResource;
}

namespace mer::editor::ui {

class SourceSelectionTexture : public SourceSelectionResource {
	explicit SourceSelectionTexture(sdk::main::IResourceLoadExecutor* pLoader) : SourceSelectionResource(pLoader) {}

public:
	using Result = TypedSourceSelectionResult<sdk::main::ITextureResource>;

	static std::shared_ptr<SourceSelectionTexture> create(sdk::main::IResourceLoadExecutor* pLoader);

	std::string getName() override { return "texture-resource"; }

	std::string getTitle() override { return "Texture"; }

	void onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	bool isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	std::string getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	void onElementSelected(const std::shared_ptr<SelectionElement> &pElement) override;

	sdk::main::ResourceType getResourceType() override { return sdk::main::ResourceType::TEXTURE; }

	std::shared_ptr<SelectionElement> createElementFromResource(
		const std::shared_ptr<sdk::main::IResource> &pResource) override;
};

} // namespace mer::editor::ui

#endif //SOURCESELECTIONTEXTURE_H
