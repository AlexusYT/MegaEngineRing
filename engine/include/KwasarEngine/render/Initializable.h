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
// Created by alexus on 10.11.24.
//

#ifndef INITIALIZABLE_H
#define INITIALIZABLE_H
#include "IInitializable.h"

namespace ke {
class Initializable : public virtual IInitializable {
	bool inited{false};

public:
	ReportMessagePtr initialize() final {
		if (inited) return nullptr;
		if (auto msg = onInitialize()) return msg;
		inited = true;
		return nullptr;
	}

	void uninitialize() final {
		if (!inited) return;
		onUninitialize();
		inited = false;
	}

	[[nodiscard]] bool isInited() const override { return inited; }

	void setInited(bool pInited) override { inited = pInited; }

protected:
	virtual ReportMessagePtr onInitialize() { return nullptr; }

	virtual void onUninitialize() {}
};
} // namespace ke

#endif //INITIALIZABLE_H
