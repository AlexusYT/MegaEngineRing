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
// Created by alexus on 23.09.24.
//

#ifndef PROJECTEXPLORERELEMENT_H
#define PROJECTEXPLORERELEMENT_H

namespace ked {
enum class ExplorerElementType {
	NONE = 0,
	DIRECTORY,
	SCENE,
	RESOURCE_MODEL,
	RESOURCE_MODEL_OBJECT,
	RESOURCE_TEXTURE,
	RESOURCE_MATERIAL,
};

class ProjectExplorerElement {
	std::vector<std::shared_ptr<ProjectExplorerElement>> children;
	bool directory{};
	ProjectExplorerElement* parent{};
	std::filesystem::path path{};
	std::filesystem::path rootPath{};
	ExplorerElementType type;

public:
	ProjectExplorerElement(const std::filesystem::path &pPath, const std::filesystem::path &pRootPath, bool pDirectory);

	static std::shared_ptr<ProjectExplorerElement> create(
		const std::filesystem::path &pPath, const std::filesystem::path &pRootPath, const bool pDirectory) {
		return std::shared_ptr<ProjectExplorerElement>(new ProjectExplorerElement(pPath, pRootPath, pDirectory));
	}

	void addChild(const std::shared_ptr<ProjectExplorerElement> &pChild);

	void removeChild(const std::shared_ptr<ProjectExplorerElement> &pChild) const;

	void sort() const;

	[[nodiscard]] ExplorerElementType getType() const { return type; }

	const char* getTypeStr() const {
		switch (type) {
			case ExplorerElementType::DIRECTORY: return "Folder";
			case ExplorerElementType::SCENE: return "Scene";
			case ExplorerElementType::RESOURCE_MODEL: return "Model";
			case ExplorerElementType::RESOURCE_TEXTURE: return "Texture";
			case ExplorerElementType::RESOURCE_MATERIAL: return "Material";
			default: return "File";
		}
	}

	[[nodiscard]] bool isDirectory() const { return directory; }

	[[nodiscard]] ProjectExplorerElement* getParent() const { return parent; }

	[[nodiscard]] std::string getName() const { return path.stem().string(); }

	[[nodiscard]] std::filesystem::path getRelativePath() const { return relative(path, rootPath); }

	[[nodiscard]] const std::filesystem::path &getPath() const { return path; }

	void setPath(const std::filesystem::path &pPath) { path = pPath; }

	[[nodiscard]] const std::vector<std::shared_ptr<ProjectExplorerElement>> &getChildren() const { return children; }
};
} // namespace ked


#endif //PROJECTEXPLORERELEMENT_H
