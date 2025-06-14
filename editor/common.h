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
// Created by alexus on 06.12.23.
//

#ifndef COMMON_H
#define COMMON_H

#define JSON_DISABLE_ENUM_SERIALIZATION 1
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <EngineUtils/utils/Logger.h>
#include <EngineUtils/utils/ReportMessage.h>
#include <I18N.h>
#include <filesystem>
#include <fstream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <list>
#include <ranges>
#include <sigc++/sigc++.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <libintl.h>
#ifdef tr
#undef tr
#endif
#define tr(__TEXT__) gettext((__TEXT__))
#define trp(__TEXT1__, __TEXT2__, __NUM__) ngettext((__TEXT1__), (__TEXT2__), (__NUM__))
#define trs(__TEXT__) std::string(gettext(__TEXT__))


#endif //COMMON_H
