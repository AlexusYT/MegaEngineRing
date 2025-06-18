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
// Created by Alexus on 15.10.2022.
//

#ifndef ELECTRICITY_SIMULATOR_CONSOLECOLORS_H
#define ELECTRICITY_SIMULATOR_CONSOLECOLORS_H


#include <iomanip>

#ifdef WIN32

	#include <windows.h>

	#undef ERROR
	#undef OUT

	#define ADD_COLOR(name, color)                                                                                     \
		static inline std::ostream &LIGHT_##name(std::ostream &os) {                                                   \
			os.flush();                                                                                                \
			setColor(color, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);               \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::ostream &DARK_##name(std::ostream &os) {                                                    \
			os.flush();                                                                                                \
			setColor(color | FOREGROUND_INTENSITY,                                                                     \
					 BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);                      \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::ostream &BACK_LIGHT_##name(std::ostream &os) {                                              \
			os.flush();                                                                                                \
			setColor(color,                                                                                            \
					 BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);    \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::ostream &BACK_DARK_##name(std::ostream &os) {                                               \
			os.flush();                                                                                                \
			setColor(color | FOREGROUND_INTENSITY,                                                                     \
					 FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);                      \
			return os;                                                                                                 \
		}
#else
#define ADD_COLOR(name, color)                                                                                     \
		static inline std::ostream &LIGHT_##name(std::ostream &os) {                                                   \
			os.flush();                                                                                                \
			os << "\033[" << color + 60 << "m";                                                                        \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::ostream &DARK_##name(std::ostream &os) {                                                    \
			os.flush();                                                                                                \
			os << "\033[" << color << "m";                                                                             \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::ostream &BACK_LIGHT_##name(std::ostream &os) {                                              \
			os.flush();                                                                                                \
			os << "\033[" << color + 70 << "m";                                                                        \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::ostream &BACK_DARK_##name(std::ostream &os) {                                               \
			os.flush();                                                                                                \
			os << "\033[" << color + 10 << "m";                                                                        \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::wostream &LIGHT_##name(std::wostream &os) {                                                 \
			os.flush();                                                                                                \
			os << L"\033[" << color + 60 << L"m";                                                                      \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::wostream &DARK_##name(std::wostream &os) {                                                  \
			os.flush();                                                                                                \
			os << L"\033[" << color << L"m";                                                                           \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::wostream &BACK_LIGHT_##name(std::wostream &os) {                                            \
			os.flush();                                                                                                \
			os << L"\033[" << color + 70 << L"m";                                                                      \
			return os;                                                                                                 \
		}                                                                                                              \
		static inline std::wostream &BACK_DARK_##name(std::wostream &os) {                                             \
			os.flush();                                                                                                \
			os << L"\033[" << color + 10 << L"m";                                                                      \
			return os;                                                                                                 \
		}
#endif
class ConsoleColors {
#ifdef WIN32
	static HANDLE hCon;
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	static DWORD dwConSize;

	static void getInfo();

	static void clear();

	static void setColor(WORD wRGBI, WORD Mask);

#endif

public:
#ifdef WIN32

	ADD_COLOR(RED, FOREGROUND_RED);

	ADD_COLOR(GREEN, FOREGROUND_GREEN);

	ADD_COLOR(BLUE, FOREGROUND_BLUE);

	ADD_COLOR(CYAN, FOREGROUND_GREEN | FOREGROUND_BLUE);

	ADD_COLOR(MAGENTA, FOREGROUND_RED | FOREGROUND_BLUE);

	ADD_COLOR(YELLOW, FOREGROUND_RED | FOREGROUND_GREEN);

	ADD_COLOR(WHITE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	//ADD_COLOR(BLACK, 0);
	static inline std::ostream &BLACK(std::ostream &os) {
		os.flush();
		setColor(0, 0x10 | 0x20 | 0x40 | 0x80);
		return os;
	}

	static inline std::ostream &GRAY(std::ostream &os) {
		os.flush();
		setColor(0 | 0x8, 0x10 | 0x20 | 0x40 | 0x80);
		return os;
	}

	static inline std::ostream &CLEAR(std::ostream &os) {
		os.flush();
		clear();
		return os;
	}

#else
	ADD_COLOR(RED, 31)

	ADD_COLOR(GREEN, 32)

	ADD_COLOR(BLUE, 34)

	ADD_COLOR(CYAN, 36)

	ADD_COLOR(MAGENTA, 35)

	ADD_COLOR(YELLOW, 33)

	ADD_COLOR(WHITE, 37)

	static inline std::ostream &BLACK(std::ostream &os) {
		os.flush();
		os << "\033[30m";
		return os;
	}

	static inline std::ostream &GRAY(std::ostream &os) {
		os.flush();
		os << "\033[90m";
		return os;
	}

	static inline std::ostream &CLEAR(std::ostream &os) {
		os.flush();
		os << "\033[0m";
		return os;
	}
#endif
};


#endif //ELECTRICITY_SIMULATOR_CONSOLECOLORS_H
