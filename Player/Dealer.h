#pragma once

#ifndef DEALER_H
#define DEALER_H

#include "Player.h"
#include "../GameEngine/GameOptions.h"

namespace bj {

	class Dealer : public Player
	{
	public:
		Dealer() { Player::init(DEALER, "Dealer", 0, 0); }

		action move() override;

	private:

	};




} //ns: bj


#endif // !DEALER_H