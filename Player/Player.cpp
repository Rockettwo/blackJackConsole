#include "Player.h"
#include <iostream>

namespace bj {

	void Player::addCard(std::shared_ptr<Card> c, int stack)
	{
		_cards[stack].push_back(c);
	}

	void Player::clearCards()
	{
		for (auto& cards : _cards) {
			cards.erase(cards.begin(), cards.end());
		}
	}

	void Player::printCards()
	{
		for (auto& cards : _cards) {
			for (int i = 0; i < cards.size(); ++i) {
				cards[i]->printCard();
				std::cout << (i + 1 != cards.size() ? " | " : " \n");
			}
		}

	}

	void Player::split()
	{
		_split = true;
		_cards[1].push_back(_cards[0].back());
		_cards[0].pop_back();
	}

	int Player::cardSum(int stack) {
		int sum = 0;
		for (auto card : _cards[stack]) {
			sum += card->getValue();
		}
		return sum;
	}

	bool Player::busted(int stack) {
		return cardSum(stack) > 21;
	}

	bool Player::splitAllowed(int stack) {
		if (!_split && _cards[stack].size() == 2
			&& _cards[stack][0]->getRank() == _cards[stack][1]->getRank()) {
			return true;
		}
		return false;
	}

	bool Player::doubleAllowed(int stack) {
		if (_cards[stack].size() == 2) {
			return true;
		}
		return false;
	}

	void Player::printActions(int stack) {
		std::cout << "(H)IT | (S)TAND" << (doubleAllowed(stack) ? " | (D)OUBLE" : "") << (splitAllowed(stack) ? " | (S)PLIT" : "") << std::endl;
	}

}