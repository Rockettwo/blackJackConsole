#pragma once

#ifndef RANDOM_PLAYER_H
#define RANDOM_PLAYER_H

#include <stdlib.h>
#include <time.h>

#include "Player.h"
#include "../GameEngine/GameOptions.h"

namespace bj {
	class RandomPlayer : public Player
	{
	public:
		RandomPlayer(strategy st, std::string name, std::shared_ptr<GameOptions> go, int money, std::shared_ptr<CardStacks> cardStacks) {
			Player::init(st, name, go, money, cardStacks);
			srand(unsigned int(time(NULL)));
		}

		action move(int stack) override { return (rand() % 2) < 1 ? HIT : STAND ; };

	private:

	};
} //ns: bj


#endif // !RANDOM_PLAYER_H