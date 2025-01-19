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
// Created by alexus on 17.09.24.
//

#ifndef MULTIPANEDCONTEXT_H
#define MULTIPANEDCONTEXT_H
#include "IWidgetContext.h"
#include "ui/customWidgets/multipaned/MultiPaned.h"

namespace mer::editor::ui {
enum class MultiPanedSide;
class MultiPaned;
} // namespace mer::editor::ui

namespace mer::editor::mvp {
class MultiPanedContext : public IWidgetContext {
	ui::MultiPaned* multiPaned;
	Gtk::Widget* widget{};

	MultiPanedContext(ui::MultiPaned* pMultiPaned) : multiPaned(pMultiPaned) {}

public:
	static std::shared_ptr<MultiPanedContext> create(ui::MultiPaned* pMultiPaned) {
		return std::shared_ptr<MultiPanedContext>(new MultiPanedContext(pMultiPaned));
	}

	void addWidget(Gtk::Widget* pWidget) override;

	void removeWidget() override;

	void setTitle(const std::string &pTitle) override;

	[[nodiscard]] Gtk::Widget* getWidget() const { return widget; }
};
} // namespace mer::editor::mvp


#endif //MULTIPANEDCONTEXT_H
