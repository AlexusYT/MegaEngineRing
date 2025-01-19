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
// Created by alexus on 31.12.23.
//

#ifndef ENGINEOPTIONGROUP_H
#define ENGINEOPTIONGROUP_H
#include "options/Option.h"

namespace mer::editor::utils {


class EngineOptionGroup final : public Glib::OptionGroup {
	std::vector<std::unique_ptr<Option>> optionEntries{};

public:
	EngineOptionGroup();

	explicit EngineOptionGroup(GOptionGroup* const pCastitem) : OptionGroup(pCastitem) {}

	explicit EngineOptionGroup(const OptionGroup &pOther) = delete;

	explicit EngineOptionGroup(OptionGroup &&pOther) = delete;

	~EngineOptionGroup() override;

protected:
	bool on_pre_parse(Glib::OptionContext &pContext) override;
	bool on_post_parse(Glib::OptionContext &pContext) override;
	void on_error(Glib::OptionContext &pContext, const Glib::Error &pError) override;
};
} // namespace CORE_UTILS_NAMESPACE


#endif //ENGINEOPTIONGROUP_H
