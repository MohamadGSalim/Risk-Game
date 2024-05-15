#include "Cards.h"
#include "../OrdersList/Orders.h"
#include "../Player/Player.h"
#include <iostream>

// Function to test all declared classes
void testCards()
{
    std::cout << "-----------------TESTING CARDS-----------------" << std::endl;
    cout << endl;

    // Create a deck of Warzone cards
    Deck deck;

    // Print the initial state of the deck
    std::cout << "Initial Deck: " << deck << std::endl;

    // Create a hand object
    Hand hand;

    // Print the initial state of the hand
    std::cout << "Initial Hand: " << hand << std::endl;

    // Draw cards from the deck and add them to the hand
    for (int i = 0; i < 5; ++i)
    {
        deck.draw();

        // Print the i+1 state of the deck
        std::cout << "Iteration " << i + 1 << ": Deck: " << deck << std::endl;

        // Print the i+1 state of the hand
        std::cout << "Iteration " << i + 1 << ": Hand: " << hand << std::endl;
    }

    // Print the state of the deck just before playing cards
    std::cout << "Deck state before playing cards: " << deck << std::endl;

    // Print the state of the hand just before playing cards
    std::cout << "Hand state before playing cards: " << hand << std::endl;

    // Play all the cards in the hand
    while (!hand.cards.empty())
    {
        Card *card = hand.cards.back(); // Get the last card in the hand
        card->play(hand.cards, deck.cards);

        // Print the current state of the deck
        std::cout << "Current state of Deck "
                  << ": " << deck << std::endl;

        // Print the current state of the hand
        std::cout << "Current state of Hand "
                  << ": " << hand << std::endl;
    }

    // Print the final state of the deck
    std::cout << "Final Deck: " << deck << std::endl;

    // Print the final state of the hand
    std::cout << "Final Hand: " << hand << std::endl;

    std::cout << "---------------end: TESTING CARDS---------------" << std::endl;
    cout << endl;

}

// Main function to run testCards()
// int main()
//{
//    testCards();
//    return 0;
//}
