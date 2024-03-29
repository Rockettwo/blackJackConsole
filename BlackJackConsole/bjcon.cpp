
#include "libs/logger.hpp"

#include <iostream>
#include "GameEngine/Game.h"

int main()
{

	PRINT("This is a black jack game!\n");

	PRINT("Generating Game... ");

	// namespace bj
	using namespace bj;

	Game game(GameOptions(gameType::LV_EASY, 10));

	PRINT("Done.\n");

	//game.printLenOfCardVecs();
	game.shuffle();

	//for (int i = 0; i < numOfCards; ++i) {
	//	game.printCardAt(SHOE, i);
	//}

	//game.addPlayer("Felix", 100, HUMAN);
	//game.addPlayer("Matthew", 100, HUMAN);
	game.addPlayer("Matthew1", 100, RANDOM);
	game.addPlayer("Matthew2", 100, RANDOM);
	game.addPlayer("Matthew3", 100, RANDOM);	
	game.addPlayer("Matthew4", 100, RANDOM);
	game.addPlayer("Matthew5", 100, RANDOM);
	game.addPlayer("Matthew6", 100, RANDOM);	
	game.addPlayer("Matthew7", 100, RANDOM);
	game.addPlayer("Matthew8", 100, RANDOM);
	game.addPlayer("Matthew9", 100, RANDOM);


	game.play();

}