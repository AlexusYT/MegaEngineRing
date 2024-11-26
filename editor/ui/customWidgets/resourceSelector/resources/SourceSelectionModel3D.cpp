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
// Created by alexus on 09.11.24.
//

#include "SourceSelectionModel3D.h"

#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "EngineSDK/main/resources/models/Model3DResource.h"
#include "ui/customWidgets/resourceSelector/ISourceSelectionResult.h"

namespace mer::editor::ui {


class ModelObjectElement : public SelectionElement {
	std::shared_ptr<sdk::main::IModel3DObject> object;

	explicit ModelObjectElement(const std::shared_ptr<sdk::main::IModel3DObject> &pObject) : object(pObject) {}

public:
	static std::shared_ptr<ModelObjectElement> create(const std::shared_ptr<sdk::main::IModel3DObject> &pObject) {
		return Glib::make_refptr_for_instance(new ModelObjectElement(pObject));
	}

	bool selectable() override { return true; }

	std::shared_ptr<Gio::ListStore<SelectionElement>> getChildren() override { return nullptr; }

	const std::string &getName() const override { return object->getName(); }

	[[nodiscard]] const std::shared_ptr<sdk::main::IModel3DObject> &getObject() const { return object; }
};

class ModelResourceElement : public ResourceElement {

	explicit ModelResourceElement(const std::shared_ptr<sdk::main::IResource> &pResource)
		: ResourceElement(pResource) {}

public:
	static std::shared_ptr<ModelResourceElement> create(const std::shared_ptr<sdk::main::IResource> &pResource) {
		return Glib::make_refptr_for_instance(new ModelResourceElement(pResource));
	}

	bool selectable() override { return false; }

	std::shared_ptr<Gio::ListStore<SelectionElement>> getChildren() override {
		auto model = std::dynamic_pointer_cast<sdk::main::IModel3DResource>(getResource());
		if (!model) return nullptr;
		auto children = Gio::ListStore<SelectionElement>::create();
		for (auto modelObject: model->getModelObjects()) {
			children->append(ModelObjectElement::create(modelObject.second));
		}
		children->sort(
			[](const std::shared_ptr<const SelectionElement> &pA, const std::shared_ptr<const SelectionElement> &pB) {
				return pA->getName().compare(pB->getName());
			});
		return children;
	}
};

SourceSelectionModel3D::SourceSelectionModel3D(sdk::main::IResourceLoadExecutor* pLoader)
	: SourceSelectionResource(pLoader) {}

std::shared_ptr<SourceSelectionModel3D> SourceSelectionModel3D::create(sdk::main::IResourceLoadExecutor* pLoader) {
	return std::shared_ptr<SourceSelectionModel3D>(new SourceSelectionModel3D(pLoader));
}

void SourceSelectionModel3D::onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	if (const auto model = std::dynamic_pointer_cast<Result>(pResult)) {
		const auto modelObject = model->getSelectionResult();
		const auto modelResource = modelObject->getIModelResource()->asResource();

		std::shared_ptr<Gtk::TreeListRow> row = findRow(modelResource);
		if (!row) return;
		row->set_expanded();
		Glib::signal_timeout().connect(
			[this, row, modelObject] {
				auto children = row->get_children();
				if (!children) return false;
				for (auto i = 0u, maxI = children->get_n_items(); i < maxI; ++i) {
					auto childRow = row->get_child_row(i);
					if (!childRow) continue;
					auto element = std::dynamic_pointer_cast<ModelObjectElement>(childRow->get_item());
					if (!element) continue;
					if (element->getObject() == modelObject) { scrollToRow(childRow); }
				}
				return false;
			},
			100);
	}
}

bool SourceSelectionModel3D::isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	return std::dynamic_pointer_cast<Result>(pResult) != nullptr;
}

std::string SourceSelectionModel3D::getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	if (const auto model = std::dynamic_pointer_cast<Result>(pResult)) {
		const auto object = model->getSelectionResult();
		const auto resource = object->getIModelResource()->asResource();
		return resource->getResourceName() + "/" + object->getName();
	}
	return "";
}

std::string SourceSelectionModel3D::getName() { return "model-resource"; }

std::string SourceSelectionModel3D::getTitle() { return "Model"; }

void SourceSelectionModel3D::onElementSelected(const std::shared_ptr<SelectionElement> &pElement) {

	if (const auto objectElement = std::dynamic_pointer_cast<ModelObjectElement>(pElement)) {
		resultCallback(Result::create(objectElement->getObject()));
	}
}

std::shared_ptr<SelectionElement> SourceSelectionModel3D::createElementFromResource(
	const std::shared_ptr<sdk::main::IResource> &pResource) {
	return ModelResourceElement::create(pResource);
}

} // namespace mer::editor::ui