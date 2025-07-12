//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 24.02.25.
//

#ifndef GLTFMODEL_H
#define GLTFMODEL_H
#include "KwasarEngine/utils/ReportMessageFwd.h"
#include "GLTFSDK/Document.h"

namespace ke {
struct Light;
class Scene3D;
class Material;
class Texture;
class Image;
class Sampler;
} // namespace ke

namespace Microsoft::glTF {
class IStreamReader;
class GLTFResourceReader;
} // namespace Microsoft::glTF

namespace ke {
class Node;
class Mesh;
class Prefab;
class Primitive;
class Accessor;

class GltfModel {
	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Accessor>> accessors;
	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<std::shared_ptr<Sampler>> samplers;
	std::vector<std::shared_ptr<Image>> images;
	std::vector<std::shared_ptr<Texture>> textures;
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<std::shared_ptr<Node>> rootNodes;

protected:
	GltfModel() = default;

public:
	static std::shared_ptr<GltfModel> createFromStream(const std::shared_ptr<std::istream> &pStream,
													   ReportMessagePtr &pErrorOut);

	static std::shared_ptr<GltfModel> createFromFile(const std::filesystem::path &pFilePath,
													 ReportMessagePtr &pErrorOut);

	[[nodiscard]] const std::vector<std::shared_ptr<Mesh>> &getMeshes() const { return meshes; }

	[[nodiscard]] const std::vector<std::shared_ptr<Material>> &getMaterials() const { return materials; }

	[[nodiscard]] const std::vector<std::shared_ptr<Node>> &getNodes() const { return nodes; }

	[[nodiscard]] const std::vector<std::shared_ptr<Node>> &getRootNodes() const { return rootNodes; }

	[[nodiscard]] const std::vector<std::shared_ptr<Light>> &getLights() const { return lights; }

private:
	ReportMessagePtr loadFromStream(const std::shared_ptr<std::istream> &pStream);

	ReportMessagePtr loadFromFile(const std::filesystem::path &pFilePath);

	static ReportMessagePtr deserializeManifest(const std::string &pJson,
												std::shared_ptr<Microsoft::glTF::Document> &pDocumentOut);

	ReportMessagePtr parseStructure(const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pReader,
									const std::shared_ptr<Microsoft::glTF::Document> &pDocument);
};
} // namespace ke

#endif //GLTFMODEL_H
