#include "Player.h"
#include <iostream>

namespace bj {

	bool Player::addCard(std::shared_ptr<Card> c, int stack)
	{
		_cards[stack].push_back(c);
		if (isBusted(stack))
			return true;
		else
			return false;
	}

	void Player::prepare()
	{
		for (auto& cards : _cards) {
			cards.erase(cards.begin(), cards.end());
		}
		_split = false;
		doubled[0] = false; doubled[1] = false;
		finished[0] = false; finished[1] = true;
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

	bool Player::isBusted(int stack) {
		if (cardSum(stack) > 21) {
			finished[stack] = true;
			std::cout << "Busted." << std::endl;
			return true;
		}
		else {
			return false;
		}
	}

	bool Player::isFinished() {
		if (finished[0] && finished[1]) {
			finished[0] = false; finished[1] = true;
			return true;
		}
		else {
			return false;
		}
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
		std::cout << "(H)IT | (S)TAND" << (doubleAllowed(stack) ? " | (D)OUBLE" : "") << (splitAllowed(stack) ? " | S(P)LIT" : "") << std::endl;
	}

	void Player::generateAllowedVec(int stack) {
		allowedActions.clear();
		allowedActions.push_back(HIT);
		allowedActions.push_back(STAND);
		if (splitAllowed(stack)) allowedActions.push_back(SPLIT);
		if (doubleAllowed(stack)) allowedActions.push_back(DOUBLE);
	}

}