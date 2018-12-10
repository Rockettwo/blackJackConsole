#pragma once

#ifndef CARD_HPP
#define CARD_HPP

#include "GameOptions.h"
#include <iostream>
#include <string>
#include <vector>

namespace bj {

	/// <summary>
	/// Reselmbles one card and its properties if _value smaller as 0 broken card
	/// </summary>
	class Card
	{
	public:
		Card() : _cC(HEART), _cR(ACE), _ace(true), _value(-1) {}

		Card(cardColor cC, cardRank cR) : _cC(cC), _cR(cR), _ace(_cR == ACE ? true : false)
		{
			_value = _cR < JACK ? int(_cR) + 1 : 10;
			_ace = _cR == ACE;
		}

		Card& operator=(Card& other) {
			_cC = other._cC;
		}

		bool isBroken() const { return _value < -1; };

		cardColor getColor() const { return _cC; }
		cardRank getRank() const { return _cR; }
		int getValue() const { return _value; }

		std::string getColorString() const { return cardColor_s[size_t(_cC)]; }
		std::string getRankString() const { return cardRank_s[size_t(_cR)]; }

		void printCard() { std::cout << getColorString() << "-" << getRankString(); }

		void setAceVal11() { _value = _ace ? 11 : _value; }

	private:
		cardColor _cC;
		cardRank _cR;
		bool _ace;
		int _value;

	};


	/// <summary>
	/// different card stacks 
	/// </summary>
	struct CardStacks {
		std::vector<std::shared_ptr<Card>> cardsInShoe;
		std::vector<std::shared_ptr<Card>> cardsOnTable;
		std::vector<std::shared_ptr<Card>> cardsPlayed;
	};

}

#endif // !CARD_HPP
