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

#ifndef IRENDERINSTANCE_H
#define IRENDERINSTANCE_H

namespace sigc {
struct connection;
}

namespace ke {
class ShaderProgram;
struct MaterialData;
class RenderInstanceData;
class IInstancedRender;

class IRenderInstance {
public:
	virtual ~IRenderInstance() = default;

	virtual const RenderInstanceData &getRenderInstanceData() = 0;

	virtual std::shared_ptr<ShaderProgram> getShader() = 0;

	virtual void notifyDataChanged() = 0;

	[[nodiscard]] virtual IInstancedRender* getInstancedRender() const = 0;

	virtual void setInstancedRender(IInstancedRender* pInstancedRender) = 0;
};
} // namespace ke

#endif //IRENDERINSTANCE_H
