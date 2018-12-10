#pragma once

#ifndef DEALER_H
#define DEALER_H

#include "Player.h"
#include "../GameEngine/GameOptions.h"

namespace bj {

	class Dealer : public Player
	{
	public:
		Dealer(std::shared_ptr<GameOptions> go) { Player::init(DEALER, "Dealer", go, 0, 0); }

		action move(int stack) override;

	private:

	};




} //ns: bj


#endif // !DEALER_H