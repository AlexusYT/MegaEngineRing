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

#ifndef FILESYSTEMRESOURCEBUNDLE_H
#define FILESYSTEMRESOURCEBUNDLE_H
#include <filesystem>
#include <vector>

#include "IResourceBundle.h"

namespace ke {
class FileSystemResourceBundle : public IResourceBundle {
	std::filesystem::path searchPath{};

protected:
	explicit FileSystemResourceBundle(const std::filesystem::path &pSearchPath) : searchPath(pSearchPath) {}

public:
	static std::shared_ptr<IResourceBundle> create(const std::filesystem::path &pSearchPath);

	ReportMessagePtr getResourceStream(const std::string &pResourceUri,
									   std::shared_ptr<std::istream> &pStream) override;

	void listResources(std::vector<std::string> &pUris) const override;
};
} // namespace ke

#endif //FSRESOURCEBUNDLE_H
