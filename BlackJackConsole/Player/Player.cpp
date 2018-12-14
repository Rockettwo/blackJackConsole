#include "Player.h"
#include <iostream>

namespace bj {

	bool Player::addCard(std::shared_ptr<Card> c, int stack)
	{
		_cards[stack].push_back(c);
		if (_cards[stack].size() == 2 && cardSum(stack) == 11 && containsAce(stack))
			blackJack[stack] = true;

		if (isBusted(stack)) {
			PRINT("Busted.\n");
			return true;
		} else {
			return false;
		}
	}

	bool Player::addCard2(std::shared_ptr<Card> c, int stack)
	{
		c->printCard(); PRINT("\n");
		return addCard(c, stack);
	}

	void Player::prepare()
	{
		for (auto& cards : _cards) {
			cards.erase(cards.begin(), cards.end());
		}
		_split = false;
		doubled[0] = false; doubled[1] = false;
		finished[0] = false; finished[1] = true;
		blackJack[0] = false; blackJack[1] = false;
	}

	void Player::printCards()
	{
		int j = 0;
		for (auto& cards : _cards) {
			for (int i = 0; i < cards.size(); ++i) {
				cards[i]->printCard();
				PRINT(i + int(1) != cards.size() ? " | " : "");
				if (i + int(1) == cards.size())
					PRINT(" --- card count: ", cardSum(j), "\n");
			}
			++j;
		}
	}

	void Player::hit(int stack)
	{

	}

	void Player::stand(int stack)
	{
		finished[stack] = true;
	}

	void Player::split()
	{
		_split = true;
		_cards[1].push_back(_cards[0].back());
		_cards[0].pop_back();
		finished[0] = false; finished[1] = false;
	}

	void Player::doubleDown(int stack)
	{
		doubled[stack] = true;
		_money -= _actBet;
		finished[stack] = true;
	}

	int Player::cardSum(int stack) {
		int sum = 0;
		for (auto card : _cards[stack]) {
			sum += card->getValue();
		}
		return sum;
	}

	void Player::getBet(unsigned int minBet) {
		_actBet = minBet;
		_money -= _actBet;
	}

	int Player::exchangeMoney(double ratio)
	{
		_money += int(_actBet * ratio);
		return int(_actBet * ratio);
	}

	bool Player::isBusted(int stack) {
		if (cardSum(stack) > 21) {
			finished[stack] = true;
			return true;
		}else {
			return false;
		}
	}

	bool Player::isFinished() {
		if (finished[0] && finished[1]) {
			for (int i = 0; i < 2; ++i) {
				if (containsAce(i) && cardSum(i) + 10 < 22) {
					for (auto card : _cards[i]) {
						if (card->getRank() == ACE) {
							card->setAceVal11();
							PRINT("Set val of ace to 11\n");
							break;
						}
					}
				}
			}
			finished[0] = false; finished[1] = true;
			return true;
		}
		else {
			return false;
		}
	}

	bool Player::containsAce(int stack) {
		for (auto card : _cards[stack]) {
			if (card->getRank() == ACE) 
				return true;
		}
		return false;
	}

	bool Player::splitAllowed(int stack) {
		if (!_split && _cards[stack].size() == 2
			&& _cards[stack][0]->getRank() == _cards[stack][1]->getRank()
			&& _money > int(_actBet)) {
			return true;
		}
		return false;
	}

	bool Player::doubleAllowed(int stack) {
		if (_cards[stack].size() == 2 && _money > int(_actBet)) {
			return true;
		}
		return false;
	}

	void Player::printActions(int stack) {
		PRINT("(H)IT | (S)TAND", (doubleAllowed(stack) ? " | (D)OUBLE" : ""), (splitAllowed(stack) ? " | S(P)LIT" : ""), "  - stack: ", stack, "\n");
	}

	void Player::generateAllowedVec(int stack) {
		allowedActions.clear();
		allowedActions.push_back(HIT);
		allowedActions.push_back(STAND);
		if (splitAllowed(stack)) allowedActions.push_back(SPLIT);
		if (doubleAllowed(stack)) allowedActions.push_back(DOUBLE);
	}

}