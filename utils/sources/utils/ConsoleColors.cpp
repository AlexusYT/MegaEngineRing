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
