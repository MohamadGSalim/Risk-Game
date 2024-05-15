#ifndef CARDS_H
#define CARDS_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>

#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../OrdersList/Orders.h"

class Card
{
public:
    // Enumerated type to represent different types of cards
    enum class Type
    {
        Bomb,
        Reinforcement,
        Blockade,
        Airlift,
        Diplomacy
    };

    // Constructor: Creates a card of the specified type
    Card(Type type);

    // Copy Constructor: Creates a copy of an existing card
    Card(const Card &other);

    // Assignment Operator: Assigns the values of another card to this card
    Card &operator=(const Card &other);

    // Destructor: Cleans up resources used by the card
    ~Card();

    // Stream insertion operator for Card class
    friend std::ostream &operator<<(std::ostream &os, const Card &card);

    // Getter method to retrieve the type of the card
    Type getType() const;

    bool cardEqualsString(std::string);

    // Method to "play" the card from the hand
    void play(std::vector<Card *> &hand, std::vector<Card *> &deck);

private:
    Type type; // Type of the card (e.g., Bomb, Reinforcement, etc.)
};

class Deck
{
public:
    // Friend function to give access to private members for testing purposes
    friend void testCards();

    // Constructor default: Initializes a deck of cards
    Deck();

    //Constructor: Initialize deck of cards to certain size
    Deck(int size);

    // Copy Constructor: Creates a copy of an existing deck
    Deck(const Deck &other);

    // Assignment Operator: Assigns the values of another deck to this deck
    Deck &operator=(const Deck &other);

    // Destructor: Cleans up resources used by the deck
    ~Deck();

    // Stream insertion operator for Deck class
    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);

    // Method to simulate drawing a card from the deck
    Card *draw();

private:
    std::vector<Card *> cards; // Collection of cards in the deck
};

class Hand
{
public:
    // Friend function to give access to private members for testing purposes
    friend void testCards();

    // Constructor: Initializes an empty hand of cards
    Hand();

    // Overloaded Constructor: Initializes a hand of cards with the specified cards
    Hand(const std::vector<Card *> &handCards);

    // Copy Constructor: Creates a copy of an existing hand
    Hand(const Hand &other);

    // Assignment Operator: Assigns the values of another hand to this hand
    Hand &operator=(const Hand &other);

    // Method to add a card to the hand
    void addCard(Card *card);

    std::vector<Card*> getHand();

    // Destructor: Cleans up resources used by the hand
    ~Hand();

    // Stream insertion operator for Hand class
    friend std::ostream &operator<<(std::ostream &os, const Hand &hand);

private:
    std::vector<Card *> cards; // Collection of cards in the hand
};

#endif // CARDS_H
