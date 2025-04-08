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
// Created by alexus on 07.04.25.
//

#ifndef BASICBUFFER_H
#define BASICBUFFER_H
#include <mutex>

#include "BufferUsageEnum.h"

namespace mer::sdk {


class BasicBuffer {
protected:
	uint32_t name{};
	BufferUsageEnum usage = DYNAMIC_DRAW;
	int64_t allocatedSize{};
	std::recursive_mutex mutex;
	bool dirty{};

	BasicBuffer() = default;

	virtual ~BasicBuffer();

	void reallocate(int64_t pNewSize, const void* pNewData);

	void bufferSubData(int32_t pOffset, int64_t pSize, const void* pData) const;

public:
	/**
	 * @brief Update the buffer. It will wait until the mutex is unlocked when updating required.
	 * @see tryUpdate() for a non-blocking version of this.
	 */
	void update() {
		if (!dirty) return;

		std::lock_guard lock(mutex);
		updateImpl();
		dirty = false;
	}

	/**
	 * @brief Try to update the buffer.
	 * @return @a true if updated or update is not required, @a false if mutex locked, and buffer is not updated.
	 * @see update() for a blocking version of this.
	 */
	bool tryUpdate() {
		if (!dirty) return true;
		if (mutex.try_lock()) {
			updateImpl();
			dirty = false;
			return true;
		}
		return false;
	}

	/**
	 * @brief Get type components count. For example, glm::vec3 has 3 float components, glm::vec2 - 2 floats and so on.
	 * @return
	 */
	virtual size_t getTypeComponentsCount() const { return 1; }

	virtual size_t getElementSize() const = 0;

	virtual size_t getBytesCount() const = 0;

	virtual const void* getBytes() const = 0;

	void unintialize();

	uint32_t native() const { return name; }

	void setUsage(const BufferUsageEnum pUsage) { usage = pUsage; }

	[[nodiscard]] const BufferUsageEnum &getUsage() const { return usage; }

	[[nodiscard]] bool isDirty() const { return dirty; }

private:
	void updateImpl() {

		if (int64_t dataSize = static_cast<int64_t>(getBytesCount()); allocatedSize < dataSize) {
			reallocate(dataSize, getBytes());
		} else
			bufferSubData(0, dataSize, getBytes());
	}
};


} // namespace mer::sdk

#endif //BASICBUFFER_H
