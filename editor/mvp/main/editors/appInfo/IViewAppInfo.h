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
// Created by alexus on 29.01.24.
//

#ifndef IVIEWAPPINFO_H
#define IVIEWAPPINFO_H
#include <mvp/IView.h>
#include <ui/customWidgets/TreeWidget.h>

namespace mer::editor::project {
class AppInfoProperty;
} // namespace mer::editor::project

namespace mer::editor::mvp {

class IViewAppInfo : public IView {
public:
	using ValueChangedSlot =
		sigc::slot<void(const std::shared_ptr<project::AppInfoProperty> &pInfoProperty, const std::string &pNewValue)>;
	virtual void setSlotCreateModel(const ui::TreeWidget::SlotCreateModel &pSlotCreateModel) = 0;

	virtual Gtk::Widget &getMainWidget() = 0;

	[[nodiscard]] virtual const ValueChangedSlot &getValueChangedSlot() const = 0;

	virtual void setValueChangedSlot(const ValueChangedSlot &pValueChangedSlot) = 0;
};

} // namespace mer::editor::mvp

#endif //IVIEWAPPINFO_H
