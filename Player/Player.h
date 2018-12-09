#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include <vector>
#include <memory>


namespace bj {

	enum strategy { DEALER, RANDOM, HUMAN };
	static std::string strategy_s[] = { "DEALER", "RANDOM", "HUMAN" };

	/// <summary>
	/// base class for player objects
	/// </summary>
	class Player
	{
	public:

		void init(strategy st, std::string name, int money, std::shared_ptr<CardStacks> cardStacks) {
			_st = st;
			_name = name;
			_money = money;
			_cardStacks = cardStacks;
		}

		virtual action move() = 0;

		void addCard(std::shared_ptr<Card> c, int stack);
		void clearCards();
		void printCards();

		void split();

		int cardSum(int stack);
		bool busted(int stack);

		bool splitAllowed(int stack);
		bool doubleAllowed(int stack);

		void printActions(int stack);

		bool isSplit() { return _split; }
		std::string name() { return _name; }

		

	protected:
		strategy _st;
		std::string _name;
		int _money;
		std::shared_ptr<CardStacks> _cardStacks;
		std::vector<std::shared_ptr<Card>> _cards[2];
		bool _split = false;
	};

} //ns: bj


#endif // !PLAYER_H