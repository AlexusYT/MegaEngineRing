//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 01.10.24.
//

#ifndef MODEL3DRESOURCE_H
#define MODEL3DRESOURCE_H
#include <memory>
#include <string>
#include <unordered_map>

#include "KwasarEngine/resources/Resource.h"
#include "KwasarEngine/resources/ResourceType.h"
#include "IModel3DResource.h"

namespace ke {
class IModel3DObject;
class IRenderInstance;

class Model3DResource : public IModel3DResource, public Resource {
	std::unordered_map<std::string, std::shared_ptr<IModel3DObject>> modelObjects;

	Model3DResource() = default;

public:
	static std::shared_ptr<Model3DResource> create();

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

public:
	void render() override;


	void addModelObject(const std::shared_ptr<IModel3DObject> &pObject) override;

	void removeModelObject(const std::shared_ptr<IModel3DObject> &pObject) override;

	void removeModelObject(const std::string &pName) override;

	std::shared_ptr<IModel3DObject> getModelObject(const std::string &pName) override;

	[[nodiscard]] const std::unordered_map<
		std::string, std::shared_ptr<IModel3DObject>> &getModelObjects() const override { return modelObjects; }

	IResource* asResource() override { return this; }

	ResourceType getResourceType() override { return ResourceType::MODEL; }
};
} // namespace ke

#endif //MODEL3DRESOURCE_H