//
// Created by Krish on 11/22/2023.
//

#include "GameEngine.h"
#include "GameContext.h"
#include "PlayersStrategiesDriver.h"
#include <iostream> //Include Input/Output
#include <string> //Include String Library
using namespace std; //Using the standard namespace

void testPlayerStrategies(){

    /**
     * TODO:
     */

    std::cout << "--------------- TESTING STRATEGIES IMPLEMENTATION --------------" << std::endl;
    std::cout << "" << std::endl;

    GameContext* context;
    context->gameMap = nullptr;
    context->deckOfCards = new Deck(12);
    context->gamePlayers;

    //startup phase
    while(context->gameMap == nullptr){
        startupPhase(*context, "loadmap");
        startupPhase(*context, "validateMap");
    }
    startupPhase(*context,"addplayer"); //add 5 players
    startupPhase(*context, "gamestart");

    for(Player* player : context->gamePlayers){
        player->setContext(context);
    }

    //Create strategies
    PlayerStrategy* human = new HumanPlayerStrategy();
    PlayerStrategy* aggressive = new AggressivePlayerStrategy();
    PlayerStrategy* benevolent = new BenevolentPlayerStrategy();
    PlayerStrategy* neutral = new NeutralPlayerStrategy();
    PlayerStrategy* cheater = new CheaterPlayerStrategy();

    cout << "Player0 set to Human strategy" << endl;
    context->gamePlayers.at(0)->setStrategy(human);

    cout << "Player1 set to Aggressive strategy" << endl;
    context->gamePlayers.at(1)->setStrategy(aggressive);

    cout << "Player2 set to Benevolent strategy" << endl;
    context->gamePlayers.at(2)->setStrategy(benevolent);

    cout << "Player3 set to Neutral strategy" << endl;
    context->gamePlayers.at(3)->setStrategy(neutral);

    cout << "Player4 set to Cheater strategy" << endl;
    context->gamePlayers.at(4)->setStrategy(cheater);

    //Show issueOrder for Human
    //Show issueOrder for Aggressive
    //Show issueOrder for Benevolent
    //Show issueOrder for Neutral
    //Show issueOrder for Cheater

};
