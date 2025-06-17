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
// Created by alexus on 26.03.24.
//

#ifndef MOUSEBUTTONEXTENSION_H
#define MOUSEBUTTONEXTENSION_H
#include "Extension.h"

namespace ke {
class MouseButtonExtension : public Extension {
public:
	using ButtonSignal = sigc::signal<void(MouseButton pButton, bool pPressed, double pX, double pY)>;

private:
	std::unordered_map<MouseButton, ButtonSignal> buttonHandlers;

public:
	METHOD_CREATE(MouseButtonExtension)

	EXT_TYPE_NAME("MouseButtonExtension")

	sigc::connection connectButtonSignal(MouseButton pButton, const ButtonSignal::slot_type &pSlot) {
		auto iter = buttonHandlers.find(pButton);
		if (iter == buttonHandlers.end()) iter = buttonHandlers.emplace_hint(iter, pButton, ButtonSignal());
		return iter->second.connect(pSlot);
	}

protected:
	void onMouseButtonStateChanged(const MouseButton pButton, const bool pPressed, const double pX,
								   const double pY) const override {
		if (const auto iter = buttonHandlers.find(pButton); iter != buttonHandlers.end()) {
			iter->second(pButton, pPressed, pX, pY);
		}
	}
};
} // namespace ke


#endif //MOUSEBUTTONEXTENSION_H
