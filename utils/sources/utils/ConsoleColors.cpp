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
// Created by Alexus on 15.10.2022.
//

#include "ConsoleColors.h"

#ifdef WIN32
HANDLE ConsoleColors::hCon = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO ConsoleColors::csbi;
DWORD ConsoleColors::dwConSize;

void ConsoleColors::setColor(WORD wRGBI, WORD Mask) {
	getInfo();
	csbi.wAttributes &= Mask;
	csbi.wAttributes |= wRGBI;
	SetConsoleTextAttribute(hCon, csbi.wAttributes);
}

void ConsoleColors::getInfo() {
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
}

void ConsoleColors::clear() {
	COORD coordScreen = {0, 0};
	DWORD cCharsWritten;
	getInfo();
	FillConsoleOutputCharacter(hCon, ' ', dwConSize, coordScreen, &cCharsWritten);
	getInfo();
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hCon, coordScreen);
}

#endif
