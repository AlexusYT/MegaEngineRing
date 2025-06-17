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
// Created by alexus on 26.09.24.
//

#ifndef MODELOBJECTPROPERTIES_H
#define MODELOBJECTPROPERTIES_H

namespace ke {
class Scene3D;
}

namespace mer::editor::mvp {
class NodeSelectionHelper;
class IPresenterObjectProperties;

class IModelObjectProperties {
public:
	virtual ~IModelObjectProperties() = default;

	virtual void setPresenter(IPresenterObjectProperties* pPresenter) = 0;

	virtual void setScene(const std::shared_ptr<ke::Scene3D> &pScene) = 0;

	[[nodiscard]] virtual const std::shared_ptr<ke::Scene3D> &getScene() const = 0;
};

class ModelObjectProperties : public IModelObjectProperties {
	NodeSelectionHelper* selectionHelper{};
	std::shared_ptr<ke::Scene3D> scene{};
	IPresenterObjectProperties* presenter{};

public:
	explicit ModelObjectProperties(NodeSelectionHelper* pSelectionHelper);

	void setPresenter(IPresenterObjectProperties* pPresenter) override { presenter = pPresenter; }

	void setScene(const std::shared_ptr<ke::Scene3D> &pScene) override;

	[[nodiscard]] const std::shared_ptr<ke::Scene3D> &getScene() const override { return scene; }
};
} // namespace mer::editor::mvp

#endif //MODELOBJECTPROPERTIES_H
