//
// Created by Krish on 11/12/2023.
//

// GameContext.h
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "Player.h"
#include "Cards.h"
#include "Map.h"

class Map;
class Deck;
class Player;

class GameContext {
public:
    Map* gameMap;
    static std::vector<Player*> gamePlayers;
    Deck* deckOfCards;
};

#endif // GAME_CONTEXT_H

