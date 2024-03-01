// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 29.01.24.
//

#include "PresenterAppInfo.h"

#include <project/Project.h>
#include <project/generatedFiles/ApplicationInfo.h>

#include "IModelAppInfo.h"
#include "IViewAppInfo.h"

namespace mer::editor::mvp {

PresenterAppInfo::PresenterAppInfo(const std::shared_ptr<IViewAppInfo> &pViewAppInfo,
								   const std::shared_ptr<IModelAppInfo> &pModelAppInfo)
	: viewAppInfo(pViewAppInfo), modelAppInfo(pModelAppInfo) {
	viewAppInfo->setSlotCreateModel(
		[this](const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
			auto col = std::dynamic_pointer_cast<project::AppInfoProperty>(pItem);
			Glib::RefPtr<Gio::ListStore<project::AppInfoProperty>> result;
			if (!col) { result = modelAppInfo->getInfo()->getPropertiesList(); }
			// If result is empty, it's a leaf in the tree, i.e. an item without children.
			// Returning an empty RefPtr (not a RefPtr with an empty Gio::ListModel)
			// signals that the item is not expandable.
			return result;
			//	return (result->get_n_items() > 0) ? result : Glib::RefPtr<Gio::ListModel>();
		});
	viewAppInfo->setValueChangedSlot(
		[this](const std::shared_ptr<project::AppInfoProperty> &pInfoProperty, const std::string &pNewValue) {
			pInfoProperty->setValue(pNewValue);
			tabHeaderChanged("*" + modelAppInfo->getInfo()->getName());
		});
}

PresenterAppInfo::operator Gtk::Widget&() { return viewAppInfo->getMainWidget(); }
} // namespace mer::editor::mvp