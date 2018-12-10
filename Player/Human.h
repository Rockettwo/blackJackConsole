#pragma once

#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"
#include "../GameEngine/GameOptions.h"
#include <string>

namespace bj {

	class Human : public Player
	{
	public:
		Human(strategy st, std::string name, std::shared_ptr<GameOptions> go, int money, std::shared_ptr<CardStacks> cardStacks) { Player::init(st, name, go, money, cardStacks); }

		action move(int stack) override;

	private:

		action charToAction(char c);

	};




} //ns: bj


#endif // !HUMAN_H