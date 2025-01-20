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

#ifndef TOOLCHAINUTILS_H
#define TOOLCHAINUTILS_H

namespace mer::editor::project {
class ToolchainUtils {
public:
	static void execute(const std::filesystem::path &pRootPath, const std::filesystem::path &path,
						const std::string &pArgs, const sigc::slot<void(const std::string &log)> &pCoutCallback,
						const sigc::slot<void(const std::string &log)> &pCerrCallback,
						const sigc::slot<void(int pReturnValue)> &pOnFinish = {});
	static int executeSync(const std::filesystem::path &pRootPath, const std::filesystem::path &pPath,
						   const std::string &pArgs, const sigc::slot<void(const std::string &pLog)> &pCoutCallback,
						   const sigc::slot<void(const std::string &pLog)> &pCerrCallback);

	static sdk::ReportMessagePtr generateCMakeListsFile(const std::filesystem::path &pPath);

	static sdk::ReportMessagePtr generateCMakePresetsFile(const std::filesystem::path &pPath);

	static sdk::ReportMessagePtr generateCMakeUserPresetsFile(const std::filesystem::path &pPath);

	static sdk::ReportMessagePtr generateVcpkgManifestFile(const std::filesystem::path &pPath);

	static sdk::ReportMessagePtr generateCMakeDirectory(const std::filesystem::path &pPath);

	static sdk::ReportMessagePtr writeFile(const std::filesystem::path &pPath, const std::string &pContents);
};
} // namespace mer::editor::project

#endif //TOOLCHAINUTILS_H
