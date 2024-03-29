#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>

#include "GameOptions.h"
#include "Card.h"
#include "../Player/Player.h"
#include "../Player/Dealer.h"

namespace bj {

	/// <summary>
	/// Game logic and actions
	/// </summary>
	class Game 
	{
	public:
		Game(GameOptions go);

		std::shared_ptr<Card> getCardAt(cardStack stack, unsigned int idx) const;

		void printLenOfCardVecs() const;
		void printCardAt(cardStack stack, unsigned int idx) const;
		std::shared_ptr<Card> popCard();

		void shuffle();
		void shuffleIfNeeded();

		void addPlayer(std::string name, int money, strategy st);
		void deletePlayer(int idx);
		void printGame();

		void playSingle();
		void play();
		void evaluate();

	private:
		GameOptions _go;
		CardStacks cardStacks;
		unsigned int round = 0;

		std::vector<std::shared_ptr<Player>> playerVec;

	};

} //ns: bj


#endif // !GAME_HPP