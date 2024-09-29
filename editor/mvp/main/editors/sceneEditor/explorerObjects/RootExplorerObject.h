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

#ifndef ROOTEXPLOREROBJECT_H
#define ROOTEXPLOREROBJECT_H
#include "ExplorerObject.h"

namespace mer::editor::mvp {
class RootExplorerObject : public ExplorerObject {
	std::string name;

public:
	static std::shared_ptr<RootExplorerObject> create() {
		return Glib::make_refptr_for_instance(new RootExplorerObject());
	}

	[[nodiscard]] const std::string &getName() const override { return name; }

	void setName(const std::string &pName) override { name = pName; }

	std::shared_ptr<Gio::ListStore<ObjectExtensionEntry>> getPropertyEntries() const override { return nullptr; }

	Glib::RefPtr<Gio::MenuModel> getMenu() override {
		return nullptr;
	}
};
} // namespace mer::editor::mvp


#endif //ROOTEXPLOREROBJECT_H
