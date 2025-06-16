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
#include "BasicBuffer.h"

namespace mer::sdk {
class SsboImpl : public BasicBuffer {
protected:
	SsboImpl() = default;

public:
	void bindBufferBase(uint32_t pBinding) const;

	void unbindBufferBase(uint32_t pBinding);
};

template <typename T>
class Ssbo : public SsboImpl {
	T data{};

public:
	using ElementT = T;

	void setData(const T &pData) {
		std::lock_guard lock(mutex);
		data = pData;
		dirty = true;
	}

	[[nodiscard]] const T &getData() const { return data; }

	size_t getElementSize() const override { return sizeof(T); }

	size_t getBytesCount() const override { return getElementSize(); }

	const void* getBytes() const override { return data.data(); }
};

template <typename T>
class Ssbo<std::vector<T>> : public SsboImpl {
	std::vector<T> data{};

public:
	using ElementT = T;
	using VectorT = std::vector<T>;

	void addElement(const T &pElement) {
		std::lock_guard lock(mutex);
		data.push_back(pElement);
		dirty = true;
	}

	void setElement(size_t pIndex, const T &pElement) {
		std::lock_guard lock(mutex);
		data.at(pIndex) = pElement;
		dirty = true;
	}

	template <typename InputIterator>
	InputIterator find(InputIterator pFirst, InputIterator pLast, const T &pVal) {
		std::lock_guard lock(mutex);
		return std::find(pFirst, pLast, pVal);
	}

	/**
	 * @brief Get an element at specified @a pIndex.
	 * @warning You must call markDirty() method if you are changed the data.
	 * @param pIndex index of an element to return. Must be less than Ssbo::size().
	 * @return reference to an element
	 * @throw std::out_of_range  If @a pIndex is an invalid index.
	 */
	typename std::vector<T>::reference getElement(size_t pIndex) {
		std::lock_guard lock(mutex);
		return data.at(pIndex);
	}

	/**
	 * @brief Get an element at specified @a pIndex.
	 * @param pIndex index of an element to return. Must be less than Ssbo::size().
	 * @return const reference to an element
	 * @throw std::out_of_range  If @a pIndex is an invalid index.
	 */
	typename std::vector<T>::reference getElement(size_t pIndex) const {
		std::lock_guard lock(mutex);
		return data.at(pIndex);
	}

	template <typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
	typename std::vector<T>::iterator insertElements(typename std::vector<T>::const_iterator pPosition,
													 InputIterator pFirst, InputIterator pLast) {
		std::lock_guard lock(mutex);
		const auto &res = data.insert(pPosition, pFirst, pLast);
		dirty = true;
		return res;
	}

	typename std::vector<T>::iterator insertElement(typename std::vector<T>::const_iterator pPosition, T pElement) {
		std::lock_guard lock(mutex);
		const auto &res = data.insert(pPosition, pElement);
		dirty = true;
		return res;
	}

	typename std::vector<T>::iterator insertElement(typename std::vector<T>::const_iterator pPosition,
													const T &pElement) {
		std::lock_guard lock(mutex);
		const auto &res = data.insert(pPosition, pElement);
		dirty = true;
		return res;
	}

	typename std::vector<T>::iterator removeElement(const typename std::vector<T>::const_iterator &pBeginIter,
													const typename std::vector<T>::const_iterator &pEndIter) {
		std::lock_guard lock(mutex);
		auto &res = data.erase(pBeginIter, pEndIter);
		dirty = true;
		return res;
	}

	typename std::vector<T>::iterator removeElement(const typename std::vector<T>::const_iterator &pPosition) {
		std::lock_guard lock(mutex);
		auto res = data.erase(pPosition);
		dirty = true;
		return res;
	}

	typename std::vector<T>::const_iterator begin() const { return data.begin(); }

	typename std::vector<T>::const_iterator end() const { return data.end(); }

	typename std::vector<T>::iterator begin() { return data.begin(); }

	typename std::vector<T>::iterator end() { return data.end(); }

	void clear() {
		data.clear();
		dirty = true;
	}

	size_t size() const { return data.size(); }

	bool empty() const { return data.empty(); }

	size_t getElementSize() const override { return sizeof(T); }

	size_t getBytesCount() const override { return data.size() * getElementSize(); }

	const void* getBytes() const override { return data.data(); }

	/*typename std::vector<T>::iterator begin() { return data.begin(); }

	typename std::vector<T>::iterator end() { return data.end(); }*/
};

class SSBO {
	uint32_t name{};
	int64_t size{};
	const void* data{};
	BufferUsageEnum usage;

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
