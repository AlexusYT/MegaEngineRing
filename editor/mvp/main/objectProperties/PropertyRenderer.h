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
// Created by alexus on 09.11.24.
//

#ifndef PROPERTYRENDERER_H
#define PROPERTYRENDERER_H
#include "EngineSDK/resources/IResourceLoadExecutor.h"
#include "EngineSDK/scene/IScene.h"

namespace mer::sdk::utils {
class PropertyBase;
}

namespace mer::editor::mvp {
enum class PropertyRendererType;

class PropertyRenderer {
	sdk::utils::PropertyBase* property;
	PropertyRendererType type;
	sdk::main::IResourceLoadExecutor* loader;

	PropertyRenderer(sdk::utils::PropertyBase* pProperty, sdk::main::IResourceLoadExecutor* pLoader,
					 PropertyRendererType pType);

public:
	static std::shared_ptr<PropertyRenderer> create(sdk::utils::PropertyBase* pProperty,
													sdk::main::IResourceLoadExecutor* pLoader);

	static std::shared_ptr<PropertyRenderer> create(
		sdk::utils::PropertyBase* pProperty, sdk::main::IResourceLoadExecutor* pLoader, PropertyRendererType pType);

	Gtk::Widget* getWidget() const;

private:
	static Gtk::SpinButton* createSpinButton(double pValue, double pMinValue = std::numeric_limits<double>::lowest(),
											 double pMaxValue = std::numeric_limits<double>::max());
};

} // namespace mer::editor::mvp

#endif //PROPERTYRENDERER_H
