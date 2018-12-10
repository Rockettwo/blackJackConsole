#pragma once

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <iostream>

void clrscr() {
	std::cout << "\033[2J\033[1;1H";
}

#endif