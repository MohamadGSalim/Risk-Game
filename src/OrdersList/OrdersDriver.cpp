#include "OrdersDriver.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../OrdersList/Orders.h"
#include <iostream>
#include <memory> // Include for std::shared_ptr

using namespace std;

// Create a list of orders using a shared pointer
OrdersList playerOrders;

// testOrdersLists function demonstrates the functionality of the Order and OrdersList classes.
void testOrdersLists()
{
     cout << "----------------- TESTING ORDERS-LISTS -----------------" << endl
          << endl;

     // Create instances of Player and Territory with specific values
     // Instantiate a continent with ID "Continent 1" and number of control points
     Continent continent1("Continent 1", 10);

     Hand *handObj = new Hand();
     Deck *deckObj = new Deck();

     // Instantiate a player named "Player 1" with some initial territories, enemies, hand, deck, and orders
     Player player1("Player 1", /* territories */ {}, /* enemies */ {}, /* hand */ handObj, /* deck */ deckObj, /* orders */ std::make_shared<OrdersList>(), /* negotiationList */ {});

     // Instantiate a territory named "Territory 1" at coordinates (0,0) and assign it to continent1 and player1 as the owner
     Territory territory1("Territory 1", 0, 0, &continent1);
     territory1.setOwnerPlayer(&player1);

     // Now you can instantiate orders with player1 and territory1

     // Deploy order
     auto deployOrder = std::make_shared<Deploy>(&player1, 5, territory1);

     // Advance order
     Territory sourceTerritory("Source Territory", 0, 0, &continent1); // You might need a source territory
     sourceTerritory.setOwnerPlayer(&player1);
     auto advanceOrder = std::make_shared<Advance>(&player1, 5, sourceTerritory, territory1);

     // Bomb order
     auto bombOrder = std::make_shared<Bomb>(&player1, territory1);

     // Blockade order
     auto blockadeOrder = std::make_shared<Blockade>(&player1, territory1);

     // Airlift order
     auto airliftOrder = std::make_shared<Airlift>(&player1, 5, sourceTerritory, territory1);

     // Negotiate order
     Player player2("Player 2", /* territories */ {}, /* enemies */ {}, /* hand */ nullptr, /* deck */ nullptr, /* orders */ std::make_shared<OrdersList>(), /*activeNegotiations*/ {});
     auto negotiateOrder = std::make_shared<Negotiate>(&player1, &player2);

     // Add the orders to the OrdersList
     playerOrders.addOrder(deployOrder);
     playerOrders.addOrder(advanceOrder);
     playerOrders.addOrder(bombOrder);
     playerOrders.addOrder(blockadeOrder);
     playerOrders.addOrder(airliftOrder);

     // Print out the orders to verify
     std::cout << playerOrders << std::endl;

     cout << "--------------- end: TESTING ORDERS-LISTS ---------------" << endl
          << endl;
}

void testOrderExecution()
{
     cout << "----------------- TESTING ORDER EXECUTION -----------------" << endl
          << endl;

     // Execute all orders in the player's OrdersList
     playerOrders.executeAll();

     cout << "--------------- end: TESTING ORDER EXECUTION ---------------" << endl
          << endl;
}

// Entry point for the application.
// int main()
//{
//    // Call testOrdersLists to demonstrate order and orders list functionality.
//    testOrdersLists();
//
//    // Assuming playerOrders is accessible here, if not, it should be returned or passed from testOrdersLists
//    testOrderExecution(playerOrders);
//
//    return 0;
//}
