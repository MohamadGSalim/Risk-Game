#include "Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../LoggingObserver/LoggingObserver.h"
#include <vector>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <string>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>

using namespace std;

// ........... 0. ORDER ..............
// DEFAULT CONSTRUCTOR to initialize any data members or set up the object
Order::Order()
    : orderType(std::make_shared<std::string>("Order")), // Default value is "Order"
      issuingPlayer(nullptr), effect(std::make_shared<std::string>("No effect"))
{
    this->addObserver(&logger);
    logger.setObservedObject(this);
}

// PARAMETERIZED CONSTRUCTOR
Order::Order(const std::string &type, const std::string &effect)
    : orderType(std::make_shared<std::string>(type)), // Use the provided type
      issuingPlayer(nullptr), effect(std::make_shared<std::string>(effect))
{
    this->addObserver(&logger);
    logger.setObservedObject(this);
}

// PARAMETERIZED CONSTRUCTOR
Order::Order(const std::string &orderType, Player *issuingPlayer, const std::string &effect)
    : orderType(std::make_shared<std::string>(orderType)),
      issuingPlayer(issuingPlayer),
      effect(std::make_shared<std::string>(effect))
{
    this->addObserver(&logger);
    logger.setObservedObject(this);
}

// COPY CONSTRUCTOR
Order::Order(const Order &orderObject)
    : orderType(std::make_shared<std::string>(*orderObject.orderType)),
      issuingPlayer(orderObject.issuingPlayer) {} // Shallow copy is okay if not taking ownership

// ASSIGNMENT OPERATOR
Order &Order::operator=(const Order &orderObject)
{
    if (this != &orderObject)
    {
        orderType = std::make_shared<std::string>(*orderObject.orderType);
        // No need to delete the old Player* since we don't own it
        issuingPlayer = orderObject.issuingPlayer; // Shallow copy is okay if not taking ownership
    }
    return *this;
}

// Virtual print function to be overridden by derived classes
void Order::print(ostream &output) const
{
    output << "ORDER TYPE: [" << *orderType << "]" << endl;
    output << "--------------------------------------\n";
    output << "Issuing player: " << endl
           << *issuingPlayer << endl;
    output << "Effect: [" << (effect ? *effect : "No effect") << "]";
    cout << endl;
}

// STREAM INSERTION OPERATOR
ostream &operator<<(ostream &output, const Order &orderObject)
{
    orderObject.print(output);
    return output;
}

// STRING-TO-LOG
string Order::stringToLog() const
{
    return "Order Executed: " + *this->effect;
}

// GETTERS
string Order::getOrderType()
{
    return *this->orderType;
}

Player *Order::getIssuingPlayer() const
{
    return issuingPlayer;
}

string Order::getEffect()
{
    return *this->effect;
}

// SETTERS
void Order::setOrderType(const string &orderType)
{
    *(this->orderType) = orderType;
}

void Order::setIssuingPlayer(Player *issuingPlayer)
{
    // No need to delete the old Player* since we don't own it
    this->issuingPlayer = issuingPlayer;
}

void Order::setEffect(const string &effect)
{
    *(this->effect) = effect;
}

// ........... 0. ORDER ..............

// ........... 1. DEPLOY ..............
// DEFAULT CONSTRUCTOR to initialize any data members or set up the object
Deploy::Deploy()
    : Order("Deploy", "No effect"),
      numArmies(std::make_shared<int>(0)),
      targetTerritory(std::make_shared<Territory>()) {}

// PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object
Deploy::Deploy(Player *issuingPlayer, int numArmies, const Territory &targetTerritory)
    : Order("Deploy", issuingPlayer, "No effect"),
      numArmies(std::make_shared<int>(numArmies)),
      targetTerritory(std::make_shared<Territory>(targetTerritory))
{
}

// COPY CONSTRUCTOR to initialize an object from another object.
Deploy::Deploy(const Deploy &deployObject)
    : Order(deployObject),
      numArmies(make_shared<int>(*deployObject.numArmies)),
      targetTerritory(make_shared<Territory>(*deployObject.targetTerritory)) {}

// ASSIGNMENT OPERATOR to assign an object to another object.
Deploy &Deploy::operator=(const Deploy &deployObject)
{
    if (this != &deployObject)
    {
        Order::operator=(deployObject); // base class assignment operator
        numArmies = make_shared<int>(*deployObject.numArmies);
        targetTerritory = make_shared<Territory>(*deployObject.targetTerritory);
    }
    return *this;
}

// Virtual print function to be overridden by derived classes
void Deploy::print(ostream &output) const
{
    Order::print(output); // Call the base class print method.
    output << "[DEPLOY ORDER DETAILS]"
           << "\n";
    output << "Number of armies: " << *numArmies << "\n";
    output << "-----------------------------------------\n";
    output << "Target territory: " << targetTerritory.get() << "\n";
}

// STREAM INSERTION OPERATOR to print the contents of an object.
// ostream &operator<<(ostream &output, const Deploy &deployObject) {
//    deployObject.print(output);
//    return output;
//}

// VALIDATE
bool Deploy::validate()
{
    // Retrieve the issuing player as a pointer to Player
    Player *issuingPlayerPtr = this->getIssuingPlayer();

    // Check if targetTerritory and its owner are valid
    if (targetTerritory && targetTerritory->getOwnerPlayer() != nullptr)
    {
        // Retrieve the owner player as a pointer to Player
        Player *ownerPtr = targetTerritory->getOwnerPlayer();

        // Validate that the issuing player owns the target territory
        if (issuingPlayerPtr && ownerPtr->getPlayerID() == issuingPlayerPtr->getPlayerID())
        {
            return true; // The target territory is owned by the player who issued the deploy order.
        }
    }
    return false; // The target territory is not owned by the player who issued the deploy order or other conditions not met.
}

// EXECUTE
void Deploy::execute()
{
    if (this->validate())
    {
        // Get issuing player's reinforcement pool
        Player *issuingPlayerPtr = this->getIssuingPlayer();

        // Get the number of armies to deploy
        int numArmies = this->getNumArmies();

        // Proceed with the deployment
        targetTerritory->addArmiesToTerritory(&numArmies);

        std::cout << "Deployed " << numArmies << " armies to "
                  << targetTerritory->getTerritoryName() << "." << std::endl;
    }

    else
    {
        std::cout << "Deploy order is not valid and cannot be executed." << std::endl;
    }

    this->setEffect("Executed");

    this->notifyObservers();
}

// GETTERS
int Deploy::getNumArmies()
{
    return *this->numArmies;
}

Territory Deploy::getTargetTerritory()
{
    return *this->targetTerritory;
}

// SETTERS
void Deploy::setNumArmies(const int &numArmies)
{
    *(this->numArmies) = numArmies;
}

void Deploy::setTargetTerritory(const Territory &targetTerritory)
{
    *(this->targetTerritory) = targetTerritory;
}
// ........... 1. DEPLOY ..............

// ........... 2. ADVANCE .............
// DEFAULT CONSTRUCTOR to initialize any data members or set up the object
Advance::Advance()
    : Order("Advance", "No effect"),
      numArmies(make_shared<int>(0)),
      sourceTerritory(make_shared<Territory>()),
      targetTerritory(make_shared<Territory>()) {}

// PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object
Advance::Advance(Player *issuingPlayer, const int numArmies, const Territory &sourceTerritory, const Territory &targetTerritory)
    : Order("Advance", issuingPlayer, "No effect"),
      numArmies(make_shared<int>(numArmies)),
      sourceTerritory(make_shared<Territory>(sourceTerritory)),
      targetTerritory(make_shared<Territory>(targetTerritory)) {}

// COPY CONSTRUCTOR to initialize an object from another object.
Advance::Advance(const Advance &advanceObject)
    : Order(advanceObject),
      numArmies(make_shared<int>(*advanceObject.numArmies)),
      sourceTerritory(make_shared<Territory>(*advanceObject.sourceTerritory)),
      targetTerritory(make_shared<Territory>(*advanceObject.targetTerritory)) {}

// ASSIGNMENT OPERATOR to assign an object to another object.
Advance &Advance::operator=(const Advance &advanceObject)
{
    if (this != &advanceObject)
    {
        Order::operator=(advanceObject); // base class assignment operator
        numArmies = make_shared<int>(*advanceObject.numArmies);
        sourceTerritory = make_shared<Territory>(*advanceObject.sourceTerritory);
        targetTerritory = make_shared<Territory>(*advanceObject.targetTerritory);
    }
    return *this;
}

// Virtual print function to be overridden by derived classes
void Advance::print(ostream &output) const
{
    Order::print(output); // Call the base class print method.
    output << "[ADVANCE ORDER DETAILS]"
           << "\n";
    output << "Number of armies: " << *numArmies << "\n";
    output << "-----------------------------------------\n";
    output << "Source territory: " << sourceTerritory;
    output << "-----------------------------------------\n";
    output << "Target territory: " << targetTerritory;
}

// STREAM INSERTION OPERATOR to print the contents of an object.
// ostream &operator<<(ostream &output, const Advance &advanceObject) {
//    advanceObject.print(output);
//    return output;
//}

// VALIDATE
bool Advance::validate()
{
    // Get issuing player
    Player *issuingPlayerPtr = this->getIssuingPlayer();

    // Validate the source territory belongs to the issuing player
    if (sourceTerritory->getOwnerPlayer() != issuingPlayerPtr)
    {
        return false;
    }

    // Validate that source and target territories are adjacent
    if (!sourceTerritory->territoriesAreAdjacent(targetTerritory.get()))
    {
        return false;
    }

    return true;
}

// EXECUTE
void Advance::execute()
{
    if (!this->validate())
    {
        std::cout << "Advance order is not valid and cannot be executed." << std::endl;
        return;
    }

    // Get issuing player
    Player *issuingPlayerPtr = this->getIssuingPlayer();

    // Get the number of armies to advance
    int numArmies = this->getNumArmies();

    // Get the number of armies in the source territory
    int numArmiesInSourceTerritory = sourceTerritory->getNumberOfArmies();
    // Get the number of armies in the target territory
    int numArmiesInTargetTerritory = targetTerritory->getNumberOfArmies();

    // Update number of armies in the source territory
    int currentNumArmiesInSourceTerritory = numArmiesInSourceTerritory - numArmies;
    // Update number of armies in the target territory
    int currentNumArmiesInTargetTerritory = numArmiesInTargetTerritory + numArmies;

    // Check if the source territory and target territory belong to the same player
    if (sourceTerritory->getOwnerPlayer() == targetTerritory->getOwnerPlayer())
    {
        // If so, move ALL the armies from the source territory to the target territory, and set the source territory to zero,
        sourceTerritory->removeArmiesFromTerritory(&currentNumArmiesInSourceTerritory);
        targetTerritory->addArmiesToTerritory(&currentNumArmiesInTargetTerritory);
    }
    else
    {
        // If the target territory is owned by another player, simulate an attack
        AttackResult attackResult = sourceTerritory->simulateAttack(targetTerritory.get(), &numArmies);

        if(targetTerritory->getOwnerPlayer()->getStrategy()->getStrategyName() == "Neutral"){
            PlayerStrategy* aggressive = new AggressivePlayerStrategy();
            targetTerritory->getOwnerPlayer()->setStrategy(aggressive);
        }

        if (attackResult.territoryConquered)
        {
            // If the territory was conquered, attacker may receive a card
            if (attackResult.shouldAwardCard && issuingPlayerPtr)
            {
                issuingPlayerPtr->awardCard();
            }
        }
    }

    this->setEffect("Executed");

    this->notifyObservers();
}

// GETTERS
int Advance::getNumArmies()
{
    return *this->numArmies;
}

Territory Advance::getSourceTerritory()
{
    return *this->sourceTerritory;
}

Territory Advance::getTargetTerritory()
{
    return *this->targetTerritory;
}

// SETTERS
void Advance::setNumArmies(const int &numArmies)
{
    *(this->numArmies) = numArmies;
}

void Advance::setSourceTerritory(const Territory &sourceTerritory)
{
    *(this->sourceTerritory) = sourceTerritory;
}

void Advance::setTargetTerritory(const Territory &targetTerritory)
{
    *(this->targetTerritory) = targetTerritory;
}
// ........... 2.ADVANCE .............

// ........... 3. BOMB ...............
// DEFAULT CONSTRUCTOR to initialize any data members or set up the object
Bomb::Bomb() : Order("Bomb", "No effect"), targetTerritory(make_shared<Territory>()) {}

// PARAMETERIZED CONSTRUCTOR to initialize an object from another object. // THIS LINE IS GIVING ERROR
Bomb::Bomb(Player *issuingPlayer, const Territory &targetTerritory)
    : Order("Bomb", issuingPlayer, "No effect"), targetTerritory(make_shared<Territory>(targetTerritory)) {}

// COPY CONSTRUCTOR to initialize an object from another object.
Bomb::Bomb(const Bomb &bombObject) : Order(bombObject),
                                     targetTerritory(make_shared<Territory>(*bombObject.targetTerritory)) {}

// ASSIGNMENT OPERATOR to assign an object to another object.
Bomb &Bomb::operator=(const Bomb &bombObject)
{
    if (this != &bombObject)
    {
        Order::operator=(bombObject); // base class assignment operator
        targetTerritory = make_shared<Territory>(*bombObject.targetTerritory);
    }
    return *this;
}

// Virtual print function to be overridden by derived classes
void Bomb::print(ostream &output) const
{
    Order::print(output); // Call the base class print method.
    output << "[BOMB ORDER DETAILS]"
           << "\n";
    output << "-----------------------------------------\n";
    output << "Target territory: " << targetTerritory.get() << "\n";
}

// STREAM INSERTION OPERATOR to print the contents of an object.
// ostream &operator<<(ostream &output, const Bomb &bombObject) {
//    bombObject.print(output);
//    return output;
//}

// VALIDATE
bool Bomb::validate()
{
    Player *issuingPlayerPtr = this->getIssuingPlayer();

    bool targetIsOwnedByPlayer = false;
    // Check if the territory belongs to the player issuing the order
    if (targetTerritory->getOwnerPlayer() == issuingPlayerPtr)
    {
        return true;
    }

    // Check if the territory is adjacent to one owned by the issuing player
    bool isAdjacent = false;
    for (auto &territory : issuingPlayerPtr->getPlayerTerritories())
    {
        if (territory->territoriesAreAdjacent(targetTerritory.get()))
        {
            isAdjacent = true;
            break;
        }
    }
    return isAdjacent;

    // If the target territory is owned by the issuing player, the order is invalid
    // If the target territory is not adjacent to any of the issuing player's territories, the order is invalid
    if (targetIsOwnedByPlayer || !isAdjacent)
    {
        std::cout << "Bomb order invalid: Target territory is not adjacent to any of the issuing player's territories." << std::endl;
    }
}

// EXECUTE
void Bomb::execute()
{
    if (this->validate())
    {
        targetTerritory->bombTerritory();
        std::cout << "Bomb order executed: Half of the armies in "
                  << targetTerritory->getTerritoryName() << " have been removed." << std::endl;

        //Create local player object of target player
        Player* targetPlayer = targetTerritory->getOwnerPlayer();
        PlayerStrategy* targetPlayerStrategy = targetPlayer->getStrategy();

        //Determine if strategy is neutral
        if(targetTerritory->getOwnerPlayer()->getStrategy()->getStrategyName() == "Neutral"){
            PlayerStrategy* aggressive = new AggressivePlayerStrategy();
            targetTerritory->getOwnerPlayer()->setStrategy(aggressive);
        }
    }
    else
    {
        std::cout << "Bomb order is invalid and cannot be executed." << std::endl;
    }

    this->setEffect("Executed");

    this->notifyObservers();
}

// GETTERS
Territory Bomb::getTargetTerritory()
{
    return *this->targetTerritory;
}

// SETTERS
void Bomb::setTargetTerritory(const Territory &targetTerritory)
{
    *(this->targetTerritory) = targetTerritory;
}
// ........... 3. BOMB ...............

// ........... 4. BLOCKADE ...........
// DEFAULT CONSTRUCTOR to initialize any data members or set up the object
Blockade::Blockade() : Order("Blockade", "No effect"), targetTerritory(make_shared<Territory>()) {}

// PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object
Blockade::Blockade(Player *issuingPlayer, const Territory &targetTerritory) : Order("Blockade", issuingPlayer, "No effect"), targetTerritory(make_shared<Territory>(targetTerritory)) {}

// COPY CONSTRUCTOR to initialize an object from another object.
Blockade::Blockade(const Blockade &blockadeObject) : Order(blockadeObject), targetTerritory(make_shared<Territory>(*blockadeObject.targetTerritory)) {}

// ASSIGNMENT OPERATOR to assign an object to another object.
Blockade &Blockade::operator=(const Blockade &blockadeObject)
{
    if (this != &blockadeObject) // protect against invalid self-assignment
    {
        Order::operator=(blockadeObject); // base class assignment operator
        targetTerritory = make_shared<Territory>(*blockadeObject.targetTerritory);
    }
    return *this;
}

// Virtual print function to be overridden by derived classes
void Blockade::print(ostream &output) const
{
    Order::print(output); // Call the base class print method.
    output << "[BLOCKADE ORDER DETAILS]"
           << "\n";
    output << "-----------------------------------------\n";
    output << "Target territory: " << targetTerritory.get() << "\n";
}

// STREAM INSERTION OPERATOR to print the contents of an object.
// ostream &operator<<(ostream &output, const Blockade &blockadeObject) {
//    blockadeObject.print(output);
//    return output;
//}

// VALIDATE
bool Blockade::validate()
{
    // Get issuing player
    Player *issuingPlayerPtr = this->getIssuingPlayer();

    // Get target territory
    Territory *targetTerritoryPtr = targetTerritory.get();

    // Check if the territory belongs to the player issuing the order
    if (targetTerritoryPtr->getOwnerPlayer() != issuingPlayerPtr)
    {
        std::cout << "Blockade order invalid: Target territory does not belong to the issuing player." << std::endl;
        return false;
    }

    return true;
}

// EXECUTE
void Blockade::execute()
{
    if (!this->validate())
    {
        std::cout << "Blockade order cannot be executed because it is not valid." << std::endl;
        return;
    }

    // Get target territory
    Territory *targetTerritoryPtr = targetTerritory.get();

    // Double the number of armies in the target territory
    int currentArmies = targetTerritoryPtr->getNumberOfArmies();
    int doubledArmies = currentArmies * 2;
    targetTerritoryPtr->setNumberOfArmies(doubledArmies);

    // Assign the territory to a neutral player.
    // We assume that the game has a mechanism to reference a neutral player object (A3)
    // For now, we'll just pass a nullptr to represent the neutral player.
    targetTerritoryPtr->setOwnerPlayer(nullptr);

    std::cout << "Blockade order executed: The number of armies in "
              << targetTerritoryPtr->getTerritoryName()
              << " has been doubled and the territory ownership has been transferred to the Neutral player." << std::endl;

    this->setEffect("Executed");

    this->notifyObservers();
}

// GETTERS
Territory Blockade::getTargetTerritory()
{
    return *this->targetTerritory;
}

// SETTERS
void Blockade::setTargetTerritory(const Territory &targetTerritory)
{
    *(this->targetTerritory) = targetTerritory;
}
// ........... 4. BLOCKADE ...........

// ........... 5. AIRLIFT ............
// DEFAULT CONSTRUCTOR to initialize any data members or set up the object
Airlift::Airlift() : Order("Airlift", "No effect"), numArmies(make_shared<int>(0)), sourceTerritory(make_shared<Territory>()), targetTerritory(make_shared<Territory>()) {}

// PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object
Airlift::Airlift(Player *issuingPlayer, const int numArmies, const Territory &sourceTerritory, const Territory &targetTerritory) : Order("Airlift", issuingPlayer, "No effect"), numArmies(make_shared<int>(numArmies)), sourceTerritory(make_shared<Territory>(sourceTerritory)), targetTerritory(make_shared<Territory>(targetTerritory)) {}

// COPY CONSTRUCTOR to initialize an object from another object.
Airlift::Airlift(const Airlift &airliftObject) : Order(airliftObject), numArmies(make_shared<int>(*airliftObject.numArmies)), sourceTerritory(make_shared<Territory>(*airliftObject.sourceTerritory)), targetTerritory(make_shared<Territory>(*airliftObject.targetTerritory)) {}

// ASSIGNMENT OPERATOR to assign an object to another object.
Airlift &Airlift::operator=(const Airlift &airliftObject)
{
    if (this != &airliftObject) // protect against invalid self-assignment
    {
        Order::operator=(airliftObject); // base class assignment operator
        numArmies = make_shared<int>(*airliftObject.numArmies);
        sourceTerritory = make_shared<Territory>(*airliftObject.sourceTerritory);
        targetTerritory = make_shared<Territory>(*airliftObject.targetTerritory);
    }
    return *this;
}

// Virtual print function to be overridden by derived classes
void Airlift::print(ostream &output) const
{
    Order::print(output); // Call the base class print method.
    output << "[AIRLIFT ORDER DETAILS]"
           << "\n";
    output << "Number of armies: " << *numArmies << "\n";
    output << "-----------------------------------------\n";
    output << "Source territory: " << sourceTerritory.get();
    output << "-----------------------------------------\n";
    output << "Target territory: " << targetTerritory.get();
}

// STREAM INSERTION OPERATOR to print the contents of an object.
// ostream &operator<<(ostream &output, const Airlift &airliftObject) {
//    airliftObject.print(output);
//    return output;
//}

// VALIDATE
bool Airlift::validate()
{
    // Get issuing player
    Player *issuingPlayerPtr = this->getIssuingPlayer();

    // Get source and target territories
    Territory *sourceTerritoryPtr = sourceTerritory.get();
    Territory *targetTerritoryPtr = targetTerritory.get();

    // Check if both source and target territories belong to the issuing player
    if (sourceTerritoryPtr->getOwnerPlayer() == issuingPlayerPtr && targetTerritoryPtr->getOwnerPlayer() == issuingPlayerPtr)
    {
        return true;
    }

    std::cout << "Airlift order invalid: Either the source or the target territory does not belong to the issuing player." << std::endl;
    return false;
}

// EXECUTE
void Airlift::execute()
{
    if (!this->validate())
    {
        std::cout << "Airlift order cannot be executed because it is not valid." << std::endl;
        return;
    }

    // Get issuing player
    Player *issuingPlayerPtr = this->getIssuingPlayer();

    // Get source and target territories
    Territory *sourceTerritoryPtr = sourceTerritory.get();
    Territory *targetTerritoryPtr = targetTerritory.get();

    // Get the number of armies to airlift
    int numArmies = this->getNumArmies();

    // Get the number of armies in the source territory
    int numArmiesInSourceTerritory = sourceTerritory->getNumberOfArmies();
    // Get the number of armies in the target territory
    int numArmiesInTargetTerritory = targetTerritory->getNumberOfArmies();

    // Update number of armies in the source territory
    int currentNumArmiesInSourceTerritory = numArmiesInSourceTerritory - numArmies;
    // Update number of armies in the target territory
    int currentNumArmiesInTargetTerritory = numArmiesInTargetTerritory + numArmies;

    // Check if the source territory and target territory belong to the same player
    if (sourceTerritory->getOwnerPlayer() == targetTerritory->getOwnerPlayer())
    {
        // If so, move ALL the armies from the source territory to the target territory, and set the source territory to zero,
        sourceTerritory->removeArmiesFromTerritory(&currentNumArmiesInSourceTerritory);
        targetTerritory->addArmiesToTerritory(&currentNumArmiesInTargetTerritory);
    }

    std::cout << "Airlift order executed: " << currentNumArmiesInSourceTerritory << " armies have been airlifted from "
              << sourceTerritoryPtr->getTerritoryName() << " to " << targetTerritoryPtr->getTerritoryName() << "." << std::endl;

    this->setEffect("Executed");

    this->notifyObservers();
}

// GETTERS
int Airlift::getNumArmies()
{
    return *this->numArmies;
}

Territory Airlift::getSourceTerritory()
{
    return *this->sourceTerritory;
}

Territory Airlift::getTargetTerritory()
{
    return *this->targetTerritory;
}

// SETTERS
void Airlift::setNumArmies(const int numArmies)
{

    *(this->numArmies) = numArmies;
}

void Airlift::setSourceTerritory(const Territory &sourceTerritory)
{

    *(this->sourceTerritory) = sourceTerritory;
}

void Airlift::setTargetTerritory(const Territory &targetTerritory)
{
    *(this->targetTerritory) = targetTerritory;
}
// ........... 5. AIRLIFT ............

// ........... 6. NEGOTIATE ..........
// DEFAULT CONSTRUCTOR to initialize any data members or set up the object
Negotiate::Negotiate() : Order("Negotiate", "No effect"), targetPlayer(nullptr) {}

// PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object
Negotiate::Negotiate(Player *issuingPlayer, Player *targetPlayer) : Order("Negotiate", issuingPlayer, "No effect"), targetPlayer(targetPlayer) {}

// COPY CONSTRUCTOR to initialize an object from another object.
Negotiate::Negotiate(const Negotiate &negotiateObject) : Order(negotiateObject), targetPlayer(negotiateObject.targetPlayer) {}

// ASSIGNMENT OPERATOR to assign an object to another object.
Negotiate &Negotiate::operator=(const Negotiate &negotiateObject)
{
    if (this != &negotiateObject) // protect against invalid self-assignment
    {
        Order::operator=(negotiateObject); // base class assignment operator
        targetPlayer = negotiateObject.targetPlayer;
    }
    return *this;
}

// Virtual print function to be overridden by derived classes
void Negotiate::print(ostream &output) const
{
    Order::print(output); // Call the base class print method.
    output << "[NEGOTIATE ORDER DETAILS]"
           << "\n";
    output << "--------------------------------------\n";
    output << "Target player: " << *targetPlayer << "\n";
}

// STREAM INSERTION OPERATOR to print the contents of an object.
// ostream &operator<<(ostream &output, const Negotiate &negotiateObject) {
//    negotiateObject.print(output);
//    return output;
//}

// VALIDATE
bool Negotiate::validate()
{
    // Retrieve the issuing player and target player for validation
    Player *issuingPlayerPtr = this->getIssuingPlayer();
    Player *targetPlayerPtr = this->getTargetPlayer();

    // Check if the target player is the same as the issuing player - negotiation must be with another player
    if (issuingPlayerPtr == targetPlayerPtr)
    {
        std::cout << "Negotiate order is invalid: cannot negotiate with oneself." << std::endl;
        return false;
    }

    // Check if the target player is a null pointer - must have a valid target player to negotiate with
    if (targetPlayerPtr == nullptr)
    {
        std::cout << "Negotiate order is invalid: target player is null." << std::endl;
        return false;
    }

    // If the players are already in a negotiation, the order is redundant/invalid
    if (issuingPlayerPtr->isNegotiationActive(targetPlayerPtr->getPlayerID()))
    {
        std::cout << "Negotiate order is invalid: already in negotiation with target player." << std::endl;
        return false;
    }

    // Additional checks can be added here if needed, such as checking if the player is in a state that allows negotiation

    return true; // If all checks pass, the negotiation is valid
}

// EXECUTE
void Negotiate::execute()
{
    if (this->validate())
    {
        // Start a negotiation between the issuing player and the target player
        this->getIssuingPlayer()->startNegotiation(this->getTargetPlayer()->getPlayerID());
        this->getTargetPlayer()->startNegotiation(this->getIssuingPlayer()->getPlayerID());

        std::cout << "Negotiate order executed: Player " << this->getIssuingPlayer()->getPlayerID()
                  << " and Player " << this->getTargetPlayer()->getPlayerID()
                  << " cannot attack each other for the remainder of the turn." << std::endl;
    }
    else
    {
        std::cout << "Negotiate order is not valid and cannot be executed." << std::endl;
    }

    this->setEffect("Executed");

    this->notifyObservers();
}

// GETTERS
Player *Negotiate::getTargetPlayer() const
{
    return targetPlayer;
}

// SETTERS
void Negotiate::setTargetPlayer(Player *targetPlayer)
{
    // No need to delete the old Player* since we don't own it
    this->targetPlayer = targetPlayer;
}

// ........... 6. NEGOTIATE ...........

// ........... 7. ORDERS LIST ...........

// DEFAULT CONSTRUCTOR
OrdersList::OrdersList() : orders(make_shared<vector<shared_ptr<Order>>>())
{
    this->addObserver(&logger);
    logger.setObservedObject(this);
}

// PARAMETERIZED CONSTRUCTOR
OrdersList::OrdersList(const vector<shared_ptr<Order>> &initOrders)
    : orders(make_shared<vector<shared_ptr<Order>>>(initOrders))
{
    this->addObserver(&logger);
    logger.setObservedObject(this);
}

// COPY CONSTRUCTOR
OrdersList::OrdersList(const OrdersList &otherList)
    : orders(make_shared<vector<shared_ptr<Order>>>(*otherList.orders)) {}

// ASSIGNMENT OPERATOR
OrdersList &OrdersList::operator=(const OrdersList &otherList)
{
    if (this != &otherList)
    {
        orders->clear();
        *orders = *otherList.orders; // Deep copy due to shared_ptr nature
    }
    return *this;
}

// STREAM INSERTION OPERATOR
ostream &operator<<(ostream &output, const OrdersList &ordersListObject)
{
    output << "\nORDERS LIST: [" << ordersListObject.getOrders().size() << "]" << endl;

    output << "ordersListObject" << ordersListObject;
    output << "*ordersListObject.orders" << *ordersListObject.orders;
    output << "ordersListObject.getOrders()" << ordersListObject.getOrders();

    // Check if the orders list is null
    if (!ordersListObject.orders)
    {
        output << "No orders available." << endl;
        return output;
    }

    if (ordersListObject.getOrders().empty())
    {
        output << "No orders in the list." << endl;
    }
    else
    {
        int i = 1;
        for (const auto &order : *ordersListObject.orders)
        {
            output << "[ORDER " << i++ << "]: \n";
            output << *order << endl; // Assuming Order has overloaded << operator
        }
    }

    return output;
}

// GETTERS
const vector<shared_ptr<Order>> &OrdersList::getOrders() const
{
    return *orders;
}

// SETTERS
void OrdersList::setOrders(const vector<shared_ptr<Order>> &newOrders)
{
    *orders = newOrders;
}

// STRING-TO-LOG
string OrdersList::stringToLog() const
{
    auto lastOrder = orders->back(); // Get the last order
    return "Order Issued: " + lastOrder->getOrderType();
}

// ADD ORDER
void OrdersList::addOrder(const shared_ptr<Order> &order)
{
    orders->push_back(order);
    this->notifyObservers();
}



// REMOVE ORDER
void OrdersList::removeOrder(size_t index)
{
    if (index < orders->size())
    {
        orders->erase(orders->begin() + index);
    }
}

// MOVE ORDER
void OrdersList::moveOrder(size_t index, OrdersList &otherOrdersList)
{
    if (index < orders->size())
    {
        otherOrdersList.addOrder((*orders)[index]);
        orders->erase(orders->begin() + index);
    }
}

// EXECUTE ALL ORDERS
void OrdersList::executeAll()
{
    for (const auto &order : *orders)
    {
        order->execute();
    }
}
// ........... 7. ORDERS LIST ...........
