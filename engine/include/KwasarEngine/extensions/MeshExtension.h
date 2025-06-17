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
// Created by alexus on 29.03.24.
//

#ifndef MODELRENDEREXTENSION_H
#define MODELRENDEREXTENSION_H

#include "KwasarEngine/gltf/Mesh.h"
#include "Extension.h"

namespace ke {
class Mesh;

class MeshExtension : public Extension {
	sigc::scoped_connection aabbChangedConnection;

protected:
	MeshExtension();

public:
	ExtensionProperty<std::shared_ptr<Mesh>> mesh;

	~MeshExtension() override;

	METHOD_CREATE(MeshExtension)

	EXT_TYPE_NAME (
	"MeshExtension"
	)

	bool isGeometryIntersects(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, glm::vec2 &pIntersectsAt,
							  float &pIntersectDistance) const;

	void onNodeChanged(Node* pNode) override;
};
} // namespace ke


#endif //MODELRENDEREXTENSION_H