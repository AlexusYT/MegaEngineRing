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
// Created by alexus on 08.05.2025.
//

#ifndef EUTILS_H
#define EUTILS_H


struct curl_slist;

namespace mer::editor {
class Utils {
public:
	static std::unique_ptr<curl_slist, void (*)(curl_slist*)> getCurlList(const std::list<std::string> &pList);

	static size_t streamWriteCallback(const char* pPtr, const size_t pSize, const size_t pNmemb,
									  std::stringstream* pUserData) {
		size_t realsize = pSize * pNmemb;
		pUserData->write(pPtr, static_cast<int64_t>(realsize));
		return realsize;
	}

	static size_t rawDataWriteCallback(const char* pPtr, const size_t pSize, const size_t pNmemb,
									   std::vector<unsigned char>* pData) {
		std::string str(pPtr, pSize * pNmemb);
		pData->insert(pData->cend(), str.begin(), str.end());
		return pSize * pNmemb;
	}
};
} // namespace mer::editor

#endif //EUTILS_H
