//
// Created by Krish on 11/21/2023.
//
#pragma once
#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../OrdersList/Orders.h"
using namespace std;



//calling the class Player here so the compiler is forced to compile it first
class Player;

class PlayerStrategy{
    protected:
//        Player* player;

    public:
//        explicit PlayerStrategy(Player* p);
        virtual void issueOrder(Player* p, string orderName) const = 0;
        virtual vector<Territory* > toAttack(Player* p) const = 0;
        virtual vector<Territory* > toDefend(Player* p) const = 0;
        virtual string getStrategyName() const = 0;
//        virtual ~PlayerStrategies() = default;
};

class HumanPlayerStrategy: public PlayerStrategy{

    public:
    void issueOrder(Player* p, string orderName) const override;
    vector<Territory* > toAttack(Player* p) const override;
    vector<Territory* > toDefend(Player* p) const override;
    string getStrategyName() const override;

};

class AggressivePlayerStrategy: public PlayerStrategy{

    public:
    void issueOrder(Player* p, string orderName) const override;
    vector<Territory* > toAttack(Player* p) const override;
    vector<Territory* > toDefend(Player* p) const override;
    string getStrategyName() const override;

};

class BenevolentPlayerStrategy: public PlayerStrategy{

    public:
    void issueOrder(Player* p, string orderName) const override;
    vector<Territory* > toAttack(Player* p) const override;
    vector<Territory* > toDefend(Player* p) const override;
    string getStrategyName() const override;

};

class NeutralPlayerStrategy: public PlayerStrategy{

    public:
    void issueOrder(Player* p, string orderName) const override;
    vector<Territory* > toAttack(Player* p) const override;
    vector<Territory* > toDefend(Player* p) const override;
    string getStrategyName() const override;

};

class CheaterPlayerStrategy: public PlayerStrategy{

public:
    void issueOrder(Player* p, string orderName) const override;
    vector<Territory* > toAttack(Player* p) const override;
    vector<Territory* > toDefend(Player* p) const override;
    string getStrategyName() const override;

};

#endif //PLAYERSTRATEGIES_H
