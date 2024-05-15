#include "PlayerDriver.h"
#include "Player.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../OrdersList/Orders.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;



//int main()
//{
//    testPlayers();
//
//    cout << "it finished" << endl;
//    return 0;
//}

void testPlayers()
{
    std::cout << "------------------TESTING PLAYER-----------------" << std::endl;
    cout << endl;

    // Player name
    string player_name = "Player_1";

    // Creating test continents
    Continent *c1 = new Continent("Northern Alberta", 1);

    // Creating test territories
    Territory *t1 = new Territory("Territory01", 382, 194, c1);
    Territory *t2 = new Territory("Territory1A", 492, 345, c1);
    Territory *t3 = new Territory("Territory02", 689, 187, c1);
    Territory *t4 = new Territory("Territory03", 852, 246, c1);

    // Creating test vectors of territories
    vector<Territory *> player_territories = {t1, t2};
    vector<Territory *> enemy_territories = {t3, t4};

    // Creating test hand
    Hand *handObj = new Hand();

    // Create test deck
    Deck *deckObj = new Deck();

    // Creating test orders list
    std::shared_ptr<OrdersList> ordersList = std::make_shared<OrdersList>();

    Player player("Player_1", player_territories, enemy_territories, handObj, deckObj, ordersList, {});
    
    for (int i = 0; i < player.toDefend().size(); i++)
    {
        cout << player.toDefend()[i]->getTerritoryName() << endl;
    }

    for (int i = 0; i < player.toAttack().size(); i++)
    {
        cout << player.toAttack()[i]->getTerritoryName() << endl;
    }

    cout << endl;

    player.issueOrder();

    cout << endl;

    cout << *ordersList << endl;

    std::cout << "---------------end: TESTING PLAYER---------------" << std::endl;
    cout << endl;
}
