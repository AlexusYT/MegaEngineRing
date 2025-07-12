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

#include "KwasarEngine/gltf/GltfModel.h"

#include "KwasarEngine/extensions/LightExtension.h"
#include "KwasarEngine/extensions/MeshExtension.h"
#include "KwasarEngine/gltf/Accessor.h"
#include "KwasarEngine/gltf/Image.h"
#include "KwasarEngine/gltf/Material.h"
#include "KwasarEngine/gltf/Mesh.h"
#include "KwasarEngine/gltf/Node.h"
#include "KwasarEngine/gltf/Primitive.h"
#include "KwasarEngine/gltf/Sampler.h"
#include "KwasarEngine/gltf/Texture.h"
#include "KwasarEngine/gltf/khr/KhrLightsPunctual.h"
#include "KwasarEngine/utils/Logger.h"
#include "GLTFSDK/Deserialize.h"
#include "GLTFSDK/ExtensionHandlers.h"
#include "GLTFSDK/ExtensionsKHR.h"
#include "GLTFSDK/GLBResourceReader.h"
#include "GLTFSDK/GLTFResourceReader.h"
#include "GLTFSDK/IStreamReader.h"

using namespace Microsoft::glTF;

class FileReader : public IStreamReader {
	std::filesystem::path pathBase;

public:
	FileReader(std::filesystem::path pPathBase) : pathBase(std::move(pPathBase)) {}

	// Resolves the relative URIs of any external resources declared in the glTF manifest
	std::shared_ptr<std::istream> GetInputStream(const std::string &pFilename) const override {
		auto streamPath = pathBase / std::filesystem::path(pFilename);
		auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

		if (!stream || !(*stream)) {
			throw std::runtime_error("Unable to create a valid input stream for uri: " + pFilename);
		}
		return stream;
	}
};

class StreamReader : public IStreamReader {
	std::shared_ptr<std::istream> stream;

public:
	StreamReader(const std::shared_ptr<std::istream> &pStream) : stream(pStream) {}

	std::shared_ptr<std::istream> GetInputStream(const std::string & /*pFilename*/) const override { return stream; }
};

namespace ke {
std::shared_ptr<GltfModel> GltfModel::createFromStream(const std::shared_ptr<std::istream> &pStream,
													   ReportMessagePtr &pErrorOut) {
	auto model = std::shared_ptr<GltfModel>(new GltfModel());
	if (const auto msg = model->loadFromStream(pStream)) {
		pErrorOut = msg;
		return nullptr;
	}
	return model;
}

std::shared_ptr<GltfModel> GltfModel::createFromFile(const std::filesystem::path &pFilePath,
													 ReportMessagePtr &pErrorOut) {
	auto model = std::shared_ptr<GltfModel>(new GltfModel());
	if (const auto msg = model->loadFromFile(pFilePath)) {
		pErrorOut = msg;
		return nullptr;
	}
	return model;
}

ReportMessagePtr GltfModel::loadFromStream(const std::shared_ptr<std::istream> &pStream) {
	auto streamReader = std::make_unique<StreamReader>(pStream);
	auto glbStream = streamReader->GetInputStream("");
	auto glbResourceReader = std::make_shared<GLBResourceReader>(std::move(streamReader), std::move(glbStream));

	std::string manifest = glbResourceReader->GetJson();

	std::shared_ptr<GLTFResourceReader> resourceReader = std::move(glbResourceReader);
	auto document = Document::create();
	if (auto msg = deserializeManifest(manifest, document)) return msg;
	if (auto msg = parseStructure(resourceReader, document)) return msg;
	return nullptr;
}

ReportMessagePtr GltfModel::loadFromFile(const std::filesystem::path &pFilePath) {
	using namespace std::string_literals;

	auto streamReader = std::make_unique<FileReader>(pFilePath.parent_path());

	std::filesystem::path pathFile = pFilePath.filename();
	std::string pathFileExt = pathFile.extension().string();

	std::string manifest;
	std::shared_ptr<GLTFResourceReader> resourceReader;
	try {
		if (pathFileExt == "."s + GLTF_EXTENSION) {
			auto gltfStream = streamReader->GetInputStream(pathFile.string());
			auto gltfResourceReader = std::make_shared<GLTFResourceReader>(std::move(streamReader));

			std::stringstream manifestStream;
			manifestStream << gltfStream->rdbuf();
			manifest = manifestStream.str();

			resourceReader = std::move(gltfResourceReader);
		} else if (pathFileExt == "."s + GLB_EXTENSION) {
			auto glbStream = streamReader->GetInputStream(pathFile.string());
			auto glbResourceReader = std::make_shared<GLBResourceReader>(std::move(streamReader), std::move(glbStream));

			manifest = glbResourceReader->GetJson(); // Get the manifest from the JSON chunk

			resourceReader = std::move(glbResourceReader);
		} else {
			auto msg = ReportMessage::create();
			msg->setTitle("GLTF Model loading failed");
			msg->setMessage("Wrong file extension. Must be .gltf or .glb");
			msg->addInfoLine("File: {}", pFilePath.string());
			msg->addInfoLine("Actual extension: {}", pathFileExt);
			return msg;
		}
	}
	catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("GLTF Model loading failed");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("File: {}", pFilePath.string());
		return msg;
	}
	auto document = Document::create();
	if (auto msg = deserializeManifest(manifest, document)) return msg;
	if (auto msg = parseStructure(resourceReader, document)) return msg;
	return nullptr;
}

ReportMessagePtr GltfModel::deserializeManifest(const std::string &pJson, std::shared_ptr<Document> &pDocumentOut) {
	try {
		auto deserializer = KHR::GetKHRExtensionDeserializer();
		KhrLightsPunctual::addHandlers(*deserializer);

		pDocumentOut = Deserializer::Deserialize(pJson, deserializer);
		return nullptr;
	}
	catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("GLTF Manifest deserialization failed");
		msg->setMessage("Exception occurred");
		return msg;
	}
}

ReportMessagePtr GltfModel::parseStructure(const std::shared_ptr<GLTFResourceReader> &pReader,
										   const std::shared_ptr<Document> &pDocument) {
	if (pDocument->IsExtensionUsed(KhrLightsPunctual::EXTENSION_NAME)) {
		auto &ext = pDocument->GetExtension<KhrLightsPunctual>();
		lights = ext.getLights();
	}
	for (auto &accessor: pDocument->accessors.Elements()) {
		accessors.emplace_back(Accessor::create(accessor, *pDocument, pReader));
	}
	for (auto &sampler: pDocument->samplers.Elements()) { samplers.emplace_back(Sampler::create(sampler)); }

	for (auto &image: pDocument->images.Elements()) {
		auto img = Image::create(image, *pDocument, pReader);
		if (auto msg = img->initialize()) return msg;
		images.emplace_back(img);
	}
	for (auto &texture: pDocument->textures.Elements()) {
		std::shared_ptr<Image> image{};
		//From spec: When undefined, an extension or other mechanism SHOULD supply an alternate texture source,
		//otherwise behavior is undefined.
		if (auto &textureId = texture.imageId; !textureId.empty()) {
			image = images.at(pDocument->images.GetIndex(textureId));
		} else {
			//TODO Add fallback texture.
		}
		std::shared_ptr<Sampler> sampler{};
		if (auto &samplerId = texture.samplerId; !samplerId.empty()) {
			sampler = samplers.at(pDocument->samplers.GetIndex(samplerId));
		}
		textures.emplace_back(Texture::create(texture, image, sampler));
	}

	//defaultScene = Scene3D::create();
	for (auto &material: pDocument->materials.Elements()) {
		std::unordered_map<TextureType, std::shared_ptr<Texture>> usedTextures;
		for (auto &texture: material.GetTextures()) {
			std::shared_ptr<Texture> tex{};
			if (auto &textureId = texture.first; !textureId.empty()) {
				tex = textures.at(pDocument->textures.GetIndex(textureId));
			}
			usedTextures.emplace(texture.second, tex);
		}
		auto mat = Material::create(material, usedTextures);
		materials.emplace_back(mat);
		//defaultScene->addMaterial(mat);
	}
	for (auto &meshData: pDocument->meshes.Elements()) {
		std::vector<std::shared_ptr<Primitive>> primitives;
		for (auto &primitive: meshData.primitives) {
			std::shared_ptr<Accessor> indexAccessor{};
			if (auto &accessorId = primitive.indicesAccessorId; !accessorId.empty())
				indexAccessor = accessors.at(pDocument->accessors.GetIndex(primitive.indicesAccessorId));

			std::shared_ptr<Accessor> positionAccessor{};
			if (auto iter = primitive.attributes.find("POSITION"); iter != primitive.attributes.end()) {
				auto index = pDocument->accessors.GetIndex(iter->second);
				positionAccessor = accessors.at(index);
			}
			auto prim = primitives.emplace_back(
				Primitive::create(static_cast<MeshMode>(primitive.mode),
								  positionAccessor, indexAccessor));

			if (auto &materialId = primitive.materialId; !materialId.empty())
				prim->setMaterial(materials.at(pDocument->materials.GetIndex(materialId)));

			std::unordered_map<std::string, std::shared_ptr<Accessor>> accessorsUsed;
			for (auto &attribute: primitive.attributes) {
				auto index = pDocument->accessors.GetIndex(attribute.second);
				const auto &accessor = accessors.at(index);
				if (attribute.first == "POSITION") continue;
				if (attribute.first == "TEXCOORD_0") {
					prim->setUvAccessor(accessor);
					continue;
				}
				if (attribute.first == "NORMAL") {
					prim->setNormalAccessor(accessor);
					continue;
				}
				if (attribute.first == "COLOR_0") {
					prim->setColorAccessor(accessor);
					continue;
				}
				accessorsUsed.emplace(attribute.first, accessor);
			}
			prim->setAccessors(accessorsUsed);
		}

		auto mesh = meshes.emplace_back(Mesh::create());
		mesh->setPrimitives(primitives);
		mesh->setName(meshData.name);
	}

	for (auto &element: pDocument->nodes.Elements()) {
		auto node = Node::create(element);
		if (!element.meshId.empty()) {
			auto mesh = meshes.at(pDocument->meshes.GetIndex(element.meshId));
			auto ext = MeshExtension::create();
			ext->mesh = mesh;
			node->addExtension(ext);
		} else if (element.HasExtension<KhrLightsPunctual::Node>()) {
			auto &ext = element.GetExtension<KhrLightsPunctual::Node>();

			auto lightExt = LightExtension::create();
			lightExt->lightDataId = ext.light;
			node->addExtension(lightExt);
		}
		nodes.emplace_back(node);
	}
	for (auto &element: pDocument->nodes.Elements()) {
		auto node = nodes.at(pDocument->nodes.GetIndex(element.id));
		for (auto &child: element.children) {
			auto childNode = nodes.at(pDocument->nodes.GetIndex(child));
			node->addChild(childNode.get());
			//defaultScene->addNode(node, childNode);
		}
	}
	auto &scene = pDocument->GetDefaultScene();
	for (auto &nodeId: scene.nodes) {
		auto node = nodes.at(pDocument->nodes.GetIndex(nodeId));
		//defaultScene->addNode(nullptr, node);
		rootNodes.emplace_back(node);
	}
	return nullptr;
}
} // namespace ke
