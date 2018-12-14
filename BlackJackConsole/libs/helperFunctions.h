#pragma once

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <iostream>

void clrscr() {
	PRINT("\033[2J\033[1;1H");
}

void waitForInput() {
	char c = getchar();
	std::system("pause");
}

#endif