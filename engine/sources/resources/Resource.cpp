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
// Created by alexus on 05.10.24.
//

#include "KwasarEngine/resources/Resource.h"

#include "KwasarEngine/utils/ReportMessage.h"
#include "KwasarEngine/utils/UUID.h"

namespace ke {
Resource::Resource() { uuid = UUID::create(); }

void Resource::addProperty(PropertyBase* pProperty) { properties.push_back(pProperty); }

void Resource::removeProperty(PropertyBase* pProperty) { erase(properties, pProperty); }

void Resource::propertyChanged(PropertyBase* /*pProperty*/) {}

void Resource::addReportInfo(const ReportMessagePtr &pMsg) const {
	pMsg->addInfoLine("Resource uri: {}", resourceUri.string());
	pMsg->addInfoLine("Resource UUID: {}", uuid->toString());
	pMsg->addInfoLine("Is resource inited: {}", isInited());
	pMsg->addInfoLine("Is resource incomplete: {}", incomplete);
}
} // namespace ke