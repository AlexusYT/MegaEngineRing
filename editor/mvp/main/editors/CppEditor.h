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
// Created by alexus on 20.01.24.
//

#ifndef CPPEDITOR_H
#define CPPEDITOR_H

#include <mvp/main/editors/IEditorPresenter.h>
#include <thread>

namespace mer::editor::mvp {
class CppEditor : public IEditorPresenter {
	std::filesystem::path filePath;
	Gtk::TextView mainTextView;

public:
	explicit CppEditor(const std::filesystem::path &pFilePath) : filePath(pFilePath) {


		/*std::thread thread([](const std::filesystem::path &pFilePath) {}, filePath);
		thread.detach();*/
	}

	operator Gtk::Widget&() override { return mainTextView; }

	sigc::connection connectTabHeaderChanged(const sigc::slot<void(const std::string &pName)> &/*pSlot*/) override {
		return {};
	}
};
} // namespace mer::editor::mvp
#endif //CPPEDITOR_H
