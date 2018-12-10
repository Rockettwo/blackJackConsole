#include <random>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <time.h>

#include "Game.h"

#include "../Player/RandomPlayer.h"
#include "../Player/Human.h"

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

		//playerVec.push_back(std::shared_ptr<Dealer>(new Dealer(std::make_shared<GameOptions>(_go))));
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
		std::shuffle(cardStacks.cardsInShoe.begin(), cardStacks.cardsInShoe.end(), std::default_random_engine(time(0)));
	}

	void Game::addPlayer(std::string name, int money, strategy st)
	{
		if (money < 0) std::cout << "Warning: starting with negative budget" << std::endl;
		
		switch (st) {
		case RANDOM:
			playerVec.push_back(std::shared_ptr<RandomPlayer>(new RandomPlayer(st, name, std::make_shared<GameOptions>(_go), money, std::make_shared<CardStacks>(cardStacks))));
			break;
		case HUMAN:
			playerVec.push_back(std::shared_ptr<Human>(new Human(st, name, std::make_shared<GameOptions>(_go), money, std::make_shared<CardStacks>(cardStacks))));
			break;
		default:
			std::cout << "Error: Unknown strategy, no player added" << std::endl;
			return;
		}

		std::cout << "Added Player " << name << " with " << money << " money units starting budget." << std::endl;
	}

	void Game::deletePlayer(int idx)
	{
		std::cout << "Deleted Player " << playerVec.at(idx)->name() << " with " << playerVec.at(idx)->getMoney() << " at the end." << std::endl;
		playerVec.erase(playerVec.begin() + idx);
	}

	std::shared_ptr<Card> Game::popCard()
	{
		std::shared_ptr<Card> actCard = cardStacks.cardsInShoe.back();
		cardStacks.cardsInShoe.pop_back();
		cardStacks.cardsOnTable.push_back(actCard);
		return actCard;
	}

	void Game::play()
	{
		while (playerVec.size() > 1) {
			playSingle();
			for (int i = 1; i < playerVec.size(); ++i) {
				//playerVec[i]->prepare();
				if (playerVec[i]->getMoney() < _go.betUnit()) deletePlayer(i);
			}
		}
		std::cout << "Error: no player" << std::endl;
	}


	void Game::playSingle()
	{
		if (playerVec.size() < 2) { std::cout << "Error: no player" << std::endl; return; }
		std::shared_ptr<Card> actCard ;
		
		//deal two cards to every player
		for (int i = 1; i < playerVec.size(); ++i) {
			for (int j = 0; j < 2; ++j) {
				playerVec[i]->addCard(popCard(), 0);
				//printGame();
			}
		}
		// dealer card
		playerVec[0]->addCard(popCard(), 0);

		printGame();

		// go through player
		for (int i = 1; i < playerVec.size(); ++i) {
			std::shared_ptr<Player> player = playerVec[i];
			bool split = false;			// Has the player splitted his cards?
			int stack = 0;				// Which stack are we using now?
			std::cout << player->name() << " it's your turn. You have " << player->getMoney() << " left." << std::endl;
			player->getBet(_go.betUnit());			// First get money
			while (!player->isFinished()) {
				switch (player->move(stack)) {
				case HIT:
					std::cout << "HIT" << std::endl ;
					//player->hit(stack);
					if (player->addCard(popCard(), stack) && split && stack == 0) {
						stack = 1;
					}
					break;
				case STAND:
					std::cout << "STAND" << std::endl;
					player->stand(stack);
					if (split && stack == 0) {
						stack = 1;
					}
					break;
				case SPLIT:
					if (split) { std::cout << "STAND - INVALID" << std::endl; break; }
					std::cout << "SPLIT" << std::endl;
					player->split();
					player->addCard(popCard(), 0);
					player->addCard(popCard(), 1);
					split = true;
					break;
				case DOUBLE:
					std::cout << "DOUBLEDOWN" << std::endl;
					player->doubleDown(stack);
					if (player->addCard(popCard(), stack) && split && stack == 0) {
						stack = 1;
					}
					break;
				default:
					std::cout << "NIX" << std::endl;
				}
				player->printCards();
			}
			std::cout << "------" << std::endl;
		}
	
	}

	void Game::printGame()
	{
		std::cout << "--------------- GAME ---------------" << std::endl;
		for (std::shared_ptr<Player> player : playerVec) {
			std::cout << player->name() << std::endl;
			player->printCards();
			std::cout << "---------------------------" << std::endl;
		}
		std::cout << "--------------- ---- ---------------" << std::endl;
	}

}