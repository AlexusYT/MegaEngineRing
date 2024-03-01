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

#ifndef PRESENTERAPPINFO_H
#define PRESENTERAPPINFO_H
#include <mvp/main/editors/IEditorPresenter.h>

namespace mer::editor::mvp {
class IViewAppInfo;
class IModelAppInfo;

class PresenterAppInfo : public IEditorPresenter {
	std::shared_ptr<IViewAppInfo> viewAppInfo;
	std::shared_ptr<IModelAppInfo> modelAppInfo;
	sigc::signal<void(const std::string &pName)> tabHeaderChanged;

public:
	PresenterAppInfo(const std::shared_ptr<IViewAppInfo> &pViewAppInfo,
					 const std::shared_ptr<IModelAppInfo> &pModelAppInfo);

	operator Gtk::Widget&() override;

	sigc::connection connectTabHeaderChanged(const sigc::slot<void(const std::string &pName)> &pSlot) override {
		return tabHeaderChanged.connect(pSlot);
	}
};

} // namespace mer::editor::mvp

#endif //PRESENTERAPPINFO_H
