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
// Created by alexus on 08.05.2025.
//

#include "Utils.h"

#include <curl/curl.h>

namespace ked {
std::unique_ptr<curl_slist, void (*)(curl_slist*)> Utils::getCurlList(const std::list<std::string> &pList) {
	curl_slist* slistTmp{};
	for (auto &str: pList) {
		if (curl_slist* tmp = curl_slist_append(slistTmp, str.c_str())) { slistTmp = tmp; } else
			break;
	}
	return std::unique_ptr<curl_slist, void (*)(curl_slist*)>(slistTmp, curl_slist_free_all);
}
} // namespace ked
