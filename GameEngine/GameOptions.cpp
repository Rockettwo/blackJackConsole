
#include <random>
#include <algorithm>
#include <iostream>
#include <vector>

#include "Game.h"

namespace bj {
	// GameOptions 
	GameOptions::GameOptions(gameType gt, unsigned int betUnit) : _gt(gt), _betUnit(betUnit)
	{
		switch (_gt) {
		case LV_EASY:
			_numOfDecks = 1;
			_bjPayRate = 1.5;
			_insurance = true;
			break;
		case LV_6D_32:
			_numOfDecks = 6;
			_bjPayRate = 1.5;
			_insurance = true;
			break;
		case LV_6D_65:
			_numOfDecks = 6;
			_bjPayRate = 1.2;
			_insurance = true;
			break;
		case LV_8D_32:
			_numOfDecks = 8;
			_bjPayRate = 1.5;
			_insurance = true;
			break;
		case LV_8D_65:
			_numOfDecks = 8;
			_bjPayRate = 1.2;
			_insurance = true;
			break;
		default:
			_numOfDecks = 6;
			_bjPayRate = 1.5;
			_insurance = true;
		}

	}
}