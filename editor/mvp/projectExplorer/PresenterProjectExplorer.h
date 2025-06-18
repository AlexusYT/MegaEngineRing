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

#ifndef PRESENTERPROJECTEXPLORER_H
#define PRESENTERPROJECTEXPLORER_H
#include "IPresenterProjectExplorer.h"

namespace ked {
class IModelProjectExplorer;
class IViewProjectExplorer;

class PresenterProjectExplorer : public IPresenterProjectExplorer {
	std::vector<std::shared_ptr<IViewProjectExplorer>> views;
	std::shared_ptr<IModelProjectExplorer> model;

public:
	explicit PresenterProjectExplorer(const std::shared_ptr<IModelProjectExplorer> &pModel);

	void createScene(const std::filesystem::path &pPathToFile) override;

	void openInFilesystem(const std::filesystem::path &pPathToFile) override;

	void deleteFile(const std::filesystem::path &pPathToDelete) override;

	void addView(const std::shared_ptr<IView> &pNewView) override;

	void removeView(const std::shared_ptr<IView> &pOldView) override;

	void run() override;

	void stop() override;

	std::string getTypeName() override { return "PresenterProjectExplorer"; }
};
} // namespace ked


#endif //PRESENTERPROJECTEXPLORER_H
