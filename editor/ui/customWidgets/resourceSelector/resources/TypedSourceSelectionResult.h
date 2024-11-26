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
// Created by alexus on 17.11.24.
//

#ifndef SOURCESELECTIONRESOURCERESULT_H
#define SOURCESELECTIONRESOURCERESULT_H
#include <memory>

#include "ui/customWidgets/resourceSelector/ISourceSelectionResult.h"

namespace mer::editor::ui {
template<typename T>
class TypedSourceSelectionResult : public ISourceSelectionResult {
	std::shared_ptr<T> resultObject;

protected:
	explicit TypedSourceSelectionResult(const std::shared_ptr<T> &pResultObject) { resultObject = pResultObject; }

public:
	using ResultT = T;

	static std::shared_ptr<TypedSourceSelectionResult> create(const std::shared_ptr<T> &pObject) {
		return std::shared_ptr<TypedSourceSelectionResult>(new TypedSourceSelectionResult(pObject));
	}

	bool isEmpty() const override { return resultObject == nullptr; }

	std::shared_ptr<T> getSelectionResult() { return resultObject; }

	bool isSame(const std::shared_ptr<ISourceSelectionResult> &pOther) override {
		auto other = std::dynamic_pointer_cast<TypedSourceSelectionResult>(pOther);
		if (!other) return false;
		return resultObject == other->resultObject;
	}
};
} // namespace mer::editor::ui

#endif //SOURCESELECTIONRESOURCERESULT_H
