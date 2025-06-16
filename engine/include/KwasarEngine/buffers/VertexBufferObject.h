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

#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H
#include <vector>

#include "BasicBuffer.h"

namespace mer::sdk {
class VertexBufferObjectImpl : public BasicBuffer {
public:
	void bindVertexArray(uint32_t pVao, uint32_t pBindingIndex, uint32_t pOffest, int32_t pStride) const;

	void bindVertexArray(uint32_t pVao, uint32_t pBindingIndex, uint32_t pOffest = 0) const;
};

template <typename T, int N>
class VertexBufferObject : public VertexBufferObjectImpl {
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

	void clear() {
		data.clear();
		dirty = true;
	}

	size_t size() const { return data.size(); }

	bool empty() const { return data.empty(); }

	size_t getTypeComponentsCount() const { return N; }

	size_t getElementSize() const override { return sizeof(T); }

	size_t getBytesCount() const override { return data.size() * getElementSize(); }

	const void* getBytes() const override { return data.data(); }
};
} // namespace mer::sdk

#endif //VERTEXBUFFEROBJECT_H
