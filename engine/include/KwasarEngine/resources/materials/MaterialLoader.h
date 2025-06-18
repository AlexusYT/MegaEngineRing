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
// Created by alexus on 25.10.24.
//

#ifndef MATERIALLOADER_H
#define MATERIALLOADER_H
#include <sigc++/functors/slot.h>

#include "KwasarEngine/resources/ResourceLoader.h"

namespace ke {
class IMaterialComponent;
}

namespace ke {
class MaterialLoader : public ResourceLoader {
public:
	std::shared_ptr<IResource> createResource() override;

	ReportMessagePtr load(IResourceLoadExecutor* pLoadExecutor, std::shared_ptr<std::istream> &pStream,
						  const std::shared_ptr<IResource> &pResource) override;

	std::string getFileExtension() override { return "enmat"; }

private:
	static void readMaterialComponent(
		const std::shared_ptr<std::istream> &pStream, IResourceLoadExecutor* pLoadExecutor,
		const sigc::slot<void(const std::shared_ptr<IMaterialComponent> & pComponent)> &pSetter);
};
} // namespace ke

#endif //MATERIALLOADER_H