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
// Created by alexus on 12.08.24.
//

#ifndef IPRESENTERPROJECTEXPLORER_H
#define IPRESENTERPROJECTEXPLORER_H
#include "mvp/IPresenter.h"

namespace ked {}

namespace ked {
class IPresenterProjectExplorer : public IPresenter {
public:
	virtual void createScene(const std::filesystem::path &pPathToFile) = 0;

	virtual void openInFilesystem(const std::filesystem::path &pPathToFile) = 0;

	virtual void deleteFile(const std::filesystem::path &pPathToDelete) = 0;
};
} // namespace ked


#endif //IPRESENTERPROJECTEXPLORER_H
