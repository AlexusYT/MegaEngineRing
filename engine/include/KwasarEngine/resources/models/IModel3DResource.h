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
// Created by alexus on 02.10.24.
//

#ifndef IMODEL3DRESOURCE_H
#define IMODEL3DRESOURCE_H

#include "KwasarEngine/render/IRenderable.h"

namespace ke {
class IModel3DObject;
class IResource;

class IModel3DResource : public IRenderable {
public:
	~IModel3DResource() override = default;

	virtual std::shared_ptr<IModel3DObject> getModelObject(const std::string &pObjectName) = 0;

	virtual void addModelObject(const std::shared_ptr<IModel3DObject> &pObject) = 0;

	virtual void removeModelObject(const std::shared_ptr<IModel3DObject> &pObject) = 0;

	virtual void removeModelObject(const std::string &pName) = 0;

	[[nodiscard]] virtual const std::unordered_map<
		std::string, std::shared_ptr<IModel3DObject>> &getModelObjects() const = 0;

	virtual IResource* asResource() = 0;
};
} // namespace ke

#endif //IMODEL3DRESOURCE_H