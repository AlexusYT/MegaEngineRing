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
// Created by alexus on 25.01.24.
//

#ifndef PROJECTCREATINGTASK_H
#define PROJECTCREATINGTASK_H

namespace mer::editor::mvp {
struct ProjectCreatingTask {
	using MessageCallback = std::function<void(const std::string &pMessage)>;
	using TaskFunc = std::function<sdk::ReportMessagePtr(const MessageCallback &pCallback)>;

	TaskFunc function;
	std::string tabName;

	//void appendToBuffer(const Glib::ustring &pMarkup) const { buffer->insert_markup(buffer->end(), pMarkup + "\n"); }
};
} // namespace mer::editor::mvp


#endif //PROJECTCREATINGTASK_H
