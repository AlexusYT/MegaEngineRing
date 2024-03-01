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

#ifndef ISSBO_H
#define ISSBO_H

#include <cstdint>

#include "BufferUsageEnum.h"

namespace mer::sdk::renderer {


class ISSBO {
public:
	virtual ~ISSBO() = default;

	virtual void setData(const void* pData, int64_t pSize, BufferUsageEnum pUsage) const = 0;

	virtual void bind() const = 0;

	virtual void unbind() const = 0;

	virtual uint32_t native() const = 0;

	virtual void bindBufferBase(uint32_t pBinding) = 0;

	virtual void bufferSubData(int32_t pOffset, int64_t pSize, const void* pData) = 0;
};
} // namespace mer::sdk::renderer


#endif //ISSBO_H
