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
// Created by alexus on 25.09.24.
//

#ifndef PRESENTEROBJECTSTREE_H
#define PRESENTEROBJECTSTREE_H
#include <memory>

#include "IPresenterObjectsTree.h"

namespace mer::editor::mvp {
class IViewObjectsTree;
class IModelObjectsTree;

class PresenterObjectsTree : public IPresenterObjectsTree {
	std::shared_ptr<IModelObjectsTree> model;
	std::vector<std::shared_ptr<IViewObjectsTree>> views;

public:
	explicit PresenterObjectsTree(const std::shared_ptr<IModelObjectsTree> &pModel) : model(pModel) {}

	void run() override;

	void stop() override;

	void addView(const std::shared_ptr<IView> &pNewView) override;

	std::string getTypeName() override {
		return "PresenterObjectsTree";
	}
};

} // namespace mer::editor::mvp

#endif //PRESENTEROBJECTSTREE_H
