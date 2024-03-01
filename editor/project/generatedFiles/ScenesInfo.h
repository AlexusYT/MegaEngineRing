// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 27.01.24.
//

#ifndef SCENESINFO_H
#define SCENESINFO_H
#include "GeneratedFileEntry.h"
#include "SceneInfo.h"

namespace mer::editor::project {
class ScenesInfo : public GeneratedFileEntry {


protected:
	explicit ScenesInfo(const std::shared_ptr<Project> &pProject);

public:
	static std::shared_ptr<ScenesInfo> create(const std::shared_ptr<Project> &pProject) {

		return Glib::make_refptr_for_instance(new ScenesInfo(pProject));
	}

	void getActionGroup(const Glib::RefPtr<Gio::SimpleActionGroup> &pActionGroup);

private:
	sdk::utils::ReportMessagePtr onLoadDatabase() override;

	sdk::utils::ReportMessagePtr onSaveDatabase() const override;

	sdk::utils::ReportMessagePtr createTable() const;
};
} // namespace mer::editor::project


#endif //SCENESINFO_H
