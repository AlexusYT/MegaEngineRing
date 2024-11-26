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
// Created by alexus on 17.11.24.
//

#ifndef SOURCESELECTIONMATERIAL_H
#define SOURCESELECTIONMATERIAL_H
#include "SourceSelectionResource.h"
#include "TypedSourceSelectionResult.h"

namespace mer::sdk::main {
class IMaterialResource;
}

namespace mer::editor::ui {

class SourceSelectionMaterial : public SourceSelectionResource {
	explicit SourceSelectionMaterial(sdk::main::IResourceLoadExecutor* pLoader) : SourceSelectionResource(pLoader) {}

public:
	using Result = TypedSourceSelectionResult<sdk::main::IMaterialResource>;

	static std::shared_ptr<SourceSelectionMaterial> create(sdk::main::IResourceLoadExecutor* pLoader);

	std::string getName() override { return "material-resource"; }

	std::string getTitle() override { return "Material"; }

	void onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	bool isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	std::string getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	void onElementSelected(const std::shared_ptr<SelectionElement> &pElement) override;

	sdk::main::ResourceType getResourceType() override { return sdk::main::ResourceType::MATERIAL; }

	std::shared_ptr<SelectionElement> createElementFromResource(
		const std::shared_ptr<sdk::main::IResource> &pResource) override;
};

} // namespace mer::editor::ui

#endif //SOURCESELECTIONMATERIAL_H
