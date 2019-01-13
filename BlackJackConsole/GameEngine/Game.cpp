#include <random>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <time.h>

#include "Game.h"

#include "../Player/RandomPlayer.h"
#include "../Player/Human.h"
#include "../libs/helperFunctions.h"
#include "../libs/logger.hpp"

namespace bj {

	Game::Game(GameOptions go) : _go(go)
	{
		for (unsigned int decks = 0; decks < _go.numOfDecks(); ++decks) {
			for (unsigned int colors = 0; colors < numOfColors; ++colors) {
				for (unsigned int ranks = 0; ranks < numOfRanks; ++ranks) {
					cardStacks.cardsInShoe.push_back(std::make_shared<Card>(Card(cardColor(colors), cardRank(ranks))));
				}
			}
		}

		playerVec.push_back(std::shared_ptr<Player>(new Dealer(std::make_shared<GameOptions>(_go))));
	}

	std::shared_ptr<Card> Game::getCardAt(cardStack stack, unsigned int idx) const
	{
		switch (stack) {
		case SHOE:
			if (idx >= cardStacks.cardsInShoe.size()) { ERR("Error: out of bounds\n"); return 0; }
			return cardStacks.cardsInShoe[idx];
		case TABLE:
			if (idx >= cardStacks.cardsOnTable.size()) { ERR("Error: out of bounds\n"); return 0; }
			return cardStacks.cardsOnTable[idx];
		case PLAYED:
			if (idx >= cardStacks.cardsPlayed.size()) { ERR("Error: out of bounds\n"); return 0; }
			return cardStacks.cardsPlayed[idx];
		default:
			ERR("Error: unknown stack\n");
			return 0;
		}
	}

	void Game::printLenOfCardVecs() const
	{
		PRINT("Cards in the shoe: ", cardStacks.cardsInShoe.size(), "\n");
		PRINT("Cards on table: ", cardStacks.cardsOnTable.size(), "\n");
		PRINT("Cards previous played: ", cardStacks.cardsPlayed.size(), "\n");
	}

	void Game::printCardAt(cardStack stack, unsigned int idx) const
	{
		std::shared_ptr<Card> actCard = getCardAt(stack, idx);
		if (actCard->isBroken()) return;

		PRINT("Card Color: ", actCard->getColorString(), " - Rank: ", actCard->getRankString(), "\n");
	}

	void Game::shuffleIfNeeded()
	{
		cardStacks.moveTablePlayed();
		if (cardStacks.cardsInShoe.size() < int(_go.reshuffleRatio() * double(numOfCards))
			|| cardStacks.cardsInShoe.size() < 3 * playerVec.size()) {
			cardStacks.movePlayedShoe();
			shuffle();
		}
	}

	void Game::shuffle()
	{
		std::shuffle(cardStacks.cardsInShoe.begin(), cardStacks.cardsInShoe.end(), std::default_random_engine(unsigned int(time(NULL))));
	}

	void Game::addPlayer(std::string name, int money, strategy st)
	{
		if (money < 0) PRINT("Warning: starting with negative budget\n");

		switch (st) {
		case RANDOM:
			playerVec.push_back(std::shared_ptr<RandomPlayer>(new RandomPlayer(st, name, std::make_shared<GameOptions>(_go), money, std::make_shared<CardStacks>(cardStacks))));
			break;
		case HUMAN:
			playerVec.push_back(std::shared_ptr<Human>(new Human(st, name, std::make_shared<GameOptions>(_go), money, std::make_shared<CardStacks>(cardStacks))));
			break;
		default:
			PRINT("Error: Unknown strategy, no player added\n");
			return;
		}

		PRINT("Added Player ", name, " with ", money, " money units starting budget.\n");
	}

	void Game::deletePlayer(int idx)
	{
		PRINT("Deleted Player ", playerVec.at(idx)->name(), " with ", playerVec.at(idx)->getMoney(), " at the end.\n");
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
		long gameCounter = 0;
		while (playerVec.size() > 1) {
			shuffleIfNeeded();

			playerVec[0]->prepare();
			for (int i = 1; i < playerVec.size(); ++i) {
				playerVec[i]->prepare();
				if (unsigned int(playerVec[i]->getMoney()) < _go.betUnit()) deletePlayer(i);
			}
			LOG1("0 ", "1 ", "NEW ", gameCounter);
			playSingle();

#if CMD__DEBUG
			waitForInput();
			clrscr();
#endif
			++gameCounter;
		}
		PRINT("Error: no player\n");
	}

	void Game::playSingle()
	{
		static std::shared_ptr<Player> dealer = playerVec[0];
		if (playerVec.size() < 2) { PRINT("Error: no player\n"); return; }
		std::shared_ptr<Card> actCard;

		//deal two cards to every player
		for (int i = 1; i < playerVec.size(); ++i) {
			for (int j = 0; j < 2; ++j) {
				playerVec[i]->addCard(popCard(), 0);
				//printGame();
			}
			LOG1(i, " ", "0 ", "DEAL ", playerVec[i]->cardsString());
		}
		// dealer card
		dealer->addCard(popCard(), 0);
		LOG1("0 ", "0 ", "DEAL ", dealer->cardsString());
		printGame();

		// go through player
		for (int i = 1; i < playerVec.size(); ++i) {
			std::shared_ptr<Player> player = playerVec[i];
			bool split = false;			// Has the player splitted his cards?
			int stack = 0;				// Which stack are we using now?
			PRINT(player->name(), " it's your turn. You have ", player->getMoney(), " money left.\n");
			player->setBet(_go.betUnit());			
			dealer->exchangeMoney(player->exchangeMoney(-1));			// First get money
			while (!player->isFinished()) {

				PRINT("\n");
				player->printCards();

				action move = player->isBlackJack(stack) ? STAND : player->move(stack);

				if (!player->isAllowedAction(move, stack)) {
					PRINT("Is not an allowed action.");
					continue;
				}
				
				switch (move) {
				case HIT:
					PRINT("HIT ");
					//player->hit(stack);
					if (player->addCard2(popCard(), stack) && split && stack == 0) {
						stack = 1;
					}
					LOG1(i, " ", "0 ", "HIT ", playerVec[i]->cardsString());
					break;
				case STAND:
					PRINT("STAND \n");
					player->stand(stack);
					if (split && stack == 0) {
						stack = 1;
					}
					LOG1(i, " ", "0 ", "STAND ", playerVec[i]->cardsString());
					break;
				case SPLIT:
					PRINT("SPLIT \n");
					player->split();
					player->addCard2(popCard(), 0);
					player->addCard2(popCard(), 1);
					split = true;
					LOG1(i, " ", "0 ", "SPLIT ", playerVec[i]->cardsString());
					break;
				case DOUBLE:
					PRINT("DOUBLEDOWN ");
					player->doubleDown(stack);
					if (player->addCard2(popCard(), stack) && split && stack == 0) {
						stack = 1;
					}
					LOG1(i, " ", "0 ", "DOUBLEDOWN ", playerVec[i]->cardsString());
					break;
				default:
					PRINT("NIX \n");
				}
			}
			PRINT("------\n\n");
		}

		// Now dealer gets cards:
		PRINT("Dealer\n");
		dealer->addCard(popCard(), 0);
		while (!dealer->isFinished()) {
			switch (dealer->move(0)) {
			case HIT:
				//player->hit(stack);
				dealer->addCard(popCard(), 0);
				LOG1("0 ", "0 ", "HIT ", dealer->cardsString());
				break;
			case STAND:
				dealer->stand(0);
				LOG1("0 ", "0 ", "STAND ", dealer->cardsString());
				break;
			default:
				break;
			}
		}
		PRINT("\n"); dealer->printCards();
		PRINT("------\n\n");

		evaluate();
	}

	void Game::evaluate()
	{
		static std::shared_ptr<Player> dealer = playerVec[0];

		std::string cardResult = "";
		for (int i = 1; i < playerVec.size(); ++i) {
			std::shared_ptr<Player> player = playerVec[i];
			for (int j = 0; j < (player->isSplit() ? 2 : 1); ++j) {
				double ratio = 0.0;
				int cardSum = player->cardSum(j);
				if (player->isBusted(j)) {
					PRINT(player->name(), " - stack ", j, " - You got busted.\n");
					cardResult = std::to_string(cardSum);
					ratio = 0.0;
				} else if (player->isBlackJack(j) && !dealer->isBlackJack(0)) {
					PRINT(player->name(), " - stack ", j, " - You have a blackjack.\n");
					cardResult = "BJ";
					ratio = 1.0 + _go.bjPayRate();
				} else if (player->isBlackJack(j) && dealer->isBlackJack(0)) {
					PRINT(player->name(), " - stack ", j, " - Blackjack for both.\n");
					cardResult = "BJBJ";
					ratio = 1.0;
				} else if (dealer->isBlackJack(0)) {
					PRINT(player->name(), " - stack ", j, " - Blackjack for dealer.\n");
					cardResult = "DBJ";
					ratio = 0.0;
				} else if (dealer->isBusted(0)) {
					PRINT(player->name(), " - stack ", j, " - Dealer busted.\n");
					cardResult = std::to_string(cardSum);
					ratio = 2.0;
				} else if (dealer->cardSum(0) > cardSum) {
					PRINT(player->name(), " - stack ", j, " - Dealer has ", dealer->cardSum(j), " you have ", cardSum, ".\n");
					cardResult = std::to_string(cardSum);
					ratio = 0.0;
				} else if (dealer->cardSum(0) < cardSum) {
					PRINT(player->name(), " - stack ", j, " - Dealer has ", dealer->cardSum(j), " you have ", cardSum, ".\n");
					cardResult = std::to_string(cardSum);
					ratio = 2.0;
				} else {
					PRINT(player->name(), " - stack ", j, " - Equal split.\n");
					cardResult = std::to_string(cardSum);
					ratio = 1.0;
				}
				int exchangedMoney = dealer->exchangeMoney(player->exchangeMoney(ratio)) - 10;
				LOG2(i, " ", cardResult, " ", exchangedMoney, " ", player->getMoney());
			}
			PRINT(player->name(), " you have ", player->getMoney(), " money left.\n\n");
		}
		LOG2("0 ", dealer->cardSum(0), " X ", dealer->getMoney());
	}

	void Game::printGame()
	{
		PRINT("\n--------------- GAME ---------------\n");
		for (std::shared_ptr<Player> player : playerVec) {
			PRINT(player->name(), "\n");
			player->printCards();
			PRINT("---------------------------\n");
		}
		PRINT("\n--------------- ----- ---------------\n\n");
	}

}