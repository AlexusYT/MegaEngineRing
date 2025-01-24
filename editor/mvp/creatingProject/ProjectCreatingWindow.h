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
// Created by alexus on 23.01.24.
//

#ifndef PROJECTCREATINGWINDOW_H
#define PROJECTCREATINGWINDOW_H

#include <gtkmm/togglebutton.h>
#include <gtkmm/window.h>
#include <mvp/creatingProject/IViewCreatingProject.h>
#include <ui/widgetWindows/LogView.h>

#include "mvp/ThreadDispatcher.h"

namespace mer::editor::mvp {


class ProjectCreatingWindow final : public Gtk::Window, public IViewCreatingProject, public ThreadDispatcher {

	ui::LogView logView;
	Gtk::ToggleButton cancelButton;
	std::vector<std::shared_ptr<Gtk::TextBuffer>> buffers;
	std::shared_ptr<IWidgetContext> context;


public:
	explicit ProjectCreatingWindow(const std::shared_ptr<IWidgetContext> &pContext);

private:
	void addMessageToLog(const std::string &pMessage) override;

	void reportError(sdk::ReportMessagePtr pMessage) override;

	void closeView() override;

	void openView() override;
};
} // namespace mer::editor::mvp

#endif //PROJECTCREATINGWINDOW_H
