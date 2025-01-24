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
// Created by alexus on 25.01.24.
//

#ifndef IVIEWSTARTUP_H
#define IVIEWSTARTUP_H

#include <gtkmm/entry.h>
#include <gtkmm/filedialog.h>

#include "mvp/IView.h"

namespace mer::editor::mvp {

class IViewStartup : public IView {
public:
	~IViewStartup() override = default;

	virtual sigc::connection connectNewProjectClickSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectCancelClickSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectCreateClickSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectOpenProjectClickSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectPathChangedSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectIconReleasedSignal(
		const sigc::slot<void(Gtk::Entry::IconPosition)> &pSlot) const = 0;

	virtual sigc::connection connectNameChangedSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual void setPath(const std::string &pPath) const = 0;

	virtual std::string getPath() const = 0;

	virtual void setName(const std::string &pPath) const = 0;

	virtual std::string getName() const = 0;

	virtual void switchMode(int pMode) = 0;

	virtual void showError(const std::string &pText) const = 0;

	virtual void hideError() const = 0;

	virtual std::shared_ptr<Gtk::FileDialog> showFolderChooserDialog(
		const std::string &pInitialFolder,
		const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
			&pSlot) = 0;

	virtual void showFileChooserDialog(
		const std::string &pInitialFolder,
		const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
			&pSlot) = 0;
};
} // namespace mer::editor::mvp


#endif //IVIEWSTARTUP_H
