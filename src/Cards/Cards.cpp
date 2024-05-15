#include "Cards.h"
#include "../OrdersList/Orders.h"
#include "../Player/Player.h"
#include <iostream>
#include <algorithm>

// Constructor for Card class
Card::Card(Type type) : type(type) {}

// Copy constructor for Card class
Card::Card(const Card &other) : type(other.type) {}

// Assignment operator for Card class
Card &Card::operator=(const Card &other)
{
    if (this == &other)
    {
        return *this;
    }
    type = other.type;
    return *this;
}

// Destructor for Card class
Card::~Card() {}

// Stream insertion operator for Card class
std::ostream &operator<<(std::ostream &os, const Card &card)
{
    std::string typeString;
    switch (card.getType())
    {
    case Card::Type::Bomb:
        typeString = "Bomb";
        break;
    case Card::Type::Reinforcement:
        typeString = "Reinforcement";
        break;
    case Card::Type::Blockade:
        typeString = "Blockade";
        break;
    case Card::Type::Airlift:
        typeString = "Airlift";
        break;
    case Card::Type::Diplomacy:
        typeString = "Diplomacy";
        break;
    }
    os << "Type: " << typeString;
    return os;
}

// Getter function to retrieve the type of the card
Card::Type Card::getType() const
{
    return type;
}

bool Card::cardEqualsString(std::string input){

    switch (this->getType())
    {
        case Card::Type::Bomb:
            return "bomb" == input;
        case Card::Type::Reinforcement:
            return "reinforcement" == input;
        case Card::Type::Blockade:
            return "blockade" == input;
        case Card::Type::Airlift:
            return "airlift" == input;
        case Card::Type::Diplomacy:
            return "diplomacy" == input;
    }

    return false;
}

// Function to play the card from HAND
void Card::play(std::vector<Card *> &hand, std::vector<Card *> &deck)
{

//    // Create an order object pointer to pass to the player constructor
//    Order *order = new Order();
//
//    // Initialize a vector of order pointers
//    std::vector<Order *> orders = {order};
//
//    // Assuming you have a vector<Order*> called orders
//    std::vector<std::shared_ptr<Order>> sharedOrders;
//    for (Order *orderPtr : orders)
//    {
//        sharedOrders.push_back(std::make_shared<Order>(*orderPtr));
//    }
//
//    // Now create an OrdersList object with the sharedOrders vector
//    std::shared_ptr<OrdersList> ordersList = std::make_shared<OrdersList>(sharedOrders);
//
//    // Create a hand object pointer to pass to the player constructor
//    Hand *handObj = new Hand(hand);
//
//    // Add the order to the player's list of orders
//    Player player("Player_1", {}, {}, handObj, ordersList);
//
//    std::cout << player << std::endl;
//
//    std::cout << "Played a card of type: " << type << std::endl;
//    // Remove the card from the hand vector
//    std::vector<Card *>::iterator it = std::find(hand.begin(), hand.end(), this);
//    if (it != hand.end())
//    {
//        hand.erase(it);
//    }
//    // Put the card back in the deck vector
//    deck.push_back(this);
}

// Constructor for Deck class
Deck::Deck()
{
    // Initialize the deck with cards
    for (int i = 0; i < 5; ++i)
    {
        cards.push_back(new Card(static_cast<Card::Type>(i)));
    }
}

Deck::Deck(int size){
    srand(time(0));

    for (int i = 0; i < size; ++i)
    {
        int randomNumber = rand() % 5;
        cards.push_back(new Card(static_cast<Card::Type>(randomNumber)));
    }
}

// Copy constructor for Deck class
Deck::Deck(const Deck &other)
{
    for (Card *card : other.cards)
    {
        cards.push_back(new Card(*(card)));
    }
}

// Assignment operator for Deck class
Deck &Deck::operator=(const Deck &other)
{
    if (this == &other)
    {
        return *this;
    }
    for (Card *card : cards)
    {
        delete card;
    }
    cards.clear();
    for (Card *card : other.cards)
    {
        cards.push_back(new Card(*(card)));
    }
    return *this;
}

// Destructor for Deck class
Deck::~Deck()
{
    for (Card *card : cards)
    {
        delete card;
    }
    cards.clear();
}

// Stream insertion operator for Deck class
std::ostream &operator<<(std::ostream &os, const Deck &deck)
{
    if (&deck == nullptr) {
        os << "Deck is empty." << std::endl;
    } else {
        os << "Deck contains " << deck.cards.size() << " cards:" << std::endl;
        for (const Card *card : deck.cards)
        {
            os << *card << std::endl;
        }
    }
    return os;
}

// Function to add card to HAND
Card *Deck::draw()
{
    if (cards.empty())
    {
        std::cerr << "Cannot draw from an empty deck." << std::endl;
        return nullptr;
    }
    // Optionally shuffle cards or generate a random index to draw a card
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    Card *drawnCard = cards.back();
    cards.pop_back(); // Remove the card from the deck
    return drawnCard; // Return the drawn card
}

// Constructor for Hand class
Hand::Hand() {}

// Overloaded constructor for Hand class
Hand::Hand(const std::vector<Card *> &handCards)
{
    for (Card *card : handCards)
    {
        cards.push_back(new Card(*card));
    }
}

// Copy constructor for Hand class
Hand::Hand(const Hand &other)
{
    for (Card *card : other.cards)
    {
        cards.push_back(new Card(*(card)));
    }
}

// Assignment operator for Hand class
Hand &Hand::operator=(const Hand &other)
{
    if (this == &other)
    {
        return *this;
    }
    for (Card *card : cards)
    {
        delete card;
    }
    cards.clear();
    for (Card *card : other.cards)
    {
        cards.push_back(new Card(*(card)));
    }
    return *this;
}

void Hand::addCard(Card *card)
{
    if (card)
    {
        cards.push_back(card); // Add the card to the hand
    }
}

std::vector<Card*> Hand::getHand()
{
   return cards;
}

// Destructor for Hand class
Hand::~Hand()
{
    for (Card *card : cards)
    {
        delete card;
    }
    cards.clear();
}

// Stream insertion operator for Hand class
std::ostream &operator<<(std::ostream &os, const Hand &hand)
{
    if (&hand == nullptr) {
        os << "Hand is empty." << std::endl;
    } else {
        os << "Hand contains " << hand.cards.size() << " cards:" << std::endl;
        for (const Card *card : hand.cards)
        {
            os << *card << std::endl;
        }
    }
    return os;
}
