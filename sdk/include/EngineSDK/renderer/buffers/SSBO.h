//
// Created by alexus on 04.02.24.
//

#ifndef SSBO_H
#define SSBO_H

#include "ISSBO.h"

namespace mer::sdk::renderer {

class SSBO : public ISSBO {
public:
	SSBO();

	~SSBO() override;

	void setData(const void* pData, uint64_t pSize, BufferUsageEnum pUsage) const override;

	void bind() const override;

	void unbind() const override;

	uint32_t native() const override;

	void bindBufferBase(uint32_t pBinding) override;
};

} // namespace mer::sdk::renderer

#endif //SSBO_H
