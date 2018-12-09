
#include <iostream>
#include "Game.h"

int main()
{

    std::cout << "This is a black jack game!\n"; 

	std::cout << "Generating Game... ";

	// namespace bj
	using namespace bj;
	
	Game game(GameOptions(gameType::LV_EASY, 5));

	std::cout << "Done.\n";

	//game.printLenOfCardVecs();
	game.shuffle();

	//for (int i = 0; i < 52; ++i) {
	//	game.printCardAt(SHOE, i);
	//}

	game.addPlayer("Felix", 100, RANDOM);

	game.playSingle();
	
}