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
// Created by alexus on 11.12.23.
//

#ifndef TOOLCHAINSETTINGS_H
#define TOOLCHAINSETTINGS_H

namespace mer::editor::project {
class ToolchainSettings {
	inline static std::filesystem::path cmakePath = "/usr/bin/cmake";
	inline static std::filesystem::path vcpkgPath = "/home/alexus/.clion-vcpkg/vcpkg/vcpkg";

public:
	static void setCMakePath(const std::filesystem::path &pNewPath) { cmakePath = pNewPath; }

	[[nodiscard]] static const std::filesystem::path &getCMakePath() { return cmakePath; }

	[[nodiscard]] static const std::filesystem::path &getVcpkgPath() { return vcpkgPath; }

	static void setVcpkgPath(const std::filesystem::path &pVcpkgPath) { vcpkgPath = pVcpkgPath; }
};
} // namespace mer::editor::project


#endif //TOOLCHAINSETTINGS_H
