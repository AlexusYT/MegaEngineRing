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
// Created by alexus on 07.11.24.
//

#ifndef RESOURCESELECTORWIDGET_H
#define RESOURCESELECTORWIDGET_H

namespace mer::editor::ui {
class ISourceSelectionResult;
class ISourceSelection;
} // namespace mer::editor::ui

namespace mer::editor::ui {

class ResourceSelectorWidget : public Gtk::MenuButton {
	bool hasSelectionNone{true};
	std::map<std::string, std::shared_ptr<ISourceSelection>> sources;
	std::shared_ptr<Gtk::StackSwitcher> stackSwitcher;
	std::shared_ptr<Gtk::Stack> stack;
	std::shared_ptr<ISourceSelectionResult> selection;
	sigc::signal<void(const std::shared_ptr<ISourceSelectionResult> &pResult)> selectionChangedSignal;

public:
	ResourceSelectorWidget();

	void addSource(const std::shared_ptr<ISourceSelection> &pSource);

	[[nodiscard]] sigc::signal<void(const std::shared_ptr<ISourceSelectionResult> &pResult)> &getSelectionChangedSignal() {
		return selectionChangedSignal;
	}

	[[nodiscard]] const std::shared_ptr<ISourceSelectionResult> &getSelection() const { return selection; }

	void setSelection(const std::shared_ptr<ISourceSelectionResult> &pSelection);

	void switchTo(const std::string &pName) const;

protected:
	void on_realize() override;

	void size_allocate_vfunc(int pWidth, int pHeight, int pBaseline) override;

	void updateLabel();
};

} // namespace mer::editor::ui

#endif //RESOURCESELECTORWIDGET_H
