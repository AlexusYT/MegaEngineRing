//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 21.02.25.
//

#ifndef SKETCHFABCACHE_H
#define SKETCHFABCACHE_H

namespace mer::editor::mvp {
class SketchfabCache {
	std::filesystem::path pathToCache;
	std::mutex mutex;

public:
	SketchfabCache(const std::filesystem::path &pPath);

	ke::ReportMessagePtr saveCache(const std::string &pUrl, const std::vector<unsigned char> &pData);

	ke::ReportMessagePtr saveCache(const std::string &pUrl, const std::ostream &pData);

	ke::ReportMessagePtr loadCache(const std::string &pUrl, std::vector<unsigned char> &pData);

	ke::ReportMessagePtr loadCache(const std::string &pUrl, std::shared_ptr<std::iostream> &pData);

	bool isCached(const std::string &pUrl) const;

	ke::ReportMessagePtr buildPath(const std::string &pUrl, std::filesystem::path &pPathOut) const;
};
} // namespace mer::editor::mvp

#endif //SKETCHFABCACHE_H
