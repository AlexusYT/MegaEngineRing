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

	virtual void setData(const void* pData, uint64_t pSize, BufferUsageEnum pUsage) const = 0;

	virtual void bind() const = 0;

	virtual void unbind() const = 0;

	virtual uint32_t native() const = 0;

	virtual void bindBufferBase(uint32_t pBinding) = 0;
};
} // namespace mer::sdk::renderer


#endif //ISSBO_H
