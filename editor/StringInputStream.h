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

#ifndef STRINGINPUTSTREAM_H
#define STRINGINPUTSTREAM_H

namespace mer::editor::utils {

class StringInputStream : public std::enable_shared_from_this<StringInputStream> {
	std::shared_ptr<Gio::InputStream> inputStream{};
	uint64_t readBufferSize{};
	std::shared_ptr<StringInputStream> self{};

	explicit StringInputStream(const std::shared_ptr<Gio::InputStream> &pInputStream, const uint64_t pReadBufferSize)
		: inputStream(pInputStream), readBufferSize(pReadBufferSize) {}

public:
	static std::shared_ptr<StringInputStream> create(const std::shared_ptr<Gio::InputStream> &pInputStream,
													 const uint64_t pReadBufferSize = 4096) {
		return std::shared_ptr<StringInputStream>(new StringInputStream(pInputStream, pReadBufferSize));
	}

	[[nodiscard]] uint64_t getReadBufferSize() const { return readBufferSize; }

	void setReadBufferSize(const uint64_t pReadBufferSize) { readBufferSize = pReadBufferSize; }

	void readAllAsync(
		const sigc::slot<void(const std::string &pResult, const std::exception_ptr &pException)> &pSlot) noexcept;

private:
	void readFromStream(const std::shared_ptr<Gio::AsyncResult> &pResult,
						const sigc::slot<void(const std::string &pResult, const std::exception_ptr &pException)> &pSlot,
						const std::shared_ptr<std::string> &pData) noexcept;
};

} // namespace mer::editor::utils

#endif //STRINGINPUTSTREAM_H
