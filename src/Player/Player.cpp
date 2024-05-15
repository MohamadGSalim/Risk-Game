#include "Player.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../OrdersList/Orders.h"
#include <iostream>
#include <algorithm>

// Default constructor
Player::Player() : player_id(new std::string("")), player_hand(nullptr), player_deck(nullptr),player_orders(nullptr), reinforcementPool(0), activeNegotiations(), strategy(nullptr)
{
    // other initializations
}

Player::Player(std::string id) : player_id(new std::string(id)), player_hand(nullptr), player_deck(nullptr), reinforcementPool(0), activeNegotiations(), player_orders(), strategy(nullptr)
{

}

// Parameterized constructor
Player::Player(std::string id,
               std::vector<Territory *> territories,
               std::vector<Territory *> enemy,
               Hand *hand,
               Deck *deck,
               std::shared_ptr<OrdersList> orders,
               std::initializer_list<std::string> activeNegotiationsInit,
               PlayerStrategy* strategy)
    : player_id(new std::string(id)),
      player_territories(territories),
      enemy_territories(enemy),
      player_hand(hand),
      player_deck(deck),
      player_orders(orders),
      reinforcementPool(50),
      activeNegotiations(activeNegotiationsInit),
      strategy(strategy)// Using initializer list
{
    // other initializations
}

// Copy constructor
Player::Player(const Player &player_copy)
{
    // copy all members
}

// Destructor
Player::~Player()
{
    delete player_id;
    delete strategy;
}

// Assignment operator
Player &Player::operator=(const Player &player_copy)
{
    if (this != &player_copy)
    {
        // Assign all members
    }
    return *this;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &out, const Player &p)
{
    out << "----------------player----------------" << std::endl;
    out << "Player ID: " << *p.player_id;

    out << "\nPlayer Territories: ";
    for (auto i : p.player_territories)
    {
        out << i->getTerritoryName() << std::endl;
    }

    out << "\nEnemy Territories: ";
    for (auto i : p.enemy_territories)
    {
        out << i->getTerritoryName() << std::endl;
    }

    out << "\nPlayer Hand: ";
    out << *p.player_hand;

    out << "Player Deck: ";
    out << *p.player_deck;

    out << "Player Orders: ";
    out << *p.player_orders;

    out << "Active Negotiations: " << std::endl;
    for (const auto &negotiation : p.activeNegotiations)
    {
        out << negotiation;
    }
    out << "----------------player----------------";
    return out;
}

// ############################## Getters / Setters ##############################

string Player::getPlayerID()
{
    return *player_id;
}

const std::vector<Territory *> &Player::getPlayerTerritories() const
{
    return player_territories;
}

const std::vector<Territory *> &Player::getEnemyTerritories() const
{
    return enemy_territories;
}

void Player::setPlayerTerritories(const std::vector<Territory *> &territories)
{
    player_territories = territories;
}

void Player::setEnemyTerritories(const std::vector<Territory *> &territories)
{
    enemy_territories = territories;
}

Hand *Player::getHand() const
{
    return player_hand;
}

void Player::setHand(Hand *hand)
{
    player_hand = hand;
}

Deck *Player::getDeck() const
{
    return player_deck;
}

void Player::setDeck(Deck *deck)
{
    player_deck = deck;
}

shared_ptr<OrdersList> Player::getOrderList()
{
    return this->player_orders;
}

void Player::setOrderList(std::shared_ptr<OrdersList> orders)
{
    player_orders = orders;
}

int Player::getReinforcementPool()
{
    return this->reinforcementPool;
}

void Player::setReinforcementPool(int i)
{
    this->reinforcementPool = i;
}

GameContext* Player::getContext(){
    return this->gameContext;
};

void Player::setContext(GameContext* newContext){
    this->gameContext = newContext;
};

// ############################## Territory Methods ##############################

// Returns a list of territories that are to be defended
std::vector<Territory *> Player::toDefend()
{

//    std::vector<Territory *> defendingPriority = getPlayerTerritories();
//
//    sort(defendingPriority.begin(), defendingPriority.end(), [](const auto &a, const auto &b)
//         { return a->getPriority() < b->getPriority(); });
//
//    return defendingPriority;

    return strategy->toDefend(this);
}

// Returns a list of territories that are to be attacked
std::vector<Territory *> Player::toAttack()
{

//    std::vector<Territory *> attackingPriority = getEnemyTerritories();
//    sort(attackingPriority.begin(), attackingPriority.end(), [](const auto &a, const auto &b)
//         { return a->getPriority() < b->getPriority(); });
//
//    return attackingPriority;

    return strategy->toAttack(this);

}

int Player::numOfTerritories()
{
    return getPlayerTerritories().size();
}

bool Player::isValidTerritory(string orderInput, std::vector<Territory *> territories)
{

    for (int i = 0; i < territories.size(); i++)
    {
        if (orderInput == territories[i]->getTerritoryName())
        {
            return true;
        }
    }
    return false;
}

Territory Player::getTerritoryByName(string orderInput, std::vector<Territory *> territories)
{

    for (int i = 0; i < territories.size(); i++)
    {
        if (orderInput == territories[i]->getTerritoryName())
        {
            return territories[i];
        }
    }
    return nullptr;
}

/**
 * A method to check if a player owns a specific territory
 * @param player The player in question
 * @param territory The territory in question
 * @return True if the player owns the territory, false otherwise
 */

bool playerOwnsTerritory(Player *player, Territory *territory)
{
    // Get the owner player of the territory
    Player *owner = territory->getOwnerPlayer();

    // Compare the player ID of the territory's owner with the player in question
    return owner != nullptr && owner->getPlayerID() == player->getPlayerID();
}

// ############################## Reinforcement Methods ##############################

void Player::addToReinforcementPool(int i)
{
    setReinforcementPool(getReinforcementPool() + i);
}

bool Player::noReinforcementsLeft()
{
    return (this->getReinforcementPool() == 0);
}

void Player::printDeployAllReinforcements()
{
    std::cout << "You must deploy all reinforcements before issuing other orders." << std::endl;
}

void Player::printDontHaveCard()
{
    std::cout << "You must own that card to issue that order" << std::endl;
}

void Player::printNoMoreReinforcements()
{
    std::cout << "You have no more reinforcements to deploy." << std::endl;
}

// ############################## Order Methods ##############################
// Creates an Order object and puts it in the player’s list of orders
bool Player::issueOrder()
{
    bool validOrder = true;
    string orderInput = "";
    int orderNum;

    std::cout << "Player: " << getPlayerID() << std::endl;
    std::cout << "The size of your reinforcement pool is: " << getReinforcementPool() << std::endl;
    std::cout << "The territories you own are: " << std::endl;
    for (int i = 0; i < getPlayerTerritories().size(); i++)
    {
        std::cout << getPlayerTerritories()[i]->getTerritoryName() << std::endl;
    }

    while (validOrder)
    {
        std::cout << "Enter the order you would like to issue. (If you have no orders to issue, enter: noorder): " << std::endl;
        std::cin >> orderInput;

        // Ensuring the order entered is all lower case
        std::transform(orderInput.begin(), orderInput.end(), orderInput.begin(), [](unsigned char c)
                       { return std::tolower(c); });

        if (orderInput == "noorder")
        {
            return false;
        }

        if (orderInput == "deploy")
        {
            if (noReinforcementsLeft())
            {
                printNoMoreReinforcements();
            }
            else
            {
                orderNum = 1;
                validOrder = false;
            }
        }
        else if (orderInput == "advance")
        {
            if (noReinforcementsLeft())
            {
                orderNum = 2;
                validOrder = false;
            }
            else
            {
                printDeployAllReinforcements();
            }
        }
        else if (orderInput == "bomb")
        {
            if (noReinforcementsLeft() && hasCard(orderInput))
            {
                orderNum = 3;
                validOrder = false;
            }

            else
            {
                if (!noReinforcementsLeft())
                {
                    printDeployAllReinforcements();
                }
                else
                    printDontHaveCard();
            }
        }
        else if (orderInput == "blockade")
        {
            if (noReinforcementsLeft() && hasCard(orderInput))
            {
                orderNum = 4;
                validOrder = false;
            }
            else
            {
                if (!noReinforcementsLeft())
                {
                    printDeployAllReinforcements();
                }
                else
                    printDontHaveCard();
            }
        }
        else if (orderInput == "airlift")
        {
            if (noReinforcementsLeft() && hasCard(orderInput))
            {
                orderNum = 5;
                validOrder = false;
            }
            else
            {
                if (!noReinforcementsLeft())
                {
                    printDeployAllReinforcements();
                }
                else
                    printDontHaveCard();
            }
        }
        else if (orderInput == "negotiate")
        {
            if (noReinforcementsLeft() && hasCard(orderInput))
            {
                orderNum = 6;
                validOrder = false;
            }
            else
            {
                if (!noReinforcementsLeft())
                {
                    printDeployAllReinforcements();
                }
                else
                    printDontHaveCard();
            }
        }
        else
        {
            std::cout << "Invalid choice. Renter a valid order." << std::endl;
        }
    }

    createOrder(orderNum);

    return true;

}

void Player::issueOrder(string orderName){

    strategy->issueOrder(this, orderName);

}

void Player::createOrder(int orderNumber)
{
    std::shared_ptr<Order> order = nullptr; // Declare 'order' as shared_ptr<Order>

   string territoryNameInput = "";
    string territoryNameInput2 =  "";

    int reinforcements;
    string playerNameInput;

    bool validEntry = true;
    Player *targetPlayer = new Player(playerNameInput);


    std::cout << "Printing Current orders" << endl;

    for (int i = 0; i < player_orders->getOrders().size(); i++){
        std::cout << player_orders->getOrders()[i]->getOrderType() << endl;
    }

    if (orderNumber == 1)
    {

        while (validEntry)
        {
            std::cout << "Enter the number of units you want to deploy: " << std::endl;

            std::cin >> reinforcements;

            if (reinforcements > getReinforcementPool())
            {

                std::cout << "You are trying to deploy too many reinforcements. Please retry." << std::endl;
            }
            else
            {
                validEntry = false;
            }
        }

        // Getting territory to deploy to
        validEntry = true;

        while (validEntry)
        {
            std::cout << "Enter the territory you want to deploy to: " << std::endl;

            std::cin >> territoryNameInput;

            std::cout << "Territory Input Name :" << territoryNameInput << endl;

            for(int i =0; i< this->getPlayerTerritories().size(); i++){
                std::cout << "Territory Names :" << this->getPlayerTerritories().at(i) << ", ";
            }

            if (isValidTerritory(territoryNameInput, getPlayerTerritories()))
            {
                std::cin.clear();
                validEntry = false;
            }
            else
            {
                std::cout << "You are trying to deploy to a territory that is not yours. Please retry." << std::endl;
            }
        }


        auto deployOrder = std::make_shared<Deploy>(this, reinforcements, getTerritoryByName(territoryNameInput, getPlayerTerritories()));


        this->player_orders->addOrder(deployOrder);

        setReinforcementPool(getReinforcementPool() - reinforcements);
        return;
    }

    if (orderNumber == 2)
    {

        std::cout << "Enter the territory you want to advance from: " << std::endl;
        std::cin >> territoryNameInput;

        std::cout << "Enter the territory you want to advance to: " << std::endl;
        std::cin >> territoryNameInput2;

        std::cout << "Enter how many units you want to advance: " << std::endl;
        std::cin >> reinforcements;

        auto advanceOrder = std::make_shared<Advance>(this, reinforcements, getTerritoryByName(territoryNameInput, getPlayerTerritories()), getTerritoryByName(territoryNameInput2, getPlayerTerritories()));
        this->player_orders->addOrder(advanceOrder);

        std::cout << "Printing Current orders" << endl;

        for (int i = 0; i < player_orders->getOrders().size(); i++){
            std::cout << player_orders->getOrders()[i]->getOrderType() << endl;
        }

        return;
    }

    if (orderNumber == 3)
    {
        std::cout << "Enter the territory you want to bomb: " << std::endl;
        std::cin >> territoryNameInput;

        auto bombOrder = std::make_shared<Bomb>(this, getTerritoryByName(territoryNameInput, getEnemyTerritories()));

        if(bombOrder == nullptr){
            std::cout <<"THIS ORDER IS NULL" << endl;
        }

        this->player_orders->addOrder(bombOrder);

        std::cout << "Printing Current orders" << endl;

        for (int i = 0; i < player_orders->getOrders().size(); i++){
            std::cout << player_orders->getOrders()[i]->getOrderType() << endl;
        }

        return;
    }

    if (orderNumber == 4)
    {
        std::cout << "Enter the territory you want to blockade: " << std::endl;
        cin >> territoryNameInput;

        auto blockadeOrder = std::make_shared<Blockade>(this, getTerritoryByName(territoryNameInput, getPlayerTerritories()));
        this->player_orders->addOrder(blockadeOrder);

        std::cout << "Printing Current orders" << endl;

        for (int i = 0; i < player_orders->getOrders().size(); i++){
            std::cout << player_orders->getOrders()[i]->getOrderType() << endl;
        }

        return;
    }

    if (orderNumber = 5) {
        std::cout << "Enter the territory you want to airlift from: " << std::endl;
        std::cin >> territoryNameInput;

        std::cout << "Enter the territory you want to airlift to: " << std::endl;
        std::cin >> territoryNameInput2;

        std::cout << "Enter how many units you want to airlift: " << std::endl;
        std::cin >> reinforcements;

        auto airliftOrder = std::make_shared<Airlift>(this, reinforcements, getTerritoryByName(territoryNameInput, getPlayerTerritories()), getTerritoryByName(territoryNameInput2, getPlayerTerritories()));
        this->player_orders->addOrder((airliftOrder));

        std::cout << "Printing Current orders" << endl;

        for (int i = 0; i < player_orders->getOrders().size(); i++){
            std::cout << player_orders->getOrders()[i]->getOrderType() << endl;
        }

        return;
    }


    if (orderNumber == 6)
    {
        // Create a target player for negotiate upon creation
        std::cout << "Enter the player you want to give the target player: " << std::endl;
        cin >> playerNameInput;

        auto negotiateOrder = std::make_shared<Negotiate>(this, targetPlayer);
        this->player_orders->addOrder((negotiateOrder));
        return;
    }


}

bool Player::hasCard(string order)
{

    std::vector<Card *> hand = this->getHand()->getHand();

    for (int i = 0; i < hand.size(); i++)
    {
        if (hand[i]->cardEqualsString(order))
        {
            return true;
        }
    }
    return false;
}

/**
 * A Method to award a card to a player from the deck if the player has a hand and there is a deck to draw from
 * @param player The player to award the card to
 * @param deck The deck to draw the card from
 * @return True if the card was awarded, false otherwise
 */

// awardCard method
void Player::awardCard()
{
    Card *card = player_deck->draw(); // Draw a card from the deck
    if (card)
    {
        player_hand->addCard(card); // Add the card to the player's hand
    }
}

void Player::removeCard(string cardName){

    vector<Card* > playerCards = this->getHand()->getHand();

    // Find the iterator pointing to the element you want to remove
    auto it = std::find_if(playerCards.begin(), playerCards.end(),
                           [&](Card* card) { return card->cardEqualsString(cardName); });


    // Check if the element was found before erasing
    if (it != playerCards.end()) {
        // Erase the element
        playerCards.erase(it);
        std::cout << "Removed Card:" << cardName << endl;
    } else {
        std::cout << "Card not found." << std::endl;
    }

    std::cout << "Current Cards in hand: " << " " << endl;
    // Print the updated vector
    for (Card* card: playerCards) {
        std::cout << card->getType() << endl;
    }

}

void Player::setPlayerID(string name) {
    this->player_id = new string(name);

}

void Player::addTerritory(Territory *newTerr) {
    this->player_territories.push_back(newTerr);
}

void Player::startNegotiation(const std::string &targetPlayerId)
{
    activeNegotiations.insert(targetPlayerId);
}

bool Player::isNegotiationActive(const std::string &targetPlayerId) const
{
    return activeNegotiations.find(targetPlayerId) != activeNegotiations.end();
}

void Player::endNegotiation(const std::string &targetPlayerId)
{
    activeNegotiations.erase(targetPlayerId);
}

PlayerStrategy *Player::getStrategy() const {
    return strategy;
}

void Player::setStrategy(PlayerStrategy *strategy) {
    Player::strategy = strategy;
}
