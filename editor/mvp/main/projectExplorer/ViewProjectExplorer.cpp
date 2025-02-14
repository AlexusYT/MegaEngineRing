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
// Created by alexus on 19.01.24.
//

#include "ViewProjectExplorer.h"

#include <project/Project.h>

#include "IPresenterProjectExplorer.h"
#include "entries/ProjectExplorerElement.h"
#include "imgui_internal.h"
#include "mvp/contexts/IWidgetContext.h"
#include "mvp/dialogs/confirmationDialog/ConfirmationDialog.h"
#include "ui/customWidgets/CustomSignalListItemFactory.h"
#include "ui/customWidgets/CustomTreeView.h"
#include "ui/utils/UiUtils.h"

namespace mer::editor::mvp {
ViewProjectExplorer::ViewProjectExplorer(const std::shared_ptr<IWidgetContext> &pContext)
	: UiWindow("ViewProjectExplorer", "View Project Explorer##ViewProjectExplorer"), context(pContext) {

	dialog = ConfirmationDialog::create("ProjectExplorer-confirm", "ConfirmDeletion");
	dialog->setWindowFlags(ImGuiWindowFlags_NoResize);
	addPopup(dialog);
	setWindowFlags(ImGuiWindowFlags_MenuBar);
#ifdef USE_OLD_UI
	treeView = Gtk::make_managed<ui::CustomTreeView>();
	treeView->set_halign(Gtk::Align::START);
	mainScrolledWindow.set_child(*treeView);
	mainScrolledWindow.set_has_frame(true);
	ViewProjectExplorer::setSelectOnDoubleClick(true);
	addNameColumn();
	addTypeColumn();
	ViewProjectExplorer::setSelectionChangedSlot([this](const ProjectExplorerElement* pElement) {
		auto str = pElement->getPath().string();
		treeView->activate_action("file.open", Glib::Variant<Glib::ustring>::create(str));
	});
#endif
}

void ViewProjectExplorer::setSlotCreateModel(
	[[maybe_unused]] const sigc::slot<std::shared_ptr<Gio::ListModel>(const std::shared_ptr<Glib::ObjectBase> &)>
		&pSlot) {
#ifdef USE_OLD_UI
	treeView->setSlotCreateModel(pSlot);
#endif
}

void ViewProjectExplorer::setElements(const std::shared_ptr<Gio::ListModel> &pArray) {
#ifdef USE_OLD_UI
	treeView->setSlotCreateModel(
		[this, pArray](const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
			if (const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(pItem)) {
				return col->getChildren();
			}
			return pArray;
		});
#else
	array = pArray;
#endif
}

void ViewProjectExplorer::setSelectionChangedSlot(
	[[maybe_unused]] const sigc::slot<void(ProjectExplorerElement* pElement)> &pSlot) {

#ifdef USE_OLD_UI
	treeView->setSlotSelectionChanged([this, pSlot](Glib::ObjectBase* pObjectBase) {
		const auto element = dynamic_cast<ProjectExplorerElement*>(pObjectBase);
		if (!element) return;
		pSlot(element);
	});
#endif
}

void ViewProjectExplorer::setSelectOnDoubleClick([[maybe_unused]] const bool pSelectOnDoubleClick) {
#ifdef USE_OLD_UI
	treeView->setSelectOnDoubleClick(pSelectOnDoubleClick);
#endif
}

void ViewProjectExplorer::openView() {
#ifdef USE_OLD_UI
	context->addWidget(&mainScrolledWindow);
#else
	context->addWindow(this);
#endif
}

void ViewProjectExplorer::closeView() { context->removeWidget(); }

void ViewProjectExplorer::updateUi() {

	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	/*auto slot = [this](const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
		if (const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(pItem)) { return col->getChildren(); }
		return array;
	};*/
	static ImGuiTableFlags table_flags =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("3ways", 2, table_flags)) {
		// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
		ImGui::TableHeadersRow();

		updateTreeLevel(array);

		if (ImGui::BeginPopup("Menu123")) {
			if (ImGui::BeginMenu("New Resource")) {
				if (ImGui::MenuItem("Model")) {}
				if (ImGui::MenuItem("Material")) {}
				if (ImGui::MenuItem("Texture")) {}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("New Folder")) {}
			if (ImGui::MenuItem("New Scene")) {}
			if (ImGui::MenuItem("New Script")) {}
			ImGui::Separator();
			ImGui::BeginDisabled();
			if (ImGui::MenuItem("Rename")) {}
			ImGui::SetItemTooltip("Not implemented yet");
			ImGui::EndDisabled();
			if (ImGui::MenuItem("Delete")) {
				ImGui::OpenPopup(dialog->getId(), 0);
				//b = true;
				if (selectedElement && presenter) { presenter->deleteFile(selectedElement->getPath()); }
			}
			ImGui::Separator();
			ImGui::BeginDisabled();
			if (ImGui::MenuItem("Show in Files")) {
				//if (selectedElement && presenter) { presenter->openInFilesystem(selectedElement->getPath()); }
			}
			ImGui::SetItemTooltip("Not implemented yet");
			ImGui::EndDisabled();
			ImGui::EndPopup();
		}

		ImGui::EndTable();
	}
	/*if (ImGui::BeginPopupModal("Delete?11###ProjectExplorer-delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}*/
}

void ViewProjectExplorer::startConfirmation(const std::string &pTitle, const std::string &pMessage,
											const std::function<void(int pId)> &pResult,
											const std::vector<std::string> &pButtons) {
	dialog->setTitle(pTitle);
	dialog->setMessage(pMessage);
	dialog->setButtons(pButtons);
	dialog->setButtonClickedCallback(pResult);
}

void ViewProjectExplorer::updateTreeLevel(const std::shared_ptr<Gio::ListModel> &pElements) {


	for (uint32_t i = 0; i < pElements->get_n_items(); ++i) {


		auto node = pElements->get_typed_object<ProjectExplorerElement>(i);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		bool folder = node->getChildren()->get_n_items() > 0;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAllColumns;
		if (!folder) { flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; }
		bool open = ImGui::TreeNodeEx(node->getName().c_str(), flags);
		if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonRight) &&
			ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
			ImGui::OpenPopup("Menu123", ImGuiPopupFlags_MouseButtonRight);
			selectedElement = node;
		}
		//ImGui::OpenPopupOnItemClick(id.c_str(), ImGuiPopupFlags_MouseButtonRight);
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(node->getTypeStr());
		if (open && folder) {

			updateTreeLevel(node->getChildren());
			ImGui::TreePop();
		}
	}
}

#ifdef USE_OLD_UI
void ViewProjectExplorer::addNameColumn() const {

	const auto factory = ui::CustomSignalListItemFactory::create();
	factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		auto* expander = Gtk::make_managed<Gtk::TreeExpander>();
		auto* label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::END);
		label->set_margin(3);
		expander->set_child(*label);
		pListItem->set_child(*expander);
	});
	factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;

		const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(row->get_item());
		if (!col) return;
		auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
		if (!expander) return;
		expander->set_list_row(row);
		auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
		if (!label) return;
		label->set_text(col->getName());
	});

	const auto column = Gtk::ColumnViewColumn::create("Name", factory);
	column->set_resizable(true);
	treeView->append_column(column);
}

void ViewProjectExplorer::addTypeColumn() const {
	const auto factory = ui::CustomSignalListItemFactory::create();
	factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		auto* label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::END);
		label->set_margin(3);
		pListItem->set_child(*label);
	});
	factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;
		const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(row->get_item());
		if (!col) return;
		auto* label = dynamic_cast<Gtk::Label*>(pListItem->get_child());
		if (!label) return;
		switch (col->getType()) {
			case ExplorerElementType::DIRECTORY: label->set_text("Folder"); break;
			case ExplorerElementType::SCENE: label->set_text("Scene"); break;
			case ExplorerElementType::RESOURCE_MODEL: label->set_text("Model"); break;
			case ExplorerElementType::RESOURCE_TEXTURE: label->set_text("Texture"); break;
			case ExplorerElementType::RESOURCE_MATERIAL: label->set_text("Material"); break;
			default:; label->set_text("File");
		}
	});

	const auto column = Gtk::ColumnViewColumn::create("Type", factory);
	column->set_resizable(true);
	treeView->append_column(column);
}
#endif

void ViewProjectExplorer::onPathChanged([[maybe_unused]] const std::filesystem::path &pPath) const {
#ifdef USE_OLD_UI
	auto menu = Gio::Menu::create();
	const auto variant = Glib::Variant<Glib::ustring>::create(pPath.string());
	const auto menuNewResource = Gio::Menu::create();
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Model", "new.resource.model", variant));
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Material", "new.resource.material", variant));
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Texture", "new.resource.texture", variant));
	const auto menuNew = Gio::Menu::create();
	menuNew->append_submenu("Resource", menuNewResource);
	menuNew->append_item(ui::UiUtils::createMenuItem("Folder", "new.folder", variant));
	menuNew->append_item(ui::UiUtils::createMenuItem("Scene", "new.scene", variant));
	menuNew->append_item(ui::UiUtils::createMenuItem("Script", "new.script", variant));
	menu->append_submenu("New", menuNew);
	treeView->setMenu(menu);
#endif
}

void ViewProjectExplorer::selectByUri([[maybe_unused]] const std::filesystem::path &pPath) {

#ifdef USE_OLD_UI
	auto selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(treeView->get_model());
	if (!selection) return;
	for (uint32_t i = 0, maxI = selection->get_n_items(); i < maxI; ++i) {
		auto row = selection->get_typed_object<Gtk::TreeListRow>(i);
		if (!row) return;

		auto pos = getPositionByUri(pPath, row);
		if (!pos) continue;
		selection->select_item(pos.value(), true);
	}
#endif
}

std::optional<uint32_t> ViewProjectExplorer::getPositionByUri(const std::filesystem::path &pPath,
															  const std::shared_ptr<Gtk::TreeListRow> &pRow) {

	auto item = std::dynamic_pointer_cast<ProjectExplorerElement>(pRow->get_item());
	if (!item) return {};
	auto relativePath = item->getRelativePath();
	if (relativePath == pPath) return pRow->get_position();

	pRow->set_expanded(true);
	auto children = pRow->get_children();

	for (uint32_t i = 0, maxI = children->get_n_items(); i < maxI; ++i) {
		auto row = pRow->get_child_row(i);
		if (!row) return {};
		return getPositionByUri(pPath, row);
	}
	pRow->set_expanded(false);
	return {};
}
} // namespace mer::editor::mvp