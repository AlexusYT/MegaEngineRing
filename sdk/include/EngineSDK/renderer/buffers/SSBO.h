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
