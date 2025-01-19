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

#ifndef OPTION_H
#define OPTION_H

namespace mer::editor::utils {

class Option : public Glib::OptionEntry {
public:
	Option(Glib::OptionGroup* pOptionGroup, const Glib::ustring &pLongName, const Glib::ustring &pDescription) {

		set_long_name(pLongName);
		set_description(pDescription);
		pOptionGroup->add_entry(*this, sigc::mem_fun(*this, &Option::onOptionParsed));
	}

	~Option() override = default;

	virtual bool onOptionParsed(const Glib::ustring &pOptionName, const Glib::ustring &pValue, bool pHasValue);
};

} // namespace mer::editor::utils

#endif //OPTION_H
