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
// Created by alexus on 09.11.24.
//

#ifndef SOURCESELECTIONMODEL3D_H
#define SOURCESELECTIONMODEL3D_H
#include "SourceSelectionResource.h"
#include "TypedSourceSelectionResult.h"
#include "ui/customWidgets/resourceSelector/ISourceSelectionResult.h"

namespace mer::sdk {
class IModel3DObject;
}

namespace mer::sdk {
class IResourceLoadExecutor;
} // namespace mer::sdk

namespace mer::editor::ui {

class SourceSelectionModel3D : public SourceSelectionResource {
	explicit SourceSelectionModel3D(sdk::IResourceLoadExecutor* pLoader);

public:
	using Result = TypedSourceSelectionResult<sdk::IModel3DObject>;

	static std::shared_ptr<SourceSelectionModel3D> create(sdk::IResourceLoadExecutor* pLoader);

	void onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	bool isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	std::string getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	std::string getName() override;

	std::string getTitle() override;

	void onElementSelected(const std::shared_ptr<SelectionElement> &pElement) override;

	sdk::ResourceType getResourceType() override { return sdk::ResourceType::MODEL; }

	std::shared_ptr<SelectionElement> createElementFromResource(
		const std::shared_ptr<sdk::IResource> &pResource) override;
};


} // namespace mer::editor::ui

#endif //SOURCESELECTIONMODEL3D_H
