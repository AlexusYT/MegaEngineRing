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
// Created by alexus on 01.10.24.
//

#ifndef FILERESOURCEREADER_H
#define FILERESOURCEREADER_H
#include <fstream>

namespace mer::editor::mvp {

class FileResourceReader {
	std::filesystem::path path;

	std::shared_ptr<std::ifstream> stream;

public:
	explicit FileResourceReader(const std::filesystem::path &pPath) : path(pPath) {}

	virtual ~FileResourceReader() = default;


	virtual sdk::ReportMessagePtr checkType() = 0;

	[[nodiscard]] const std::filesystem::path &getPath() const { return path; }

	[[nodiscard]] const std::shared_ptr<std::ifstream> &getStream() const { return stream; }

protected:
	sdk::ReportMessagePtr open() {
		if (stream) return nullptr;

		std::shared_ptr<std::ifstream> file = std::make_shared<std::ifstream>();
		try {
			file->exceptions(std::_S_badbit | std::_S_failbit);
			file->open(path);
		} catch (...) {
			sdk::ReportMessagePtr msg = sdk::ReportMessage::create();
			msg->setStacktrace();
			msg->setTitle("Failed to open file");
			msg->setMessage("Exception thrown");
			msg->addInfoLine("File path: {}", absolute(path).string());
			return msg;
		}
		stream = file;
		return nullptr;
	}
};

} // namespace mer::editor::mvp

#endif //FILERESOURCEREADER_H
