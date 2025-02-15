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
// Created by alexus on 30.09.24.
//

#include "ViewResourceEditor.h"

#include "EngineSDK/resources/materials/ColorComponent.h"
#include "EngineSDK/resources/materials/IMaterialResource.h"
#include "EngineSDK/resources/models/IModel3DObject.h"
#include "EngineSDK/resources/textures/ITextureResource.h"
#include "IPresenterResourceEditor.h"
#include "mvp/contexts/IWidgetContext.h"
#include "ui/customWidgets/resourceSelector/ResourceSelectorWidget.h"
#include "ui/customWidgets/resourceSelector/SourceSelectionColor.h"
#include "ui/customWidgets/resourceSelector/SourceSelectionNone.h"
#include "ui/customWidgets/resourceSelector/resources/SourceSelectionTexture.h"
#include "ui/utils/UiUtils.h"

namespace mer::editor::mvp {
class ModelObject : public Glib::Object {
	std::shared_ptr<sdk::IModel3DObject> object{};
	bool importBtn{};

	ModelObject(const std::shared_ptr<sdk::IModel3DObject> &pObject, bool pImportBtn)
		: object(pObject), importBtn(pImportBtn) {}

public:
	static std::shared_ptr<ModelObject> create(const std::shared_ptr<sdk::IModel3DObject> &pObject,
											   bool pImportBtn) {
		return Glib::make_refptr_for_instance(new ModelObject(pObject, pImportBtn));
	}

	[[nodiscard]] const std::shared_ptr<sdk::IModel3DObject> &getObject() const { return object; }

	[[nodiscard]] bool isImportBtn() const { return importBtn; }

	void setImportBtn(bool pImportBtn) { importBtn = pImportBtn; }
};

std::string OpenedResource::getType() const {
	switch (resource->getResourceType()) {

		case sdk::ResourceType::NONE: break;
		case sdk::ResourceType::MODEL: return "Model";
		case sdk::ResourceType::TEXTURE: return "Texture";
		case sdk::ResourceType::MATERIAL: return "Material";
		case sdk::ResourceType::SHADER: break;
	}
	return "Unknown";
}

ViewResourceEditor::ViewResourceEditor(const std::shared_ptr<IWidgetContext> &pContext,
									   const std::shared_ptr<Gtk::Builder> &pBuilder)
	: context(pContext), builder(pBuilder) {
	fileObjectsListView = builder->get_widget<Gtk::ListView>("fileObjectsListView");
	resourceObjectsListView = builder->get_widget<Gtk::ListView>("resourceObjectsListView");
	auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		auto box = Gtk::make_managed<Gtk::Box>();
		auto nameLbl = Gtk::make_managed<Gtk::Label>();
		nameLbl->set_hexpand();
		nameLbl->set_halign(Gtk::Align::START);
		box->append(*nameLbl);
		auto actionBtn = Gtk::make_managed<Gtk::Button>();
		actionBtn->set_has_frame(false);
		box->append(*actionBtn);
		box->set_margin_end(10);
		pItem->set_child(*box);
	});

	factory->signal_bind().connect([this](const std::shared_ptr<Gtk::ListItem> &pItem) {
		auto data = std::dynamic_pointer_cast<ModelObject>(pItem->get_item());
		if (!data) return;
		auto box = dynamic_cast<Gtk::Box*>(pItem->get_child());
		if (!box) return;
		auto label = dynamic_cast<Gtk::Label*>(box->get_first_child());
		if (!label) return;
		label->set_label(data->getObject()->getName());
		auto button = dynamic_cast<Gtk::Button*>(box->get_last_child());
		if (!button) return;
		button->signal_clicked().connect([this, data] {
			if (data->isImportBtn()) {
				presenter->addObject(data->getObject(), this);
			} else {
				presenter->removeObject(data->getObject(), this);
			}
		});

		if (data->isImportBtn()) {
			button->set_image_from_icon_name("list-add-symbolic");
		} else {
			button->set_image_from_icon_name("list-remove-symbolic");
		}
	});
	fileObjectsListView->set_factory(factory);
	resourceObjectsListView->set_factory(factory);
	unsavedDialogBox = builder->get_widget<Gtk::Box>("unsavedDialogBox");

	mainStack = builder->get_widget<Gtk::Stack>("mainStack");
	resourcePathEntry = builder->get_widget<Gtk::Entry>("resourcePathEntry");
	resourcePathEntry->signal_changed().connect([this] {
		std::filesystem::path path{resourcePathEntry->get_text().raw()};
		if (path.empty()) {
			markErrored(resourcePathEntry);
			return;
		}
		unmarkErrored(resourcePathEntry);
		updateUnsavedDialog(presenter->getSelectedResource(this));
	});
	resourceNameEntry = builder->get_widget<Gtk::Entry>("nameEntry");
	resourceNameEntry->signal_changed().connect([this] {
		auto name = resourceNameEntry->get_text().raw();
		if (name.empty()) {
			markErrored(resourceNameEntry);
			return;
		}
		unmarkErrored(resourceNameEntry);
		updateUnsavedDialog(presenter->getSelectedResource(this));
	});
	/*auto stackVisible = mainStack->property_visible_child();
	stackVisible.signal_changed().connect([this, stackVisible] {
		if (!presenter) return;
		auto scroll = dynamic_cast<Gtk::ScrolledWindow*>(stackVisible.get_value());
		//templateStackPage->unparent();

		scroll->set_child(*templateStackPage);
		});*/
	auto createNewMBtn = builder->get_widget<Gtk::MenuButton>("createNewMBtn");
	auto menuCreateNew = Gio::Menu::create();
	auto variant = Glib::Variant<Glib::ustring>::create("/");
	menuCreateNew->append_item(ui::UiUtils::createMenuItem("Model", "new.resource.model", variant));
	menuCreateNew->append_item(ui::UiUtils::createMenuItem("Material", "new.resource.material", variant));
	menuCreateNew->append_item(ui::UiUtils::createMenuItem("Texture", "new.resource.texture", variant));
	createNewMBtn->set_menu_model(menuCreateNew);

	selectHistoryMb = builder->get_widget<Gtk::MenuButton>("selectHistoryMB");

	setupColumnView();

	builder->get_widget<Gtk::Button>("saveBtn")->signal_clicked().connect([this] {
		if (!presenter) return;
		presenter->savePathClicked(this, resourcePathEntry->get_text(), resourceNameEntry->get_text());
	});

	builder->get_widget<Gtk::Button>("discardBtn")->signal_clicked().connect([this] {
		auto resource = presenter->getSelectedResource(this);
		updateResourcePath(resource);
		//if (presenter) presenter->onCancelClicked();
	});
	auto nameEntry = builder->get_widget<Gtk::Entry>("nameEntry");
	nameEntry->signal_changed().connect([this, nameEntry] {
		if (presenter) presenter->onResourceNameChanged(nameEntry->get_text());
	});


	widget = builder->get_widget<Gtk::Widget>("root");
	preferencesStack = builder->get_widget<Gtk::Stack>("preferencesStack");
	preferencesStack->add(*builder->get_widget<Gtk::Box>("modelEditor"), "model");
	preferencesStack->add(*builder->get_widget<Gtk::Box>("textureEditor"), "texture");
	preferencesStack->add(*builder->get_widget<Gtk::Box>("materialEditor"), "material");
}

std::shared_ptr<ViewResourceEditor> ViewResourceEditor::create(const std::shared_ptr<IWidgetContext> &pContext) {
	std::shared_ptr<Gtk::Builder> builder;
	try {
		builder = Gtk::Builder::create_from_resource(
			"/ui/ResourceEditorWindow.ui",
			std::vector<Glib::ustring>{"root", "modelEditor", "textureEditor", "materialEditor"});
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to load ui file");
		msg->setMessage("Error while loading resource creation window");
		sdk::Logger::error(msg);
		return nullptr;
	}
	return std::shared_ptr<ViewResourceEditor>(new ViewResourceEditor(pContext, builder));
}

void ViewResourceEditor::openView() { context->addWidget(widget); }

void ViewResourceEditor::closeView() { context->removeWidget(); }

void ViewResourceEditor::setPresenter(IPresenterResourceEditor* pPresenter) {
	presenter = pPresenter;
	auto iconReleaseSlot = hide(bind(sigc::mem_fun(*presenter, &IPresenterResourceEditor::chooseFileClicked), this));

	builder->get_widget<Gtk::Entry>("modelFilePathEntry")->signal_icon_release().connect(iconReleaseSlot);
	builder->get_widget<Gtk::Entry>("textureFilePathEntry")->signal_icon_release().connect(iconReleaseSlot);
	//builder->get_widget<Gtk::Entry>("materialFilePathEntry")->signal_icon_release().connect(iconReleaseSlot);

	matBaseColorSelector = addNewSelector("Base color (Albedo)", 1);
	matBaseColorSelector->getSelectionChangedSignal().connect(
		[this](const std::shared_ptr<ui::ISourceSelectionResult> &pResult) {
			presenter->onMaterialBaseColorChanged(pResult, this);
		});

	matMetallicSelector = addNewSelector("Metallic map", 2);
	matMetallicSelector->getSelectionChangedSignal().connect(
		[this](const std::shared_ptr<ui::ISourceSelectionResult> &pResult) {
			presenter->onMaterialMetallicChanged(pResult, this);
		});
	matNormalSelector = addNewSelector("Normal map", 3);
	matNormalSelector->getSelectionChangedSignal().connect(
		[this](const std::shared_ptr<ui::ISourceSelectionResult> &pResult) {
			presenter->onMaterialNormalChanged(pResult, this);
		});
	matRoughnessSelector = addNewSelector("Roughness map", 4);
	matRoughnessSelector->getSelectionChangedSignal().connect(
		[this](const std::shared_ptr<ui::ISourceSelectionResult> &pResult) {
			presenter->onMaterialRoughnessChanged(pResult, this);
		});
	matAoSelector = addNewSelector("Ambient occlusion map", 5);
	matAoSelector->getSelectionChangedSignal().connect(
		[this](const std::shared_ptr<ui::ISourceSelectionResult> &pResult) {
			presenter->onMaterialAOChanged(pResult, this);
		});
}

void ViewResourceEditor::displayError(const sdk::ReportMessagePtr &pError) {
	mer::sdk::Logger::error(pError);
	//ErrorDialog::showErrorDialog(this, pError);
}

void ViewResourceEditor::displayChosenPath(const std::string &pPath) {
	executeInMainThread([this, pPath](auto &) {
		if (auto entry = getFilePathEntry()) entry->set_text(pPath);
	});
}

void ViewResourceEditor::displayResourceName(const std::string &pName) {
	builder->get_widget<Gtk::Entry>("nameEntry")->set_text(pName);
}

void ViewResourceEditor::switchTo(const std::string &pTabName) {
	executeInMainThread([this, pTabName](auto &) {
		if (preferencesStack->get_visible_child_name().raw() == pTabName) return;
		preferencesStack->set_visible_child(pTabName);
	});
}

void ViewResourceEditor::setStackVisibility(const bool pState) {
	builder->get_widget<Gtk::Stack>("preferencesStack")->set_visible(pState);
}

void ViewResourceEditor::setSaveButtonSensitivity(const bool pState) const {
	builder->get_widget<Gtk::Button>("saveBtn")->set_sensitive(pState);
}

void ViewResourceEditor::displayMessage(const std::string &pMessage) {
	const auto label = builder->get_widget<Gtk::Label>("messageLabel");
	label->set_visible(!pMessage.empty());
	label->set_text(pMessage);
}

void ViewResourceEditor::displayObjects(const std::vector<std::shared_ptr<sdk::IModel3DObject>> &pObjects,
										bool pFileObjects) {
	executeInMainThread([this, pObjects, pFileObjects](auto &) {
		auto objectsList = Gio::ListStore<ModelObject>::create();

		for (auto object: pObjects) {
			auto modelObject = ModelObject::create(object, pFileObjects);
			objectsList->append(modelObject);
		}
		objectsList->sort(
			[](const Glib::RefPtr<const ModelObject> &pFirst, const Glib::RefPtr<const ModelObject> &pSecond) {
				return pFirst->getObject()->getName().compare(pSecond->getObject()->getName());
			});
		auto selection = Gtk::NoSelection::create(objectsList);
		if (pFileObjects) fileObjectsListView->set_model(selection);
		else
			resourceObjectsListView->set_model(selection);
	});
}

std::vector<std::string> ViewResourceEditor::getSelectedObjects() {
	auto objectSelectionModel = std::dynamic_pointer_cast<Gtk::NoSelection>(fileObjectsListView->get_model());
	if (!objectSelectionModel) return {};
	std::vector<std::string> objects;
	for (uint32_t i = 0, maxI = objectSelectionModel->get_n_items(); i < maxI; i++) {
		auto object = objectSelectionModel->get_typed_object<ModelObject>(i);
		if (!object) continue;
		//objects.push_back(object->getName());
	}
	return objects;
}

void ViewResourceEditor::appendResource(const std::shared_ptr<sdk::IResource> &pNewResource) {
	executeInMainThread([this, pNewResource](auto &) { openedList->append(OpenedResource::create(pNewResource)); });
}

void ViewResourceEditor::removeResource(const std::shared_ptr<sdk::IResource> &pResource) {
	for (uint32_t i = 0, maxI = openedList->get_n_items(); i < maxI; i++) {
		std::shared_ptr<OpenedResource> item = openedList->get_item(i);
		if (!item) continue;
		if (item->getResource() == pResource) {
			openedList->remove(i);
			break;
		}
	}
}

void ViewResourceEditor::selectResource(const std::shared_ptr<sdk::IResource> &pResource) {
	executeInMainThread([this, pResource](auto &) {
		mainStack->set_visible_child("editorPage");
		for (uint32_t i = 0, maxI = openedList->get_n_items(); i < maxI; i++) {
			std::shared_ptr<OpenedResource> item = openedList->get_item(i);
			if (!item) continue;
			if (item->getResource() == pResource && selectionModel->get_selected_item() != item) {
				selectionModel->select_item(i, true);
				break;
			}
		}

		if (auto entry = getFilePathEntry()) entry->set_text("");
		auto resource = presenter->getSelectedResource(this);

		updateResourcePath(resource);

		selectHistoryMb->set_label(
			std::to_string(selectionModel->get_selected() + 1) + "/" + std::to_string(openedList->get_n_items()));
	});
}

void ViewResourceEditor::markErrored(Gtk::Widget* pWidget) {
	if (!pWidget->has_css_class("error")) pWidget->add_css_class("error");
}

void ViewResourceEditor::unmarkErrored(Gtk::Widget* pWidget) {
	if (pWidget->has_css_class("error")) pWidget->remove_css_class("error");
}

void ViewResourceEditor::updateUnsavedDialog(const std::shared_ptr<sdk::IResource> &pResource) const {
	const std::filesystem::path path{resourcePathEntry->get_text().raw()};
	const auto name = resourceNameEntry->get_text().raw();

	if (const auto uri = "/" / pResource->getResourceUri(); uri.parent_path() != path || uri.stem().string() != name) {
		unsavedDialogBox->set_visible(true);
	} else
		unsavedDialogBox->set_visible(false);
}

void ViewResourceEditor::updateResourcePath(const std::shared_ptr<sdk::IResource> &pResource) const {
	if (const auto uri = pResource->getResourceUri(); is_regular_file(presenter->getDataPath() / uri)) {
		auto pathStr = uri.parent_path().string();
		resourcePathEntry->set_text(pathStr.empty() ? "/" : pathStr);
		resourceNameEntry->set_text(uri.stem().string());
	} else {
		resourcePathEntry->set_text(uri.string());
		resourceNameEntry->set_text("");
	}
}

void ViewResourceEditor::setupColumnView() {
	openedColumnView = builder->get_widget<Gtk::ColumnView>("openedColumnView");
	auto nameFactory = Gtk::SignalListItemFactory::create();
	nameFactory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto opened = std::dynamic_pointer_cast<OpenedResource>(pItem->get_item());
		if (!opened) return;
		const auto label = dynamic_cast<Gtk::Label*>(pItem->get_child());
		label->set_label(opened->getName());
	});
	nameFactory->signal_setup().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto label = Gtk::make_managed<Gtk::Label>();
		pItem->set_child(*label);
		label->set_margin(3);
	});
	const auto nameColumn = Gtk::ColumnViewColumn::create("Name", nameFactory);
	openedColumnView->append_column(nameColumn);
	auto typeFactory = Gtk::SignalListItemFactory::create();
	typeFactory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto opened = std::dynamic_pointer_cast<OpenedResource>(pItem->get_item());
		if (!opened) return;
		const auto label = dynamic_cast<Gtk::Label*>(pItem->get_child());
		label->set_label(opened->getType());
	});
	typeFactory->signal_setup().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto label = Gtk::make_managed<Gtk::Label>();
		pItem->set_child(*label);
		label->set_margin(3);
	});
	const auto typeColumn = Gtk::ColumnViewColumn::create("Type", typeFactory);
	openedColumnView->append_column(typeColumn);

	openedList = Gio::ListStore<OpenedResource>::create();
	selectionModel = Gtk::SingleSelection::create(openedList);
	openedColumnView->set_model(selectionModel);
	auto selectedItemProperty = selectionModel->property_selected_item();
	selectedItemProperty.signal_changed().connect([selectedItemProperty, this] {
		auto val = std::dynamic_pointer_cast<OpenedResource>(selectedItemProperty.get_value());
		if (!val) return;
		presenter->onSelectionChanged(val->getResource(), this);
		selectHistoryMb->popdown();
	});
}

Gtk::Entry* ViewResourceEditor::getFilePathEntry() const {
	const auto name = preferencesStack->get_visible_child_name();
	if (name == "material") return nullptr;
	const auto entry = builder->get_widget<Gtk::Entry>(name + "FilePathEntry");
	return entry;
}

void ViewResourceEditor::displayMaterial(const std::shared_ptr<sdk::IMaterialResource> &pMaterialResource) {

	auto baseColorSetter = [this](const std::shared_ptr<sdk::IMaterialComponent> &pComponent,
								  ui::ResourceSelectorWidget* pSelector) {
		std::shared_ptr<ui::ISourceSelectionResult> selection;
		if (const auto map = std::dynamic_pointer_cast<sdk::ITextureResource>(pComponent))
			selection = ui::SourceSelectionTexture::Result::create(map);
		else if (const auto color = std::dynamic_pointer_cast<sdk::ColorComponent>(pComponent))
			selection = ui::SourceSelectionColor::Result::create(color);

		pSelector->setSelection(selection);
	};
	pMaterialResource->getAlbedo().connectEvent(bind(baseColorSetter, matBaseColorSelector));
	pMaterialResource->getMetallic().connectEvent(bind(baseColorSetter, matMetallicSelector));
	pMaterialResource->getNormal().connectEvent(bind(baseColorSetter, matNormalSelector));
	pMaterialResource->getRoughness().connectEvent(bind(baseColorSetter, matRoughnessSelector));
	pMaterialResource->getAo().connectEvent(bind(baseColorSetter, matAoSelector));
}

ui::ResourceSelectorWidget* ViewResourceEditor::addNewSelector(const std::string &pLabelText, const int pRowPos) const {

	const auto grid = builder->get_widget<Gtk::Grid>("materialGrid");
	grid->insert_row(pRowPos);
	const auto label = Gtk::make_managed<Gtk::Label>(pLabelText);
	label->set_ellipsize(Pango::EllipsizeMode::END);
	label->set_xalign(0.0);
	grid->attach(*label, 0, pRowPos, 1, 1);
	const auto selector = Gtk::make_managed<ui::ResourceSelectorWidget>();
	selector->addSource(std::make_shared<ui::SourceSelectionNone>());
	selector->addSource(ui::SourceSelectionTexture::create(presenter->getResourceLoader()));
	selector->addSource(ui::SourceSelectionColor::create());
	grid->attach(*selector, 1, pRowPos, 1, 1);
	return selector;
}

} // namespace mer::editor::mvp