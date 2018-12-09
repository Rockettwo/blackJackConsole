#pragma once

#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"
#include "../GameEngine/GameOptions.h"

namespace bj {

	class Human : public Player
	{
	public:
		Human(strategy st, std::string name, int money, std::shared_ptr<CardStacks> cardStacks) { Player::init(st, name, money, cardStacks); }

		action move() override;

	private:

	};




} //ns: bj


#endif // !HUMAN_H