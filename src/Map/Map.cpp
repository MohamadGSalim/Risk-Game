#include "Map.h"
#include "../Player/Player.h"
#include <iostream>
#include <fstream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding the random number generator with time

/**
 * TODO: reset player to PLAYER object and not string
 */

//-----------------------------------------------------------
// TERRITORY CLASS
//-----------------------------------------------------------

// Constructors
Territory::Territory()
{
    // Default constructor implementation
    this->territoryName = new std::string("");
    this->coordX = new int(0);
    this->coordY = new int(0);
    this->neighbors = std::vector<Territory *>();
    this->continent = new Continent();
    this->ownerPlayer = new Player();
    this->numberOfArmies = new int(0);
}

Territory::Territory(const std::string &name, int coordX, int coordY, Continent *newContinent)
{
    // Constructor with parameters implementation
    this->territoryName = new std::string(name);
    this->coordX = new int(coordX);
    this->coordY = new int(coordY);
    this->neighbors = std::vector<Territory *>();
    this->continent = newContinent;
    this->ownerPlayer = new Player();
    this->numberOfArmies = new int(0);
    territoryPriority = 0;
}

Territory::Territory(Territory *otherTerr)
{
    territoryName = new std::string(otherTerr->getTerritoryName());
    coordX = new int(otherTerr->getCoordX());
    coordY = new int(otherTerr->getCoordY());
    neighbors = otherTerr->getNeighbors();
    continent = otherTerr->getContinent();
    ownerPlayer = otherTerr->getOwnerPlayer();
    numberOfArmies = new int(otherTerr->getNumberOfArmies());
}

// Assignment operator
Territory &Territory::operator=(const Territory &other)
{
    if (this != &other)
    {
        delete territoryName;
        delete coordX;
        delete coordY;
        delete ownerPlayer;
        delete numberOfArmies;

        territoryName = new std::string(*other.territoryName);
        coordX = new int(*other.coordX);
        coordY = new int(*other.coordY);
        neighbors = other.neighbors;
        continent = other.continent;
        ownerPlayer = other.ownerPlayer;
        numberOfArmies = new int(*other.numberOfArmies);
    }
    return *this;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, const Territory *territory)
{
    os << "\n----------------territory----------------"
            << "\n";
    os << "Territory Name: " << territory->getTerritoryName() << "\n";
    os << "Coordinates: (" << territory->getCoordX() << ", " << territory->getCoordY() << ")\n";
    os << "Number of Armies: " << territory->getNumberOfArmies() << "\n";
    os << "Neighbors: ";
    for (const auto &neighbor : territory->getNeighbors())
    {
        os << neighbor->getTerritoryName() << " ";
    }
    os << "\n";
    os << "----------------territory----------------"
            << "\n";
    return os;
}

void Territory::addNeighbors(Territory *neighbor)
{
    this->neighbors.push_back(neighbor);
}

// Getters
std::string Territory::getTerritoryName() const
{
    return *territoryName;
}

int Territory::getCoordX() const
{
    return *coordX;
}

int Territory::getCoordY() const
{
    return *coordY;
}

std::vector<Territory *> Territory::getNeighbors() const
{
    return neighbors;
}

Continent *Territory::getContinent() const
{
    return continent;
}

Player *Territory::getOwnerPlayer() const
{
    return ownerPlayer;
}

int Territory::getNumberOfArmies() const
{
    return *numberOfArmies;
}

// Setters
void Territory::setTerritoryName(const std::string &name)
{
    *territoryName = name;
}

void Territory::setCoordX(int x)
{
    *coordX = x;
}

void Territory::setCoordY(int y)
{
    *coordY = y;
}

void Territory::setNeighbors(const std::vector<Territory *> &newNeighbors)
{
    neighbors = newNeighbors;
}

void Territory::setContinent(Continent *newContinent)
{
    continent = newContinent;
}

void Territory::setOwnerPlayer(Player *owner)
{
    ownerPlayer = owner;
}

void Territory::setNumberOfArmies(int armies)
{
    *numberOfArmies = armies;
}

void Territory::setPriority(int i)
{
    territoryPriority = i;
}

int Territory::getPriority()
{
    return territoryPriority;
}

/**
 * A method to add armies to this territory
 * @param numOfArmies
 */

void Territory::addArmiesToTerritory(int *numOfArmies)
{
    if (numOfArmies != nullptr)
    {
        *(this->numberOfArmies) += *numOfArmies;
    }
}

/**
 * A method to remove armies from this territory
 * @param numOfArmies
 */

void Territory::removeArmiesFromTerritory(int *numOfArmies)
{
    if (numOfArmies != nullptr)
    {
        *(this->numberOfArmies) -= *numOfArmies;
    }
}

/**
 * This method checks if two territories are adjacent to each other by checking if the target territory is in the source's neighbors vector
 * @param target The territory that is being checked against
 * @return true if the target territory is in the source's neighbors vector, false otherwise
 */
bool Territory::territoriesAreAdjacent(Territory *target)
{
    if (!target)
    {
        return false; // Return false if either this or target is null
    }

    // Check if the target territory is in this territory's neighbors vector
    for (Territory *neighbor : this->getNeighbors())
    {
        if (neighbor == target)
        {
            return true; // The target is a neighbor of this territory
        }
    }

    return false; // The target is not a neighbor of this territory
}

/**
 * This method simulates an attack between two territories if they are adjacent, the source territory is owned by the current player, and the target territory is owned by another player
 * @param target The territory that is being attacked
 * @param attackingArmies The number of armies that are attacking
 * @return An AttackResult struct that contains a boolean indicating whether the territory was conquered and a boolean indicating whether a card should be awarded
 */

AttackResult Territory::simulateAttack(Territory *target, int *attackingArmies)
{
    AttackResult result = {false, false};

    Player *attacker = this->getOwnerPlayer();
    Player *defender = target->getOwnerPlayer();

    // Check for valid input
    if (!attacker || !defender || attacker == defender || *attackingArmies <= 0 || !this->territoriesAreAdjacent(target))
    {
        return result; // Invalid conditions for attack
    }

    // Check for active negotiation between the attacker and defender
    if (attacker->isNegotiationActive(defender->getPlayerID()))
    {
        std::cout << "Attack not allowed due to active negotiation between " << attacker->getPlayerID() << " and " << defender->getPlayerID() << "." << std::endl;
        return result; // Attack is not allowed due to negotiation
    }

    // Seed the random number generator
    std::srand(std::time(0));

    int attackingArmyCount = *attackingArmies;
    int defendingArmyCount = target->getNumberOfArmies();

    // Simulate the battle
    while (attackingArmyCount > 0 && defendingArmyCount > 0)
    {
        // 60% chance for each attacking army unit to kill a defending unit
        for (int i = 0; i < attackingArmyCount; ++i)
        {
            if (std::rand() % 100 < 60)
            {
                defendingArmyCount--;
                if (defendingArmyCount <= 0)
                {
                    break; // All defenders are eliminated
                }
            }
        }

        // 70% chance for each defending army unit to kill an attacking unit
        for (int i = 0; i < defendingArmyCount; ++i)
        {
            if (std::rand() % 100 < 70)
            {
                attackingArmyCount--;
                if (attackingArmyCount <= 0)
                {
                    break; // All attackers are eliminated
                }
            }
        }
    }

    // Update the number of armies in the source and target territories

    if (defendingArmyCount <= 0 && attackingArmyCount > 0)
    {
        // Attacker wins and conquers the territory
        target->setOwnerPlayer(this->getOwnerPlayer());
        // Update the attacking armies to reflect those that survived
        this->setNumberOfArmies(attackingArmyCount);
        // Update the defending armies to reflect those that survived (zero in this case)
        target->setNumberOfArmies(defendingArmyCount);
        result.territoryConquered = true;
        result.shouldAwardCard = true;
    }

    return result;
}

/**
 * This method simulates a bomb order on a territory by removing half of the army units in the territory
 * @param target The territory that is being bombed
 * @return void
 */
void Territory::bombTerritory()
{
    if (numberOfArmies != nullptr)
    {
        *numberOfArmies = *numberOfArmies / 2; // Remove half of the army units
    }
}

// Destructor
Territory::~Territory()
{
    // Delete members in reverse order of creation

    if (numberOfArmies == nullptr){
        delete numberOfArmies;
        numberOfArmies = nullptr;
    }

    if (ownerPlayer == nullptr){
        delete ownerPlayer;
        ownerPlayer = nullptr;
    }

    if (continent == nullptr)
    {
        delete continent;
        continent = nullptr;
    }

    // Delete each neighbor in the vector
    for (auto n : neighbors)
    {
        delete n;
    }

    // Clear the vector after deleting its elements
    neighbors.clear();

    delete coordY;
    coordY = nullptr;

    delete coordX;
    coordX = nullptr;

    if (territoryName == nullptr){
        delete territoryName;
        territoryName = nullptr;
    }
}

//-----------------------------------------------------------
// CONTINENT CLASS
//-----------------------------------------------------------

// Constructors

// Default constructor
Continent::Continent()
{
    this->continentID = new std::string("");
    this->numOfPoints = new int(0);
    this->continentTerritories = std::vector<Territory *>();
}

// Parameterized constructor
Continent::Continent(const std::string &continentID, const int numOfPoints)
{
    this->continentID = new std::string(continentID);
    this->numOfPoints = new int(numOfPoints);
    this->continentTerritories = std::vector<Territory *>();
}

// Copy Constructor
Continent::Continent(const Continent &other)
{
    this->continentID = new std::string(*other.continentID);
    this->continentTerritories = other.continentTerritories;
    this->numOfPoints = new int(*other.numOfPoints);
}

// Assignment operator
Continent &Continent::operator=(const Continent &other)
{
    if (this != &other)
    {
        delete continentID;
        delete numOfPoints;

        continentID = new std::string(*other.continentID);
        continentTerritories = other.continentTerritories; // Assuming Territory objects are relatively lightweight
        numOfPoints = new int(*other.numOfPoints);
    }
    return *this;
}

/**
 * addTerritories is a function that pushes a new territory into the territory* vector of a continent
 * @param newTerritory
 */
void Continent::addTerritories(Territory *newTerritory)
{
    continentTerritories.push_back(newTerritory);
}

// Getters
std::string Continent::getContinentName() const
{
    return *continentID;
}

std::vector<Territory *> Continent::getContinentTerritories() const
{
    return continentTerritories;
}

int Continent::getNumOfPoints() const
{
    return *numOfPoints;
}

// Setters
void Continent::setContinentID(const std::string &newContinentID)
{
    *continentID = newContinentID;
}

// Setter for continent territories
void Continent::setContinentTerritories(const std::vector<Territory *> &territories)
{
    this->continentTerritories = territories;
}

void Continent::setNumOfPoints(int newNumOfPoints)
{
    *numOfPoints = newNumOfPoints;
}

// Destructor
Continent::~Continent()
{

    for (auto n : continentTerritories)
    {
        delete n;
        n = nullptr;
    }
    continentTerritories.clear();

    delete numOfPoints;
    numOfPoints = nullptr;

    delete continentID;
    continentID = nullptr;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, const Continent *continent)
{
    os << "----------------continent----------------"
       << "\n";
    os << "Continent ID: " << continent->getContinentName() << "\n";
    os << "Number of Points: " << continent->getNumOfPoints() << "\n";
    os << "Territories: ";
    for (const auto &territory : continent->getContinentTerritories())
    {
        os << territory->getTerritoryName() << " ";
    }
    os << "\n";
    os << "----------------continent----------------"
       << "\n";
    return os;
}

//-----------------------------------------------------------
// MAP CLASS
//-----------------------------------------------------------

// CONSTRUCTORS
Map::Map()
{
    this->mapName = new std::string("");
    this->mapContinents = std::vector<Continent *>();
    this->mapTerritories = std::vector<Territory *>();
}

Map::Map(string &mapName)
{
    this->mapName = new std::string(mapName);
    this->mapContinents = std::vector<Continent *>();
    this->mapTerritories = std::vector<Territory *>();
}

// Copy constructor
Map::Map(const Map &other)
{
    mapName = new std::string(*other.mapName);
    mapContinents = other.mapContinents;   // Assuming Continent objects are relatively lightweight
    mapTerritories = other.mapTerritories; // Assuming Territory objects are relatively lightweight
}

// Assignment operator
Map &Map::operator=(const Map &other)
{
    if (this != &other)
    {
        delete mapName;

        mapName = new std::string(*other.mapName);
        mapContinents = other.mapContinents;   // Assuming Continent objects are relatively lightweight
        mapTerritories = other.mapTerritories; // Assuming Territory objects are relatively lightweight
    }
    return *this;
}

/**
 * addTerritories pushes new territories in the mapTerritories vector
 * @param newTerr
 */
void Map::addTerritories(Territory *newTerr)
{
    this->mapTerritories.push_back(newTerr);
}

/**
 * addContinents adds new continents to the mapContinents vector
 * @param newCon
 */
void Map::addContinents(Continent *newCon)
{
    this->mapContinents.push_back(newCon);
}

// GETTERS
std::string Map::getMapName() const
{
    return *mapName;
}

std::vector<Territory *> Map::getMapTerritories() const
{
    return mapTerritories;
}

std::vector<Continent *> Map::getMapContinents() const
{
    return mapContinents;
}

// SETTERS
void Map::setMapName(const std::string &newMapName)
{
    *mapName = newMapName;
}

void Map::setMapTerritories(const vector<Territory *> &newTerritories)
{
    mapTerritories = newTerritories;
}

void Map::setMapContinents(const vector<Continent *> &newContinents)
{
    mapContinents = newContinents;
}

/**
 * In the loadMap() function, if there is an error in processing the file, the function returns a nullptr.
 * This function is a helper method that checks if the map in question is a map object or a nullptr.
 * @return if the map is a nullptr, then return false.
 */
bool Map::isMapValid()
{
    if (this == nullptr)
    {
        return false;
    }
    return true;
}

/**
 * isMapConnected
 * using DFS isMapConnected() goes through the list of territories in a map and
 * runs through all of their respective neighbors, it checks if every node in the map is reachable
 * @return true if all nodes have been visited, false otherwise
 */

bool Map::isMapConnected()
{
    bool answer;
    std::set<Territory *> visitedTerritories;
    std::stack<Territory *> neighborStack;

    // Add initial territory in visited vector and push its neighbors in the stack
    Territory *startTerr = this->getMapTerritories().at(0);
    visitedTerritories.insert(startTerr);

    for (int i = 0; i < startTerr->getNeighbors().size(); i++)
    {
        Territory *neighbor = startTerr->getNeighbors().at(i);
        neighborStack.push(neighbor);
    }

    // Go through the stack until it is empty
    while (!neighborStack.empty())
    {
        Territory *targetTerr = neighborStack.top();
        neighborStack.pop();

        if (visitedTerritories.find(targetTerr) == visitedTerritories.end())
        {
            // Node is not visited, mark it as visited
            visitedTerritories.insert(targetTerr);

            // Push unvisited neighbors onto the stack
            for (int i = 0; i < targetTerr->getNeighbors().size(); i++)
            {
                Territory *neighbor = targetTerr->getNeighbors().at(i);
                if (visitedTerritories.find(neighbor) == visitedTerritories.end())
                {
                    neighborStack.push(neighbor);
                }
            }
        }
    }

    // With depth-first search if the number of visited territories is the same that the map contains,
    // then the graph is connected
    if (visitedTerritories.size() == this->getMapTerritories().size())
    {
        answer = true;
        return answer;
    }
    else
    {
        answer = false;
        std::cout << "Some nodes have not been visited...Map disconnected graph" << std::endl;
        return answer;
    }
}

/**
 * This method checks if all the continents within a map are subgraphs using a helper method, if
 * @return true if all continents are subgraphs, returns false otherwise
 */
bool Map::isContinentConnected()
{
    for (Continent *targetContinent : this->getMapContinents())
    {
        if (!this->dfsContinent(targetContinent))
        {
            return false;
        }
    }
    return true;
}

/**
 * dfsContinent is a helper method that checks if the territories within a continent are reachable and therefore a subgraph
 * @param continent
 * @return if territories are a subgraph, return true, else false
 */
bool Map::dfsContinent(Continent *continent)
{
    bool answer;
    std::set<Territory *> visitedTerritories;
    std::stack<Territory *> neighborStack;

    // Add initial territory in visited vector and push its neighbors in the stack
    Territory *targetTerr = continent->getContinentTerritories().at(0);
    visitedTerritories.insert(targetTerr);

    for (int i = 0; i < targetTerr->getNeighbors().size(); i++)
    {
        Territory *neighbor = targetTerr->getNeighbors().at(i);

        // check that the neighbor is in the continent we are checking
        if (neighbor->getContinent()->getContinentName() == continent->getContinentName())
        {
            // if the neighbor is in the same continent, push it onto the stack
            neighborStack.push(neighbor);
        }
    }

    while (!neighborStack.empty())
    {
        targetTerr = neighborStack.top();
        neighborStack.pop();
        if (visitedTerritories.find(targetTerr) == visitedTerritories.end())
        {
            // Node is not visited, mark it as visited
            visitedTerritories.insert(targetTerr);

            // Push unvisited neighbors onto the stack
            for (int i = 0; i < targetTerr->getNeighbors().size(); i++)
            {
                Territory *neighbor = targetTerr->getNeighbors().at(i);

                // if it has not been visited yet AND is within the same continent, push in stack
                if ((visitedTerritories.find(neighbor) == visitedTerritories.end()) &&
                    neighbor->getContinent()->getContinentName() == continent->getContinentName())
                {

                    neighborStack.push(neighbor);
                }
            }
        }
    }

    // With depth-first search if the number of visited territories is the same that the continent contains,
    // then the graph is connected
    if (visitedTerritories.size() == continent->getContinentTerritories().size())
    {
        answer = true;
        return answer;
    }
    else
    {
        answer = false;
        std::cout << "Some nodes have not been visited...Continent disconnected graph" << std::endl;
        return answer;
    }
}

/**
 * territoryHasOneContinent checks if every territory in a map only belongs to a single continent.
 * If a territory is initialized twice within the same file and it is assigned two different territory, it
 * will detect this and return false. It uses a map tracker to keep a pair of territory and continent*.
 * @return
 */
bool Map::territoryHasOneContinent()
{

    std::map<string, Continent *> tracker;

    for (Territory *terr : this->getMapTerritories())
    {
        if (tracker.find(terr->getTerritoryName()) == tracker.end())
        {
            // not found
            tracker.insert(std::make_pair(terr->getTerritoryName(), terr->getContinent()));
        }
        else
        {
            // found
            if (tracker[terr->getTerritoryName()] != terr->getContinent())
            {
                // if the current continent stored in the map for this territory doesn't match the
                // territory's continent
                std::cout << "Territory was defined more than once and was given different Continents!" << std::endl;
                return false;
            }
        }
    }

    return true;
}

/**
 * validate checks each requirement:
 * 0) is the map a nullptr, if yes invalid
 * 1) is the map connected
 * 2) are the continents in the map connected
 * 3) does each territory only belong to a single continent
 * @return if any of the conditions arent met, return false
 */
bool Map::validate()
{
    bool answer = true;
    if (!this->isMapValid())
    {
        answer = false;
        cout << "This map is empty and is not valid as it came from an invalid file." << endl;
        return answer;
    }
    if (!this->isMapConnected())
    {
        answer = false;
        cout << "1. This map is not fully connected and is not valid." << endl;
    }
    else
    {
        cout << "1. This map is fully connected." << endl;
    }
    if (!this->isContinentConnected())
    {
        answer = false;
        cout << "2. The continents in this map are not all connected and is not valid." << endl;
    }
    else
    {
        cout << "2. The continents of this map are all subgraphs of the map." << endl;
    }
    if (!this->territoryHasOneContinent())
    {
        answer = false;
        cout << "3. The territories in this map have been assigned multiple continents, and is not valid." << endl;
    }
    else
    {
        cout << "3. The territories in this map all belong to a single continent." << endl;
    }

    return answer;
}

// Destructor
Map::~Map()
{
    delete mapName;
    mapName = nullptr;

    for (auto n : mapTerritories)
    {
        delete n;
        n = nullptr;
    }
    mapTerritories.clear();

    for (auto n : mapContinents)
    {
        delete n;
        n = nullptr;
    }
    mapContinents.clear();
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, const Map *map)
{
    if (map == nullptr)
    {
        os << "----------------map----------------"
           << "\n";
        os << "There is no map!\n";
        os << "----------------map----------------"
                << "\n";
        return os;
    }
    os << "----------------map----------------"
            << "\n";
    os << "Map Name: " << map->getMapName() << "\n";
    os << "Continents: ";
    for (const auto &continent : map->getMapContinents())
    {
        os << continent->getContinentName() << ", ";
    }
    os << "\n";
    os << "Territories: ";
    for (const auto &territory : map->getMapTerritories())
    {
        os << territory->getTerritoryName() << ", ";
    }
    os << "\n";
    os << "----------------map----------------"
            << "\n";
    return os;
}

/**
 * Helper method
 * Remove surrounding whitespace from a std::string.
 * @param s The string to be modified.
 * @param t The set of characters to delete from each end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
std::string &trim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

//-----------------------------------------------------------
// MAPLOADER CLASS
//-----------------------------------------------------------

/**
 * This method takes a fullPath to a .map file and reads it line by line in two iterations
 * 1. The first iterations goes through the continents and territories and creates new objects in memory and assigned to a map object
 * 2. The second iteration goes through the neighbors of each territory, updating the list of neighbors of said territory with the right pointers
 * This function uses a dictionary for both continents and territories to keep track of the pointers before adding them to their respective vectors
 * @param filePath
 * @return
 */
Map *MapLoader::loadMap(std::string filePath)
{
    // Open a file for reading
    std::ifstream inputFile(filePath);

    // Get file name for map initialization
    std::size_t found = filePath.find_last_of("/\\");
    std::cout << " path: " << filePath.substr(0, found) << '\n';
    std::string fileName = filePath.substr(found + 1);
    std::cout << " file: " << filePath.substr(found + 1) << '\n';

    // create new Map
    Map *newMap = new Map(fileName);

    // Create dictionaries to keep track of references
    std::map<std::string, Continent *> continentDict;
    std::map<std::string, Territory *> territoryDict;

    // Check if the file is successfully opened
    if (!inputFile.is_open() || !MapLoader().isMapFileValid(filePath))
    {
        std::cout << "Error opening the file!" << std::endl;
        inputFile.close();
        continentDict.clear();
        territoryDict.clear();
        return nullptr;
    }
    else
    {
        // If file is opened
        std::string line;

        // start reading through file
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            std::string section;
            iss >> section;

            if (section == "[Map]")
            {
                while (std::getline(inputFile, line) && !line.empty())
                {
                    // skip map portion of the file as the information isn't relevant
                }
            }
            else if (section == "[Continents]")
            {
                // Initializing the continents of the Map
                while (std::getline(inputFile, line) && !line.empty())
                {
                    std::stringstream ss(line);
                    std::string token;
                    std::getline(ss, token, '='); // Split by '='

                    std::string name;
                    int number;

                    if (ss >> number)
                    {
                        name = token;

                        Continent *newContinent = new Continent(name, number);
                        newMap->addContinents(newContinent);

                        continentDict.insert(std::make_pair(newContinent->getContinentName(), newContinent));

                        // Checking that it was inserted in the dictionary correctly
                        Continent *retrievedContinent = continentDict[name];
                        if (!retrievedContinent)
                        {
                            std::cout << "Continent not found in the map." << std::endl;
                            inputFile.close();
                            continentDict.clear();
                            territoryDict.clear();
                            delete newMap; // Clean up the partially constructed map
                            return nullptr;
                        }
                    }
                    else
                    {
                        std::cout << "Error parsing line: " << line << std::endl;
                        inputFile.close();
                        continentDict.clear();
                        territoryDict.clear();
                        delete newMap; // Clean up the partially constructed map
                        return nullptr;
                    }
                }
            }
            else if (section == "[Territories]")
            {
                // Get territories name, x-y coordinates and continent
                // update continent to add territory to its vector
                while (std::getline(inputFile, line))
                {
                    if (!line.empty())
                    {
                        std::vector<string> mapInfo;

                        stringstream s_stream(line);
                        while (s_stream.good())
                        {
                            string substr;
                            getline(s_stream, substr, ',');
                            mapInfo.push_back(substr);
                        }
                        /**
                         * 0 - name of territory
                         * 1- x coordinate
                         * 2 - y coordinate
                         * 3 - continent name
                         * 4 - ... neighbors
                         * */

                        if (mapInfo.size() < 5)
                        {
                            std::cout << "Error in map file, territory has no neighbors!" << std::endl;
                            inputFile.close();
                            continentDict.clear();
                            territoryDict.clear();
                            delete newMap; // Clean up the partially constructed map
                            return nullptr;
                        }

                        string continentID = trim(mapInfo.at(3));
                        Continent *targetedCont = continentDict[continentID];
                        string name = trim(mapInfo.at(0));
                        int xcoord = stoi(trim(mapInfo.at(1)));
                        int ycoord = stoi(trim(mapInfo.at(2)));

                        Territory *newTerr = new Territory(name, xcoord, ycoord, targetedCont);

                        // add to Map
                        newMap->addTerritories(newTerr);

                        // update continent
                        targetedCont->addTerritories(newTerr);

                        // update territory dictionary for later
                        territoryDict.insert(std::make_pair(newTerr->getTerritoryName(), newTerr));
                    }
                }
            }
        }

        // Done with first iteration, starting second to populate neighbors
        inputFile.close();
        std::ifstream inputFile(filePath);

        if (!inputFile.is_open())
        {
            std::cout << "Error opening the file!" << std::endl;
            inputFile.close();
            continentDict.clear();
            territoryDict.clear();
            delete newMap; // Clean up the partially constructed map
            return nullptr;
        }
        else
        {
            // Read file, skip to Territories section
            std::string line;

            // start reading through file
            while (std::getline(inputFile, line))
            {
                std::istringstream iss(line);
                std::string section;
                iss >> section;

                if (section == "[Territories]")
                {
                    while (std::getline(inputFile, line))
                    {
                        if (!line.empty())
                        {
                            std::vector<string> mapInfo;

                            stringstream s_stream(line);
                            while (s_stream.good())
                            {
                                string substr;
                                getline(s_stream, substr, ',');
                                mapInfo.push_back(substr);
                            }
                            /**
                             * 0 - name of territory
                             * 1 - x coordinate
                             * 2 - y coordinate
                             * 3 - continent name
                             * 4 - neighbors till end
                             * */
                            Territory *targetedTerritory = territoryDict[trim(mapInfo.at(0))];

                            for (int i = 4; i < mapInfo.size(); i++)
                            {
                                Territory *neighbor = territoryDict[trim(mapInfo.at(i))];
                                targetedTerritory->addNeighbors(neighbor);
                            }
                        }
                    }
                }
            }
        }
    }

    // Close the file and clear memory to avoid leaks
    inputFile.close();
    continentDict.clear();
    territoryDict.clear();

    return newMap;
}

/**
 * Helper method for loadMap
 * Checks if the formatting of the map file is as expected.
 * First line must include [Map] followed by the map info
 * If the file has less than 11 lines it does not contain territory information and is invalid
 * It makes sure that every section "[Map]", "[Continent]" and "[Territories}" are present in the file
 * @param fullPath
 * @return true if file follows right template, false if not
 */
bool MapLoader::isMapFileValid(std::string fullPath)
{
    std::ifstream inputFile(fullPath);

    if (!inputFile.is_open())
    {
        std::cout << "Error opening the file!" << std::endl;
        return false;
    }

    std::string line;
    int lineCounter = 0;

    // start reading through file
    while (std::getline(inputFile, line))
    {
        lineCounter++;
    }

    if (lineCounter < 11)
    {
        std::cout << "File is not properly defined! Missing information." << std::endl;
        return false;
    }

    // reread file from beginning
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);

    std::getline(inputFile, line);
    if (line.find("[Map]") == std::string::npos)
    {
        std::cout << "File does not contain '[Map]' in the first line." << std::endl;
        return false;
    }

    // Skip the next 6 lines
    for (int i = 1; i < 7; ++i)
    {
        std::getline(inputFile, line);
    }

    // Check if the 8th line contains "[Continents]"
    std::getline(inputFile, line);
    if (line.find("[Continents]") == std::string::npos)
    {
        std::cout << "File does not contain '[Continents]' in the 8th line." << std::endl;
        return false;
    }

    // Check if there is a section named [Territories]
    bool hasTerritoriesSection = false;
    while (std::getline(inputFile, line))
    {
        if (line.find("[Territories]") != std::string::npos)
        {
            hasTerritoriesSection = true;
            break;
        }
    }

    if (!hasTerritoriesSection)
    {
        std::cout << "File does not contain a section named '[Territories]'." << std::endl;
        return false;
    }

    // Close the file
    inputFile.close();

    // If all checks passed, the file is considered valid
    return true;
}

MapLoader::~MapLoader()
{
}
