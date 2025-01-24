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
// Created by alexus on 07.11.24.
//

#ifndef ISOURCESELECTION_H
#define ISOURCESELECTION_H

#include <sigc++/functors/slot.h>

namespace Gtk {
class Widget;
}

namespace mer::editor::ui {
class ResourceSelectorWidget;
class ISourceSelectionResult;

class ISourceSelection {
public:
	virtual ~ISourceSelection() = default;

	virtual std::string getName() = 0;

	virtual std::string getTitle() = 0;

	virtual Gtk::Widget* getWidget() = 0;

	virtual void onShow() {}

	virtual void setParent(ResourceSelectorWidget* pParent) = 0;

	virtual void setResultCallback(
		const sigc::slot<void(const std::shared_ptr<ISourceSelectionResult> &pResult)> &pSignal) = 0;

	virtual void onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) = 0;

	virtual bool isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) = 0;

	virtual std::string getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) = 0;
};

} // namespace mer::editor::ui

#endif //ISOURCESELECTION_H
