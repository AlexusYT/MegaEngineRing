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
// Created by alexus on 09.07.24.
//

#ifndef PRESENTERGRAPHICSSCRIPTEDITOR_H
#define PRESENTERGRAPHICSSCRIPTEDITOR_H
#include "mvp/PresenterBase.h"

namespace mer::editor::mvp {
class IModelGraphicsScriptEditor;
class IViewGraphicsScriptEditor;

class PresenterGraphicsScriptEditor : public PresenterBase {
	std::shared_ptr<IViewGraphicsScriptEditor> viewGraphicsScriptEditor;
	std::shared_ptr<IModelGraphicsScriptEditor> modelGraphicsScriptEditor;
	sigc::signal<void(const std::string &pName)> tabHeaderChanged;
	glm::dvec2 prevElementPos{};
	bool moving{};

public:
	PresenterGraphicsScriptEditor(const std::shared_ptr<IViewGraphicsScriptEditor> &pViewGraphicsScriptEditor,
								  const std::shared_ptr<IModelGraphicsScriptEditor> &pModelGraphicsScriptEditor);

	void run() override;

	void stop() override;

	std::string getTypeName() override {
		return "PresenterGraphicsScriptEditor";
	}
};

} // namespace mer::editor::mvp

// mvp

#endif //PRESENTERGRAPHICSSCRIPTEDITOR_H
