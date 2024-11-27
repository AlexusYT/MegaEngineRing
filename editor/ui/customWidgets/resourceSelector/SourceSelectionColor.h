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
// Created by alexus on 22.11.24.
//

#ifndef SOURCESELECTIONCOLOR_H
#define SOURCESELECTIONCOLOR_H
#include "SourceSelection.h"
#include "resources/TypedSourceSelectionResult.h"

namespace mer::sdk::main {
class ColorComponent;
}

namespace mer::editor::ui {

class SourceSelectionColor : public SourceSelection {
	Gtk::ColorDialogButton* picker;

	SourceSelectionColor() = default;

public:
	class Result : public TypedSourceSelectionResult<sdk::main::ColorComponent> {

		explicit Result(const std::shared_ptr<sdk::main::ColorComponent> &pResultObject)
			: TypedSourceSelectionResult(pResultObject) {}

	public:
		static std::shared_ptr<Result> create(const std::shared_ptr<sdk::main::ColorComponent> &pObject) {
			return std::shared_ptr<Result>(new Result(pObject));
		}

		bool isSame(const std::shared_ptr<ISourceSelectionResult> &pOther) override;
	};

	static std::shared_ptr<SourceSelectionColor> create();

	std::string getName() override { return "color"; }

	std::string getTitle() override { return "Color"; }

	Gtk::Widget* getWidget() override;

	void onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	bool isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) override;

	std::string getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) override;
};

} // namespace mer::editor::ui

#endif //SOURCESELECTIONCOLOR_H
