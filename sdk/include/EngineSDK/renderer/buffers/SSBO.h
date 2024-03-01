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
// Created by alexus on 04.02.24.
//

#ifndef SSBO_H
#define SSBO_H

#include "ISSBO.h"

namespace mer::sdk::renderer {

class SSBO : public ISSBO {
	uint32_t name;

public:
	SSBO();

	~SSBO() override;

	void setData(const void* pData, int64_t pSize, BufferUsageEnum pUsage) const override;

	void bind() const override;

	void unbind() const override;

	uint32_t native() const override;

	void bindBufferBase(uint32_t pBinding) override;

	virtual void bufferSubData(int32_t pOffset, int64_t pSize, const void* pData);
};

} // namespace mer::sdk::renderer

#endif //SSBO_H
