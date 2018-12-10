#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "../GameEngine/Card.h"
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

		void init(strategy st, std::string name, std::shared_ptr<GameOptions> go, int money, std::shared_ptr<CardStacks> cardStacks) {
			_st = st;
			_name = name;
			_go = go;
			_money = money;
			_cardStacks = cardStacks;
		}

		virtual action move(int stack) = 0;

		bool addCard(std::shared_ptr<Card> c, int stack);
		void prepare();
		void printCards();

		void hit(int stack);
		void stand(int stack);
		void split();
		void doubleDown(int stack);
		
		void getBet(unsigned int minBet);
		int cardSum(int stack);
		bool isBusted(int stack);
		bool isFinished();
		int getMoney() { return _money; };

		bool splitAllowed(int stack);
		bool doubleAllowed(int stack);

		void printActions(int stack);
		void generateAllowedVec(int stack);

		bool isSplit() { return _split; }
		std::string name() { return _name; }

	protected:
		std::shared_ptr<GameOptions> _go;
		strategy _st;
		std::string _name;
		int _money;
		std::shared_ptr<CardStacks> _cardStacks;

		std::vector<action> allowedActions;
		std::vector<std::shared_ptr<Card>> _cards[2];
		bool _split = false;
		bool doubled[2] = { false, false };
		bool finished[2] = { false, true };
		unsigned int _actBet;
	};

} //ns: bj


#endif // !PLAYER_H