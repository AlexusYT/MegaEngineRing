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
// Created by alexus on 31.08.24.
//

#include "ExplorerObject.h"

namespace mer::editor::mvp {
ExplorerObject::ExplorerObject() : TreeElementBase(Gio::ListStore<ExplorerObject>::create()) {}

void ExplorerObject::addChild(const std::shared_ptr<ExplorerObject> &pChild) {
	auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ExplorerObject>>(getChildren());
	childrenSelf->append(pChild);
	pChild->parent = this;
	notifyChanged();
}

void ExplorerObject::removeChild(const std::shared_ptr<ExplorerObject> &pChild) const {
	auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ExplorerObject>>(getChildren());
	auto result = childrenSelf->find(pChild);
	if (!result.first) return;
	childrenSelf->remove(result.second);
	notifyChanged();
}

void ExplorerObject::clearChildren() const {
	auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ExplorerObject>>(getChildren());
	childrenSelf->remove_all();
	notifyChanged();
}
} // namespace mer::editor::mvp