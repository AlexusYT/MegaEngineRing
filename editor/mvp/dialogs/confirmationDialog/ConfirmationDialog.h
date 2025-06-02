//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 14.02.25.
//

#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H
#include "EngineSDK/ui/UiPopup.h"

namespace mer::editor::mvp {

class ConfirmationDialog : public sdk::UiPopup {

	std::string message;
	std::vector<std::string> buttons;

	std::function<void(int pId)> buttonClickedCallback;

	ConfirmationDialog(const std::string &pName, const std::string &pTitle) : UiPopup(pName, pTitle) {}

public:
	static std::shared_ptr<ConfirmationDialog> create(const std::string &pName, const std::string &pTitle);

	void onUpdate(bool pVisible) override;

	[[nodiscard]] const std::string &getMessage() const { return message; }

	void setMessage(const std::string &pMessage) { message = pMessage; }

	[[nodiscard]] const std::vector<std::string> &getButtons() const { return buttons; }

	void setButtons(const std::vector<std::string> &pButtons) { buttons = pButtons; }

	[[nodiscard]] const std::function<void(int pId)> &getButtonClickedCallback() const { return buttonClickedCallback; }

	void setButtonClickedCallback(const std::function<void(int pId)> &pButtonClickedCallback) {
		buttonClickedCallback = pButtonClickedCallback;
	}
};

} // namespace mer::editor::mvp

#endif //CONFIRMATIONDIALOG_H
