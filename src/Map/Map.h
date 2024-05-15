#pragma once
#ifndef MAP_H
#define MAP_H
#include <stdio.h>
#include "../Player/Player.h"
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * TODO: reset player to PLAYER object and not string
 */
using namespace std;

// Forward declaration of Player class
class Player;
class Continent;

// TERRITORY
struct AttackResult
{
    bool territoryConquered;
    bool shouldAwardCard;
};

class Territory
{

private:
    std::string *territoryName;
    int *coordX;
    int *coordY;
    std::vector<Territory *> neighbors;
    Continent *continent;
    Player *ownerPlayer;
    int *numberOfArmies;
    int territoryPriority;

public:
    // Constructors: Default, parametized and copy
    Territory();
    Territory(const std::string &name, int coordX, int coordY, Continent *newContinent);
    Territory(Territory *otherTerr);

    // Assignment operator
    Territory &operator=(const Territory &other);

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Territory *territory);

    // Methods
    void addNeighbors(Territory *neighbor);

    // Getters
    std::string getTerritoryName() const;
    int getCoordX() const;
    int getCoordY() const;
    std::vector<Territory *> getNeighbors() const;
    Continent *getContinent() const;
    Player *getOwnerPlayer() const;
    int getNumberOfArmies() const;
    int getPriority();

    // Setters
    void setTerritoryName(const std::string &name);
    void setCoordX(int x);
    void setCoordY(int y);
    void setNeighbors(const std::vector<Territory *> &neighbors);
    void setContinent(Continent *newContinent);
    void setOwnerPlayer(Player *owner);
    void setNumberOfArmies(int armies);
    void setPriority(int);

    // A method to add armies to a territory
    void addArmiesToTerritory(int *numOfArmies);

    // A method to remove armies from a territory
    void removeArmiesFromTerritory(int *numOfArmies);

    // A method to check if a territory is adjacent to another territory
    bool territoriesAreAdjacent(Territory *target);

    // A method to simulate an attack between two territories if they are adjacent, the source territory is owned by the current player and the target territory is owned by another player
    AttackResult simulateAttack(Territory *target, int *attackingArmies);

    // A mmethod to bomb a territory and remove half of its armies
    void bombTerritory();

    // Destructor
    ~Territory();
};

// Class Continent

class Continent
{
private:
    std::string *continentID;
    std::vector<Territory *> continentTerritories;
    int *numOfPoints;

public:
    // Constructors
    Continent();
    Continent(const std::string &continentID, const int numOfPoints);
    Continent(const Continent &other);

    // Assignment operator
    Continent &operator=(const Continent &other);

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Continent *continent);

    // Methods
    void addTerritories(Territory *newTerritory);

    // Getters
    std::string getContinentName() const;
    std::vector<Territory *> getContinentTerritories() const;
    int getNumOfPoints() const;

    // Setters
    void setContinentID(const std::string &newContinentID);
    void setContinentTerritories(const std::vector<Territory *> &territories);
    void setNumOfPoints(int newNumOfArmies);

    // Destructor
    ~Continent();
};

// Map Class

class Map
{

private:
    std::string *mapName;
    std::vector<Continent *> mapContinents;
    std::vector<Territory *> mapTerritories;

public:
    // Constructors
    Map();
    Map(std::string &mapName);
    Map(const Map &other);

    // Assignment operator
    Map &operator=(const Map &other);

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Map *map);

    // Methods
    void addTerritories(Territory *newTerr);
    void addContinents(Continent *newCon);

    bool isMapValid();
    bool isMapConnected();
    bool isContinentConnected();
    bool dfsContinent(Continent *continent);
    bool territoryHasOneContinent();
    bool validate();

    // Getters
    std::string getMapName() const;
    std::vector<Territory *> getMapTerritories() const;
    std::vector<Continent *> getMapContinents() const;

    // Setters
    void setMapName(const std::string &newMapName);
    void setMapTerritories(const std::vector<Territory *> &newTerritories);
    void setMapContinents(const std::vector<Continent *> &newContinents);

    // Destructor
    ~Map();
};

// Class MapLoader
class MapLoader
{
public:
    //    Methods
    static Map *loadMap(std::string filePath);
    static bool isMapFileValid(string fullPath);

    // Destructor
    ~MapLoader();
};

#endif // MAP_H