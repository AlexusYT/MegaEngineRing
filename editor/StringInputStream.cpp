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
// Created by alexus on 05.11.24.
//

#include "StringInputStream.h"

namespace mer::editor::utils {
void StringInputStream::readAllAsync(
	const sigc::slot<void(const std::string &pResult, const std::exception_ptr &pException)> &pSlot) noexcept {
	//Prevent class instance from destroying from std::shared_ptr destructor while reading
	self = shared_from_this();
	const auto str = std::make_shared<std::string>();
	try {
		inputStream->read_bytes_async(readBufferSize,
									  sigc::bind(sigc::mem_fun(*this, &StringInputStream::readFromStream), pSlot, str));
	} catch (...) {
		try {
			pSlot("", std::current_exception());
		} catch (...) {
			//Allow destroying even if we received an exception
			self.reset();
		}
	}
}

void StringInputStream::readFromStream(
	const std::shared_ptr<Gio::AsyncResult> &pResult,
	const sigc::slot<void(const std::string &pResult, const std::exception_ptr &pException)> &pSlot,
	const std::shared_ptr<std::string> &pData) noexcept {
	try {
		auto bytes = inputStream->read_bytes_finish(pResult);
		gsize size = 0;
		const void* data = bytes->get_data(size);
		pData->append(static_cast<const char*>(data), size);
		if (size >= readBufferSize)
			inputStream->read_bytes_async(
				readBufferSize, sigc::bind(sigc::mem_fun(*this, &StringInputStream::readFromStream), pSlot, pData));
		else {
			try {
				pSlot(*pData, nullptr);
			} catch (...) {
				//Allow destroying even if we received an exception
				self.reset();
			}
		}
	} catch (...) {
		try {
			pSlot(*pData, std::current_exception());
		} catch (...) {
			//Allow destroying even if we received an exception
			self.reset();
		}
	}
}
} // namespace mer::editor::utils