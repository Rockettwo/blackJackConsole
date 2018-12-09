#include <random>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

#include "Game.h"

#include "RandomPlayer.h"
#include "Human.h"

namespace bj {

	Game::Game(GameOptions go) : _go(go)
	{
		for (unsigned int decks = 0; decks < _go.numOfDecks(); ++decks) {
			for (unsigned int colors = 0; colors < numOfColors; ++colors) {
				for (unsigned int ranks = 0; ranks < numOfRanks; ++ranks) {
					cardStacks.cardsInShoe.push_back( std::make_shared<Card>( Card( cardColor(colors), cardRank(ranks) ) ) );
				}
			}
		}

		playerVec.push_back(std::shared_ptr<Dealer>(new Dealer{}));
	}

	std::shared_ptr<Card> Game::getCardAt(cardStack stack, unsigned int idx) const
	{
		switch (stack) {
		case SHOE:
			if (idx >= cardStacks.cardsInShoe.size()) { std::cout << "Error: out of bounds" << std::endl; return 0; }
			return cardStacks.cardsInShoe[idx];
		case TABLE:
			if (idx >= cardStacks.cardsOnTable.size()) { std::cout << "Error: out of bounds" << std::endl; return 0; }
			return cardStacks.cardsOnTable[idx];
		case PLAYED:
			if (idx >= cardStacks.cardsPlayed.size()) { std::cout << "Error: out of bounds" << std::endl; return 0; }
			return cardStacks.cardsPlayed[idx];
		default:
			std::cout << "Error: unknonw stack" << std::endl;
			return 0;
		}
	}

	void Game::printLenOfCardVecs() const 
	{
		std::cout << "Cards in the shoe: " << cardStacks.cardsInShoe.size() << std::endl;
		std::cout << "Cards on the table: " << cardStacks.cardsOnTable.size() << std::endl;
		std::cout << "Cards previous played: " << cardStacks.cardsPlayed.size() << std::endl;
	}

	void Game::printCardAt(cardStack stack, unsigned int idx) const
	{
		std::shared_ptr<Card> actCard = getCardAt(stack, idx);
		if (actCard->isBroken()) return;

		std::cout << "Card Color: " << actCard->getColorString() << " - Rank: " << actCard->getRankString() << std::endl;
	}

	void Game::shuffle() 
	{
		std::shuffle(cardStacks.cardsInShoe.begin(), cardStacks.cardsInShoe.end(), std::default_random_engine(0));
	}

	void Game::addPlayer(std::string name, int money, strategy st)
	{
		if (money < 0) std::cout << "Warning: starting with negative budget" << std::endl;
		
		switch (st) {
		case RANDOM:
			playerVec.push_back(std::shared_ptr<RandomPlayer>(new RandomPlayer(st, name, money, std::make_shared<CardStacks>(cardStacks))));
			break;
		default:
			std::cout << "Error: Unknown strategy, no player added" << std::endl;
			return;
		}

		std::cout << "Added Player " << name << " with " << money << " money units starting budget." << std::endl;
	}

	std::shared_ptr<Card> Game::popCard()
	{
		std::shared_ptr<Card> actCard = cardStacks.cardsInShoe.back();
		cardStacks.cardsInShoe.pop_back();
		cardStacks.cardsOnTable.push_back(actCard);
		return actCard;
	}

	void Game::playSingle()
	{
		if (playerVec.size() < 2) { std::cout << "Error: no player" << std::endl; return; }
		std::shared_ptr<Card> actCard ;
		
		//deal two cards to every player
		for (int i = 1; i < playerVec.size(); ++i) {
			for (int j = 0; j < 2; ++j) {
				playerVec[i]->addCard(popCard(), false);
				//printGame();
			}
		}
		// dealer card
		playerVec[0]->addCard(popCard(), false);

		printGame();

		// go through player
		for (int i = 1; i < playerVec.size(); ++i) {
			std::shared_ptr<Player> player = playerVec[i];
			bool split = false;			// Has the player splitted his cards?
			int stack = 0;				// Which stack are we using now?
			while (!player->busted(stack)) {
				switch (player->move()) {
				case HIT:
					std::cout << "HIT" << std::endl;
					break;
				case STAND:
					std::cout << "STAND" << std::endl;
					break;
				case SPLIT:
					std::cout << "SPLIT" << std::endl;
					break;
				case DOUBLE:
					std::cout << "DOUBLE" << std::endl;
					break;
				default:
					std::cout << "NIX" << std::endl;
				}

			}

		}

	}

	void Game::printGame()
	{
		for (std::shared_ptr<Player> player : playerVec) {
			std::cout << player->name() << std::endl;
			player->printCards();
		}
	}

}