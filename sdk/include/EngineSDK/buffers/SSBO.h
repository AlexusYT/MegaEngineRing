//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 04.02.24.
//

#ifndef SSBO_H
#define SSBO_H
#include "BufferUsageEnum.h"

namespace mer::sdk {

class SSBO {
	uint32_t name{};
	int64_t size{};
	const void* data{};
	BufferUsageEnum usage;
	uint32_t baseIndex{};


public:
	SSBO();

	~SSBO();

	void setData(const void* pData, int64_t pSize, BufferUsageEnum pUsage);

	void bind() const;

	void unbind() const;

	uint32_t native() const;

	void bindBufferBase(uint32_t pBinding);

	void unbindBufferBase(uint32_t pBinding);

	void bufferSubData(int32_t pOffset, int64_t pSize, const void* pData);

	void reallocate(int64_t pNewSize, const void* pNewData);

	[[nodiscard]] int64_t getSize() const { return size; }

	[[nodiscard]] const void* getData() const { return data; }

	void setUsage(const BufferUsageEnum pUsage) { usage = pUsage; }

	[[nodiscard]] const BufferUsageEnum &getUsage() const { return usage; }
};

} // namespace mer::sdk

#endif //SSBO_H
