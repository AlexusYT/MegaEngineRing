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
// Created by alexus on 26.01.24.
//

#ifndef PRESENTERMAIN_H
#define PRESENTERMAIN_H
#include <mvp/IPresenter.h>

namespace mer::editor::mvp {
class IViewMain;
class IModelMain;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {

class PresenterMain : public IPresenter {
	std::shared_ptr<IViewMain> viewMain;
	std::shared_ptr<IModelMain> modelMain;

public:
	PresenterMain(const std::shared_ptr<IViewMain> &pViewMain, const std::shared_ptr<IModelMain> &pModelMain);

	~PresenterMain() override;

private:
	void generateCmakeFiles(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void build(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void run(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void logMessage(int pId, const std::string &pMessage) const;

	void logError(int pId, const std::string &pMessage) const;
};

} // namespace mer::editor::mvp

#endif //PRESENTERMAIN_H
