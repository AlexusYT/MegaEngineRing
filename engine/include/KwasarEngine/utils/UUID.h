//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 08.01.24.
//

#ifndef UUID_H
#define UUID_H
#include <chrono>
#include <cstdint>
#include <format>
#include <random>
#include <string>
#include <thread>

#include "ReportMessageFwd.h"

namespace ke {
class UUID {
	friend struct std::hash<UUID>;

	inline static std::random_device rd;
	uint32_t timeLow = 0;
	uint16_t timeMid = 0;
	uint16_t timeHiVersion = 0;
	uint16_t varClockSeq = 0;
	uint32_t node1 = 0;
	uint16_t node2 = 0;

public:
	static std::shared_ptr<UUID> create() {
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
		static std::uniform_int_distribution<uint16_t> dis2(0, 0xFFFF);
		const auto time = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());

		using namespace std::chrono_literals;
		auto uuid = std::make_shared<UUID>();
		uuid->timeLow = time & 0xFFFFFFFF;
		uuid->timeMid = (time >> 32) & 0xFFFF;
		uuid->timeHiVersion = static_cast<uint16_t>(time >> 48);
		uuid->varClockSeq = dis2(gen);
		uuid->node1 = dis(gen);
		uuid->node2 = dis2(gen);
		return uuid;
	}

	static UUID newInstance() {
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
		static std::uniform_int_distribution<uint16_t> dis2(0, 0xFFFF);
		const auto time = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());

		using namespace std::chrono_literals;
		UUID uuid;
		uuid.timeLow = time & 0xFFFFFFFF;
		uuid.timeMid = (time >> 32) & 0xFFFF;
		uuid.timeHiVersion = static_cast<uint16_t>(time >> 48);
		uuid.varClockSeq = dis2(gen);
		uuid.node1 = dis(gen);
		uuid.node2 = dis2(gen);
		return uuid;
	}

	static UUID getNull() {
		static UUID uuid;
		return uuid;
	}

	static std::shared_ptr<UUID> parse(const std::string &pString) {
		ReportMessagePtr error;
		return parse(pString, error);
	}

	static std::shared_ptr<UUID> parse(const std::string &pString, ReportMessagePtr &pError);

	constexpr std::string toString(const bool pUpperCase = false) const {
		if (pUpperCase) {
			return std::format("{:0>8X}-{:0>4X}-{:0>4X}-{:0>4X}-{:0>8X}{:0>4X}", timeLow, timeMid, timeHiVersion,
							   varClockSeq, node1, node2);
		}
		return std::format("{:0>8x}-{:0>4x}-{:0>4x}-{:0>4x}-{:0>8x}{:0>4x}", timeLow, timeMid, timeHiVersion,
						   varClockSeq, node1, node2);
	}

	friend bool operator==(const UUID &pLhs, const UUID &pRhs) {
		return pLhs.timeLow == pRhs.timeLow && pLhs.timeMid == pRhs.timeMid &&
			   pLhs.timeHiVersion == pRhs.timeHiVersion && pLhs.varClockSeq == pRhs.varClockSeq &&
			   pLhs.node1 == pRhs.node1 && pLhs.node2 == pRhs.node2;
	}

	friend bool operator!=(const UUID &pLhs, const UUID &pRhs) { return !(pLhs == pRhs); }

	operator bool() const { return *this != UUID(); }

	friend std::size_t hash_value(const UUID &pObj) {
		std::size_t seed = 0x6686B61D;
		seed ^= (seed << 6) + (seed >> 2) + 0x349E2F00 + static_cast<std::size_t>(pObj.timeLow);
		seed ^= (seed << 6) + (seed >> 2) + 0x6129562D + static_cast<std::size_t>(pObj.timeMid);
		seed ^= (seed << 6) + (seed >> 2) + 0x4A844698 + static_cast<std::size_t>(pObj.timeHiVersion);
		seed ^= (seed << 6) + (seed >> 2) + 0x1DC5B6C3 + static_cast<std::size_t>(pObj.varClockSeq);
		seed ^= (seed << 6) + (seed >> 2) + 0x5FC6A2F8 + static_cast<std::size_t>(pObj.node1);
		seed ^= (seed << 6) + (seed >> 2) + 0x00B47AA8 + static_cast<std::size_t>(pObj.node2);
		return seed;
	}
};
} // namespace ke

// Custom specialization of std::hash can be injected in namespace std.
template <>
struct std::hash<ke::UUID> {
	std::size_t operator()(const ke::UUID &s) const noexcept { return hash_value(s); }
};

template <>
struct std::formatter<ke::UUID> {
	bool upperCase{};

	constexpr auto parse(std::format_parse_context &ctx) {
		auto pos = ctx.begin();
		while (pos != ctx.end() && *pos != '}') {
			if (*pos == 'h' || *pos == 'H') upperCase = true;
			++pos;
		}
		return pos;
	}

	auto format(const ke::UUID &obj, std::format_context &ctx) const {
		return std::format_to(ctx.out(), "{}", obj.toString(upperCase));
	}
};

template <>
struct std::formatter<std::shared_ptr<ke::UUID>> {
	bool upperCase{};

	constexpr auto parse(std::format_parse_context &ctx) {
		auto pos = ctx.begin();
		while (pos != ctx.end() && *pos != '}') {
			if (*pos == 'h' || *pos == 'H') upperCase = true;
			++pos;
		}
		return pos;
	}

	auto format(const std::shared_ptr<ke::UUID> &obj, std::format_context &ctx) const {
		return std::format_to(ctx.out(), "{}", obj->toString(upperCase));
	}
};

template <>
struct std::formatter<ke::UUID*> {
	bool upperCase{};

	constexpr auto parse(std::format_parse_context &ctx) {
		auto pos = ctx.begin();
		while (pos != ctx.end() && *pos != '}') {
			if (*pos == 'h' || *pos == 'H') upperCase = true;
			++pos;
		}
		return pos;
	}

	auto format(ke::UUID* obj, std::format_context &ctx) const {
		return std::format_to(ctx.out(), "{}", obj->toString(upperCase));
	}
};
#endif //UUID_H
