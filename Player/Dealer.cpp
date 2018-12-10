#include "Dealer.h"

namespace bj {

	action Dealer::move(int stack)
	{
		// this is soft 17
		if (_cards[0][0]->getRank() == ACE) _cards[0][0]->setAceVal11();
		if (!(_cards[0][0]->getRank() == ACE) && _cards[0][1]->getRank() == ACE) _cards[0][1]->setAceVal11();
		if (_cards[0][_cards[0].size() - 1]->getRank() == ACE && (cardSum(0) + 10) < 22) _cards[0][_cards[0].size() - 1]->setAceVal11();

		if (cardSum(0) < 17) 
			return HIT;
		else 
			return STAND;
	}

	int Dealer::exchangeMoney(double ratio)
	{
		_money += int(ratio);
		return int(ratio);
	}

}