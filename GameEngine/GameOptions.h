#pragma once

#ifndef GAMEOPTIONS_HPP
#define GAMEOPTIONS_HPP

#include <iostream>

namespace bj {
	constexpr unsigned int numOfColors = 4;
	constexpr unsigned int numOfRanks = 13;

	enum cardStack { SHOE, TABLE, PLAYED };
	static std::string cardStack_s[] = { "SHOE", "TABLE", "PLAYED" };

	enum gameType { LV_EASY, LV_6D_32, LV_6D_65, LV_8D_32, LV_8D_65 };
	static std::string gameType_s[] = { "LV_EASY", "LV_6D_32", "LV_6D_65", "LV_8D_32", "LV_8D_65" };

	enum cardColor { HEART, CLUB, DIAMOND, SPADE };
	static std::string cardColor_s[] = { "HEART", "CLUB", "DIAMOND", "SPADE" };

	enum cardRank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };
	static std::string cardRank_s[] = { "ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING" };

	enum action { HIT, STAND, DOUBLE, SPLIT };
	static std::string action_s[] = { "HIT", "STAND", "DOUBLE", "SPLIT" };

	/// <summary>
	/// Options of game such as the Black Jack payrate, the number of decks, insurance available, betting unit (in money unit) and number of players participating
	/// </summary>
	class GameOptions
	{
	public:
		GameOptions(gameType gt, unsigned int betUnit);

		unsigned int numOfDecks() const { return _numOfDecks; };
		double bjPayRate() const { return _bjPayRate; };
		bool insurance() const { return _insurance; };
		unsigned int betUnit() const { return _betUnit; };

	private:
		gameType _gt;
		unsigned int _numOfDecks;
		double _bjPayRate;
		bool _insurance;
		unsigned int _betUnit;

	};

}

#endif  // !GAMEOPTIONS_HPP