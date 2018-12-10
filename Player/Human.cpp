#include "Human.h"

#include <algorithm>

namespace bj {

	action Human::move(int stack)
	{
		Player::printActions(stack);
		action a = INVALID;
		while (a == INVALID) {
			char c;
			std::cin >> c;
			a = charToAction(c);
		}
		
		return a;
	}

	action Human::charToAction(char c) {
		c = tolower(c);
		
		switch (c) {
		case 'h':
			return HIT;
		case 's':
			return STAND;
		case 'p':
			return SPLIT;
		case 'd':
			return DOUBLE;
		default:
			return INVALID;
		}
	}

}