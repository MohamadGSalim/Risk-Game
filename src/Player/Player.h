#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <memory>
#include <unordered_set>

#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../OrdersList/Orders.h"
#include "../PlayerStrategy/PlayerStrategies.h"
#include "../GameEngine/GameContext.h"

using namespace std;

class Territory;
class Hand;
class Deck;
class OrdersList;

class Player
{
private:
    std::string *player_id;
    std::vector<Territory *> player_territories;
    std::vector<Territory *> enemy_territories;
    Hand *player_hand; // A hand of cards for the player
    Deck *player_deck; // A reference to the game's deck
    std::shared_ptr<OrdersList> player_orders;
    int reinforcementPool;
    // Stores player IDs with whom this player has active negotiations
    std::unordered_set<std::string> activeNegotiations;
    PlayerStrategy* strategy;
    GameContext* gameContext;

public:
    // Default constructor
    Player();

    Player(std::string id);

    // Parameterized constructor
    Player(std::string id,
           std::vector<Territory *> territories,
           std::vector<Territory *> enemy,
           Hand *hand,
           Deck *deck,
           std::shared_ptr<OrdersList> orders,
           std::initializer_list<std::string> activeNegotiationsInit,
           PlayerStrategy* strategy);

    // Copy constructor
    Player(const Player &player_copy);

    // Destructor
    ~Player();

    // Assignment operator
    Player &operator=(const Player &player_copy);

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &out, const Player &p);

    // Returns a list of territories that are to be defended
    std::vector<Territory *> toDefend();

    // Returns a list of territories that are to be attacked
    std::vector<Territory *> toAttack();

    // Getters / Setters
    std::string getPlayerID();

    Hand *getHand() const;
    void setHand(Hand *hand);

    Deck *getDeck() const;
    void setDeck(Deck *deck);

    const std::vector<Territory *> &getPlayerTerritories() const;
    void setPlayerTerritories(const std::vector<Territory *> &territories);

    const std::vector<Territory *> &getEnemyTerritories() const;
    void setEnemyTerritories(const std::vector<Territory *> &territories);

    int getReinforcementPool();
    void setReinforcementPool(int i);

    std::shared_ptr<OrdersList> getOrderList();
    void setOrderList(std::shared_ptr<OrdersList>);

    PlayerStrategy *getStrategy() const;
    void setStrategy(PlayerStrategy *strategy);

    GameContext* getContext();
    void setContext(GameContext* newContext);

    // Territory methods
    // A Method to check if a player owns a specific territory
    bool playerOwnsTerritory(Player *player, Territory *territory);
    int numOfTerritories();
    bool isValidTerritory(std::string, std::vector<Territory *>);
    Territory getTerritoryByName(std::string, std::vector<Territory *>);

    // Reinforcement methods
    void addToReinforcementPool(int i);
    bool noReinforcementsLeft();
    void printDeployAllReinforcements();
    void printNoMoreReinforcements();

    // Order methods
    // Creates an Order object and puts it in the player’s list of orders
    bool issueOrder();
    void createOrder(int orderNumber);
    bool hasCard(std::string);

    bool compareByPriority(Territory, Territory);

    // A Method to award a card from the deck to the player
    void awardCard();

    // Getters and setters for new instance variables
    void setPlayerID(string name);
    void addTerritory(Territory* newTerr);
    
    // Print cards
    void printDontHaveCard();

    // Methods to handle negotiations
    void startNegotiation(const std::string &targetPlayerId);
    bool isNegotiationActive(const std::string &targetPlayerId) const;
    void endNegotiation(const std::string &targetPlayerId);

    //Remove card from hand
    void removeCard(string cardName);

    //NEW A3 STRATEGY PATTERN ISSUEORDER
    void issueOrder(string orderName);
};

#endif // PLAYER_H
