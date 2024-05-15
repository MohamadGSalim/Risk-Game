//
// Created by Krish on 11/21/2023.
//

#include "PlayerStrategies.h"
#include "GameContext.h"
using namespace std;


//-----------------------------------------------------------
// PlayerStrategies CLASS
//-----------------------------------------------------------

//PlayerStrategies::PlayerStrategies(Player* p) : player(p) {}

//-----------------------------------------------------------
// HumanPlayerStrategy CLASS
//-----------------------------------------------------------

/**
* requires user interactions to make decisions, including deploy and
* advance orders, as well as playing any card.
*/

void HumanPlayerStrategy::issueOrder(Player* p, string orderChoice) const {
    // Implement the human player's decision-making logic here
    cout << "Human player issues an order." << endl;
    /**
     * TODO: Makes any order using console user interactions
     * deploy : human can deploy to any territory owned by them - specify the territory to
     * deploy to and the number of armies that should be deployed
     * advance :
     * airlift : (needs card)
     * bomb : (needs card)
     * blockade : (needs card)
     */

    //Converts the string to lower
    transform(orderChoice.begin(), orderChoice.end(), orderChoice.begin(), ::tolower);

    bool valid = false;

    //Checks if order is valid
    if (orderChoice == "deploy" || orderChoice == "advance" || orderChoice == "airlift" || orderChoice == "bomb" || orderChoice == "blockade" || orderChoice == "negotiate"){
        valid = true;
    }

    //While order is not a valid option, prompt the user to enter order again
    while (!valid){
        cout << "\nInvalid order. Enter another order: ";
        cin >> orderChoice;

        if (orderChoice == "deploy" || orderChoice == "advance" || orderChoice == "airlift" || orderChoice == "bomb" || orderChoice == "blockade" || orderChoice == "negotiate") {
            valid = true;
        }
    }

    //Retried the owned territories from the player
    vector<Territory*> ownedTerritories = p->getPlayerTerritories();
    int reinforcementPool = p->getReinforcementPool();
    int numTerritory;
    int numArmies;

    //HUMAN DEPLOY
    if (orderChoice == "deploy"){
        cout << "\nIssued Order: " << "---DEPLOY---" << endl;

        if (reinforcementPool> 0){
            cout << "\nCurrent number of armies: " << reinforcementPool << endl;

            cout << "\nHow many armies would you like to deploy? ";
            cin >> numArmies;

            while (numArmies > reinforcementPool){
                cout << "You do not have that many armies to deploy. Pick a number below " << reinforcementPool;
                cin >> numArmies;
            }

            cout << "\nWhich territory would you like to deploy to?\n";
            //Displays each element of the player's owned territories - these are the territories they can advance to
            for (int i = 0; i < ownedTerritories.size(); i++){
                cout << i << ". " << ownedTerritories[i] << "\n";
            }

            cin >> numTerritory;

            //Checks user choice is in scope of the amount of elements in ownedTerritories
            while (numTerritory > ownedTerritories.size()){
                cout << "The territory you chose is not owned by you. Choose a territory from the following list: \n";
                for (int i = 0; i < ownedTerritories.size(); i++){
                    cout << i << ". " << ownedTerritories[i] << "\n";
                }
                cin >> numTerritory;
            }

            //Outputs order details
            std::cout << "Creating Deploy Order: ...." << std::endl;
            std::cout << "Player deploying: " << p->getPlayerID() << std::endl;
            std::cout << "Territory being deployed to: " << ownedTerritories.at(numTerritory)->getTerritoryName() << std::endl;
            std::cout << "Number of armies deployed: " << numArmies << std::endl;

            std::shared_ptr<Order> newDeploy = std::make_shared<Deploy>(p, numArmies, ownedTerritories.at(numTerritory));
            p->getOrderList()->addOrder(newDeploy);

        } else {
            cout << "\nYou do not have enough armies to deploy" << endl;
        }

    //HUMAN ADVANCE
    } else if (orderChoice == "advance"){
        std::cout << "Issued Order: " << "---ADVANCE---" << std::endl;
        //Retrieves enemy territories
        vector<Territory*> enemyTerritories = p->toAttack();

        cout << "Which territory would you like to advance from?";
        for (int i = 0; i < ownedTerritories.size(); i++){
            cout << i << ". " << ownedTerritories[i] << "\n";
        }

        cin >> numTerritory;

        //Checks user choice is in scope of the amount of elements in ownedTerritories
        while (numTerritory > ownedTerritories.size()){
            cout << "\nThe territory you chose is not owned by you. Choose a territory from the following list: \n";
            for (int i = 0; i < ownedTerritories.size(); i++){
                cout << i << ". " << ownedTerritories[i] << "\n";
            }
            cin >> numTerritory;
        }

        Territory* sourceTerritory = ownedTerritories.at(numTerritory);

        cout << "\nWhich territory would you like to advance to?";

        // Get neighbors of the selected territory
        vector<Territory*> neighbors = ownedTerritories[numTerritory]->getNeighbors();

        //Outputs the neighbors
        for (int i = 0; i < neighbors.size(); i++){
            cout << i << ". " << neighbors[i] << "\n";
        }

        int numTargetTerritory;
        cin >> numTargetTerritory;

        //Checks user choice is in scope of the amount of elements in neighbors
        while (numTargetTerritory > neighbors.size()){
            cout << "The territory you chose is not adjacent to the target territory. Choose a territory from the following list: \n";
            for (int i = 0; i < neighbors.size(); i++){
                cout << i << ". " << neighbors[i] << "\n";
            }
            cin >> numTargetTerritory;
        }

        Territory* targetTerritory = neighbors.at(numTargetTerritory);

        //Retrieves amount of available armies the user can deploy from source territory
        int availableArmies = sourceTerritory->getNumberOfArmies();
        cout << "\nHow many armies would you like like to deploy? (" << availableArmies << " available) ";

        int armiesToAdvance;
        cin >> armiesToAdvance;

        while (armiesToAdvance > availableArmies){
            cout << "You do not have that many armies to deploy. Pick a number below " << availableArmies;
            cin >> armiesToAdvance;
        }

        //Prints advance receipt
        std::cout << "Creating Advance Order: ...." << std::endl;
        std::cout << "Player deploying: " << p->getPlayerID() << std::endl;
        std::cout << "Territory being advanced to: " << neighbors[numTargetTerritory]->getTerritoryName() << std::endl;
        std::cout << "Territory being advanced from: " << ownedTerritories[numTerritory]->getTerritoryName() << std::endl;
        std::cout << "Number of armies deployed: " << armiesToAdvance << std::endl;

        //Create advance order and add it to player's orderList
        std::shared_ptr<Order> newAdvance = std::make_shared<Advance>(p, armiesToAdvance,
                                                                      targetTerritory, sourceTerritory);
        p->getOrderList()->addOrder(newAdvance);

    //HUMAN AIRLIFT
    } else if (orderChoice == "airlift"){
        std::cout << "Issued Order: " << "---AIRLIFT---" << std::endl;
        //Checks if player has card
        if (p->hasCard("airlift")){

            cout << "Which territory would you like to advance from?";
            for (int i = 0; i < ownedTerritories.size(); i++){
                cout << i << ". " << ownedTerritories[i] << "\n";
            }

            cin >> numTerritory;

            //Checks user choice is in scope of the amount of elements in ownedTerritories
            while (numTerritory > ownedTerritories.size()){
                cout << "\nThe territory you chose is not owned by you. Choose a territory from the following list: \n";
                for (int i = 0; i < ownedTerritories.size(); i++){
                    cout << i << ". " << ownedTerritories[i] << "\n";
                }
                cin >> numTerritory;
            }

            Territory* sourceTerritory = ownedTerritories.at(numTerritory);

            cout << "\nWhich territory would you like to advance to?";

            //Outputs owned territories excluding source territory
            for (int i = 0; i < ownedTerritories.size(); i++){
                if (i == numTerritory){
                    continue;
                } else {
                    cout << i << ". " << ownedTerritories[i] << "\n";
                }
            }

            int numTargetTerritory;
            cin >> numTargetTerritory;

            //Checks user choice is in scope of the amount of elements in neighbors
            while (numTargetTerritory > ownedTerritories.size()){
                cout << "The territory you chose is not adjacent to the target territory. Choose a territory from the following list: \n";
                for (int i = 0; i < ownedTerritories.size(); i++){
                    if (i == numTerritory){
                        continue;
                    } else {
                        cout << i << ". " << ownedTerritories[i] << "\n";
                    }
                }
                cin >> numTargetTerritory;
            }

            //Creates object targetTerritory at the position of numTargetTerritory (entered by the user)
            Territory* targetTerritory = ownedTerritories.at(numTargetTerritory);

            //Retrieves amount of available armies the user can deploy from source territory
            int availableArmies = sourceTerritory->getNumberOfArmies();
            cout << "\nHow many armies would you like like to deploy? (" << availableArmies << " available) ";

            int armiesToAirlift;
            cin >> armiesToAirlift;

            while (armiesToAirlift > availableArmies){
                cout << "You do not have that many armies to deploy. Pick a number below " << availableArmies;
                cin >> armiesToAirlift;
            }

            std::cout << "Creating Airlift Order user Airlift card: ...." << std::endl;

            //remove card from hand
            p->removeCard("airlift");

            //Print airlift receipt
            std::cout << "Player deploying: " << p->getPlayerID() << std::endl;
            std::cout << "Territory being Airlifted from: " << sourceTerritory->getTerritoryName() << std::endl;
            std::cout << "Territory being airlifted to: " << targetTerritory->getTerritoryName() << std::endl;
            std::cout << "Number of armies deployed: " << armiesToAirlift << std::endl;

            //Create Airlift order and give to player's orders list
            std::shared_ptr<Order> newAirlift = std::make_shared<Airlift>(p, armiesToAirlift,
                                                                          targetTerritory, sourceTerritory);
            p->getOrderList()->addOrder(newAirlift);

        } else {
            cout << "You cannot play Airlift as you do not have that card";
        }

    //HUMAN BOMB
    } else if (orderChoice == "bomb"){
        std::cout << "Issued Order: " << "---BOMB---" << std::endl;
        //Checks if player has card
        if (p->hasCard("bomb")){
            //Retrieves all enemy territories
            vector<Territory*> enemyTerritories = p->toAttack();

            cout << "\nWhich territory would you like to bomb? (Enter the number of the territory)";
            for (int i = 0; i < enemyTerritories.size();i++){
                cout << i << "." << enemyTerritories[i]->getTerritoryName();
            }

            int numTargetTerritory;
            cin >> numTargetTerritory;

            while (numTargetTerritory > enemyTerritories.size()){
                cout << "\nThat territory does not exist. Please chose one from the following: ";
                for (int i = 0; i < enemyTerritories.size();i++){
                    cout << i << "." << enemyTerritories[i]->getTerritoryName();
                }
                cin >> numTargetTerritory;
            }

            //Creates targetTerritory object
            Territory* targetTerritory = enemyTerritories.at(numTargetTerritory);

            std::cout << "Creating Bomb Order user Bomb card: ...." << std::endl;

            //Remove card from player's hand
            p->removeCard("bomb");

            //Prints bomb receipt
            std::cout << "Player deploying: " << p->getPlayerID() << std::endl;
            std::cout << "Territory being Bombed to: " << targetTerritory->getTerritoryName() << std::endl;

            //Create Bomb order and give to player's orders list
            std::shared_ptr<Order> newBomb = std::make_shared<Bomb>(p, targetTerritory);
            p->getOrderList()->addOrder(newBomb);
        } else {
            cout << "You cannot play Bomb as you do not have that card";
        }

    //HUMAN BLOCKADE
    } else if (orderChoice == "blockade"){
        std::cout << "Issued Order: " << "---BLOCKADE---" << std::endl;
        //Checks if player has card
        if (p->hasCard("blockade")){
            cout << "\nWhich territory do you want to target?";

            for (int i = 0; i < ownedTerritories.size(); i++){
                cout << i << ". " << ownedTerritories[i] << "\n";
            }

            cin >> numTerritory;

            //Checks user choice is in scope of the amount of elements in ownedTerritories
            while (numTerritory > ownedTerritories.size()){
                cout << "\nThe territory you chose is not owned by you. Choose a territory from the following list: \n";
                for (int i = 0; i < ownedTerritories.size(); i++){
                    cout << i << ". " << ownedTerritories[i] << "\n";
                }
                cin >> numTerritory;
            }

            Territory* targetTerritory = ownedTerritories.at(numTerritory);

            p->removeCard("blockade");

            //Prints blockade receipt
            std::cout << "Player blockading: " << p->getPlayerID() << std::endl;
            std::cout << "Territory being Bombed to: " << targetTerritory->getTerritoryName() << std::endl;

            //Create Blockade order and give to player's orders list
            std::shared_ptr<Order> newBlockade = std::make_shared<Blockade>(p, targetTerritory);
            p->getOrderList()->addOrder(newBlockade);

            //TRANSFER OWNERSHIP TO NEUTRAL PLAYER

        } else {
            cout << "\nYou cannot play Blockade as you do not have that card";
        }

    } else if (orderChoice == "diplomacy"){
        if (p->hasCard("diplomacy")){
            //Creates a local vector of player
            vector<Player*> players = GameContext::gamePlayers;

            Player* issuingPlayer = p;

            cout << "\nEnter the number of the player you wish to negotiate with: \n";
            for (int i = 0; i < players.size(); i++){
                if (players[i] == issuingPlayer){
                    continue;
                } else {
                    cout << i << ". " << players[i]->getPlayerID();
                }

            }
            int chosenPlayer;
            cin >> chosenPlayer;

            while (chosenPlayer > players.size()){
                cout << "\nThat player does not exist. Please choose another player from the following list: \n";
                for (int i = 0; i < players.size(); i++){
                    if (players[i] == issuingPlayer){
                        continue;
                    } else {
                        cout << i << ". " << players[i]->getPlayerID();
                    }
                }
                cin >> chosenPlayer;
            }

            //Creates targetPlayer object
            Player* targetPlayer = players[chosenPlayer];

            p->removeCard("diplomacy");

            //Prints diplomacy receipt
            std::cout << "Issuing Player: " << p->getPlayerID() << std::endl;
            std::cout << "Target Player: " << targetPlayer->getPlayerID() << std::endl;

            //Create Blockade order and give to player's orders list
            std::shared_ptr<Order> newNegotiate = std::make_shared<Negotiate>(issuingPlayer, targetPlayer);
            p->getOrderList()->addOrder(newNegotiate);

        } else {
            cout << "\nYou cannot play Negotiate as you do not have the Diplomacy card";
        }


    }
};

vector<Territory* > HumanPlayerStrategy::toAttack(Player* player) const {
    /**
     * TODO: return the vector of Player p's enemy territories
     * Human player returns all adjacent territories to their ownded territories
     * that are not owned by them in no particular order (all adjacent enemy territories)
     */

    vector<Territory* > enemyTerritories;

    //Go through owned territories neighbors
    for(Territory* ownedTerritory: player->getPlayerTerritories()){
        for(Territory* neighbor: ownedTerritory->getNeighbors()){
            //if the neighbor territory does not belong to me, add it to enemy territory
            if(neighbor->getOwnerPlayer() != player){
                enemyTerritories.push_back(neighbor);
            }
        }
    }

    // Remove duplicates
    auto newEnd = std::unique(enemyTerritories.begin(), enemyTerritories.end());
    enemyTerritories.erase(newEnd, enemyTerritories.end());

    //set the player's enemy territories to this vector
    player->setEnemyTerritories(enemyTerritories);

    return enemyTerritories;
};

vector<Territory* > HumanPlayerStrategy::toDefend(Player* player) const {
    /**
    * TODO: return the vector of Player p's owned territories
    * Human player returns all the territories that they own in no specific order
    */

    return player->getPlayerTerritories();

};

string HumanPlayerStrategy::getStrategyName() const {
    return "Human";
};

//-----------------------------------------------------------
// AggressivePlayerStrategy CLASS
//-----------------------------------------------------------

/**
* computer player that focuses on attack (deploys or advances armies on its strongest country,
* then always advances to enemy territories until it cannot do so anymore).
*/

void  AggressivePlayerStrategy::issueOrder(Player* player, string orderName) const {
    // Implement the human player's decision-making logic here
    cout << "Aggressive player issues an order." << endl;
    /**
     * TODO: focuses on attacking territory, will enhance its strongest territories first
     * deploy : give random number of armies to its strongest territory
     * advance : transfer armies from randomly owned territory (neighbor to weak territory) to the strangest territory
     * then try to advance to every single enemy territory until it cant (no more reinforcement pool strong enough)
     * airlift : (needs card) airlift to the weakest enemy territory on the map
     * bomb : (needs card) play the bomb card on its strongest enemy adjacent to its strongest enemy
     * blockade : (needs card) not played since ownership is transferred to Neutral player
     * negotiate : (needs card) negotiate order restricts attacks for the remainder of the turn, not used
     */

    //DEPLOY
    if(orderName == "deploy"){
        std::cout << "Issued Order: " << "---DEPLOY---" << std::endl;

        vector<Territory*> strongestTerritories = player->toDefend();
        vector<Territory*> enemyTerritories = player->toAttack();
        int reinforcementPool = player->getReinforcementPool();

        //Aggressive Player will deploy its reinforcement pool to its atrongest territory
        std::cout << "Current number of armies: " << reinforcementPool << std::endl;

        if(reinforcementPool>0){
            //Find a random number of armies to deploy
            int armiesToDeploy = (rand() % player->getReinforcementPool()) + 1;

                //make sure that the player owner of the weakest territory is owned by player
                if(strongestTerritories.at(0)->getOwnerPlayer() == player){

                    //take out from reinforcement pool number of armies meant to deploy
                    player->setReinforcementPool(reinforcementPool-armiesToDeploy);

                    std::cout << "Creating Deploy Order: ...." << std::endl;
                    std::cout << "Player deploying: " << player->getPlayerID() << std::endl;
                    std::cout << "Territory being deployed to: " << strongestTerritories.at(0)->getTerritoryName() << std::endl;
                    std::cout << "Number of armies deployed: " << armiesToDeploy << std::endl;

                    std::shared_ptr<Order> newDeploy = std::make_shared<Deploy>(player, armiesToDeploy, strongestTerritories.at(0));
                    player->getOrderList()->addOrder(newDeploy);
                    return;
                }else{
                    std::cout << "Target Territory does not belong to current Player" << std::endl;
                    return;
            }
        }else{
            std::cout << "Sorry, the player does not have any armies left to deploy" << std::endl;
            return;
        }

    //ADVANCE
    }else if(orderName == "advance"){
        std::cout << "Issued Order: " << "---ADVANCE---" << std::endl;

        vector<Territory*> strongestTerritories = player->toDefend();
        vector<Territory*> enemyTerritories = player->toAttack();

        //First, Aggressive Player will advance to its strongest territory
        if (!strongestTerritories.empty()) {
            // Select strongest territory
            Territory* strongestTerritory = strongestTerritories.at(0);

            // Get neighbors of the strong territory
            vector<Territory*> neighbors = strongestTerritory->getNeighbors();

            // If there are neighbors, transfer armies from a random neighbor to the strong territory
            if (!neighbors.empty()) {

                Territory* randomNeighbor;

                do{
                    //get random neighbor of the weakest territory but
                    // make sure its a territory player owns from 1 to end
                    //advance armies only from your owned territory
                    randomNeighbor = neighbors.at(1 + rand() % (neighbors.size() - 1));

                }while(randomNeighbor->getOwnerPlayer() != player);

                int armiesToTransfer = (rand() % randomNeighbor->getNumberOfArmies());

                std::cout << "Creating Advance Order: ...." << std::endl;
                std::cout << "Player deploying: " << player->getPlayerID() << std::endl;
                std::cout << "Territory being advanced to: " << strongestTerritory->getTerritoryName() << std::endl;
                std::cout << "Territory being advanced from: " << randomNeighbor->getTerritoryName() << std::endl;
                std::cout << "Number of armies deployed: " << armiesToTransfer << std::endl;

                //create advance order and add it to player's orderList
                std::shared_ptr<Order> newAdvance = std::make_shared<Advance>(player, armiesToTransfer, randomNeighbor, strongestTerritory);
                player->getOrderList()->addOrder(newAdvance);
            } else {
                cout << "No neighbors for the strong territory." << endl;
            }
        } else {
            cout << "No strong territories to advance armies to." << endl;
        }

        //Second, Aggressive player will advance (attack) until it does not have any armies left in its strongest

        while(strongestTerritories.at(0)->getNumberOfArmies() > 1) {
            std::cout << "Issued Order: " << "---ATTACK ADVANCE---" << std::endl;

            Territory *strongestTerritory = strongestTerritories.at(0);
            Territory *targetTerritory = nullptr;

            for (Territory *enemy: enemyTerritories) {
                for (Territory *neighbor: strongestTerritory->getNeighbors()) {

                    if (neighbor == enemy) {
                        targetTerritory = enemy;
                        break;
                    }
                }

                int armiesToAdvance = std::min(strongestTerritory->getNumberOfArmies(),
                                               rand() % strongestTerritory->getNumberOfArmies() + 1);

                if (targetTerritory) {
                    std::cout << "Creating Advance Order: ...." << std::endl;
                    std::cout << "Player deploying: " << player->getPlayerID() << std::endl;
                    std::cout << "Territory being advanced from: " << strongestTerritory->getTerritoryName() << std::endl;
                    std::cout << "Territory being advanced to: " << targetTerritory->getTerritoryName() << std::endl;
                    std::cout << "Number of armies deployed: " << armiesToAdvance << std::endl;

                    //create advance order and add it to player's orderList
                    std::shared_ptr<Order> newAdvance = std::make_shared<Advance>(player, armiesToAdvance,
                                                                                  targetTerritory, strongestTerritory);
                    //Execute or ??? just add the order
                    //Could keep a tracker of the  armies of the strongest territory
                    //create advance orders with that number?
                    newAdvance->execute();

                    enemyTerritories = player->toAttack();
                }
                else{
                    std::cout << "Sorry, no enemy territory to attack was found." << std::endl;
                    return;
                }
            }
        }

        std::cout << "Aggressive Player's strongest territory has no more armies to advance/attack." << endl;

    //AIRLIFT
    }else if(orderName == "airlift"){
        std::cout << "Issued Order: " << "---AIRLIFT---" << std::endl;

        Hand* playerHand = player->getHand();
        vector<Territory*> strongestTerritories = player->toDefend();
        vector<Territory*> allTerritories = player->getContext()->gameMap->getMapTerritories();
        vector<Territory*> allEnemyTerritories;

        std::cout << "Aggressive Player's current cards: " << std::endl;

        for(Card* card: playerHand->getHand()){
            std::cout << " Card Name: " << card->getType() << endl;
        }

        //Check if player has a airlift card
        if(player->hasCard("airlift")){

            //if yes, issue airlift order
            //from strongest owned territory as source territory
            //random number of armies
            if(!strongestTerritories.empty()){

                Territory* strongestTerritory = strongestTerritories.at(0);
                Territory* randomTerritory;

                // Collect all territories not owned by the aggressive player
                for (Territory* territory : allTerritories) {
                    if (territory->getOwnerPlayer() != player) {
                        allEnemyTerritories.push_back(territory);
                    }
                }

                // Check if there are any enemy territories
                if (!allEnemyTerritories.empty()) {
                    // Choose a random enemy territory
                   randomTerritory = allEnemyTerritories[rand() % allEnemyTerritories.size()];
                } else {
                    // No enemy territories found
                    randomTerritory = nullptr;
                }

                if(randomTerritory != nullptr) {
                    int armiesToTransfer = (rand() % randomTerritory->getNumberOfArmies());

                    std::cout << "Creating Airlift Order user Airlift card: ...." << std::endl;

                    //remove card from hand
                    player->removeCard("airlift");

                    std::cout << "Player deploying: " << player->getPlayerID() << std::endl;
                    std::cout << "Territory being Airlifted from: " << strongestTerritory->getTerritoryName() << std::endl;
                    std::cout << "Territory being airlifted to: " << randomTerritory->getTerritoryName() << std::endl;
                    std::cout << "Number of armies deployed: " << armiesToTransfer << std::endl;

                    //create Airlift order and give to player's orders list
                    std::shared_ptr<Order> newAirlift = std::make_shared<Airlift>(player, armiesToTransfer,
                                                                                  randomTerritory, strongestTerritory);
                    player->getOrderList()->addOrder(newAirlift);
                    return;
                }else{
                    std::cout << "No enemy territory" << endl;
                }
            }else{
                cout << "No territories to airlift attack armies to." << endl;
                return;
            }
        }else{
            //else, airlift order cannot be issued and return
            cout << "No airlift card owned, cannot call airlift order." << endl;
            return;
        }

    //BOMB
    }else if(orderName == "bomb"){
        std::cout << "Issued Order: " << "---BOMB---" << std::endl;

        Hand* playerHand = player->getHand();
        vector<Territory*> strongestTerritories = player->toDefend();
        vector<Territory*> allTerritories = player->getContext()->gameMap->getMapTerritories();
        vector<Territory*> allEnemyTerritories;

        std::cout << "Aggressive Player's current cards: " << std::endl;

        for(Card* card: playerHand->getHand()){
            std::cout << " Card Name: " << card->getType() << endl;
        }

        if(player->hasCard("bomb")){

            Territory* strongestTerritory = strongestTerritories.at(0);
            Territory* targetTerritory;

            for(Territory* neighbor: strongestTerritory->getNeighbors()){
                if(neighbor->getOwnerPlayer() !=player){
                    targetTerritory = neighbor;
                    break;
                }
            }

            if(targetTerritory){

                std::cout << "Creating Bomb Order user Bomb card: ...." << std::endl;

                //remove card from hand
                player->removeCard("bomb");

                std::cout << "Player deploying: " << player->getPlayerID() << std::endl;
                std::cout << "Territory being Bombed from: " << strongestTerritory->getTerritoryName() << std::endl;
                std::cout << "Territory being Bombed to: " << targetTerritory->getTerritoryName() << std::endl;

                //create Bomb order and give to player's orders list
                std::shared_ptr<Order> newBomb = std::make_shared<Bomb>(player, targetTerritory);
                player->getOrderList()->addOrder(newBomb);
                return;
            }else{
                cout << "No territory to bomb." << endl;
            }
        }else{
            //blockade and negotiate are not valid orders
            std::cout << "The Aggressive Player does not have a Bomb card." << endl;
            return;
        }
    }else{
        std::cout << "Sorry, this order does not fit with Aggressive Player strategy" << std::endl;
    }

};

vector<Territory* >  AggressivePlayerStrategy::toAttack(Player* player) const {
    /**
     * TODO: return the vector of Player p's enemy territories
     * The Aggressive player will return a vector of adjacent enemy territories ordered by weakest
     * (least armies) to strongest (most armies)
     */

    vector<Territory*> enemyTerritories;

    for (Territory* ownedTerritory : player->getPlayerTerritories()) {
        for (Territory* neighbor : ownedTerritory->getNeighbors()) {
            if (neighbor->getOwnerPlayer() != player) {
                enemyTerritories.push_back(neighbor);
            }
        }
    }

    // Remove duplicates
    auto newEnd = std::unique(enemyTerritories.begin(), enemyTerritories.end());
    enemyTerritories.erase(newEnd, enemyTerritories.end());

    // Order by weakest to strongest (least armies to most armies)
    std::sort(enemyTerritories.begin(), enemyTerritories.end(), [](Territory* a, Territory* b) {
        return a->getNumberOfArmies() < b->getNumberOfArmies();
    });

    player->setEnemyTerritories(enemyTerritories);
    return enemyTerritories;
};

vector<Territory* >  AggressivePlayerStrategy::toDefend(Player* player) const {
    /**
     * TODO: return the vector of Player p's owned Territories
     * The Aggressive player returns a vector of its owned territories ordered by strongest to weakest
     * Maybe: Aggressive Player will defend its strongest territory aka the one with the highest armies
     */

    vector<Territory*> ownedTerritories = player->getPlayerTerritories();

    // Order owned territories from strongest to weakest (highest armies to lowest armies)
    std::sort(ownedTerritories.begin(), ownedTerritories.end(), [](Territory* a, Territory* b) {
        return a->getNumberOfArmies() > b->getNumberOfArmies();
    });

    player->setPlayerTerritories(ownedTerritories);
    return ownedTerritories;
};

string AggressivePlayerStrategy::getStrategyName() const {
    return "Aggressive";
};

//-----------------------------------------------------------
// BenevolentPlayerStrategy CLASS
//-----------------------------------------------------------

/**
* computer player that focuses on protecting its weak countries
* (deploys or advances armies on its weakest countries, never advances to enemy territories).
*/

void  BenevolentPlayerStrategy::issueOrder(Player* player, string orderName) const {
    // Implement the human player's decision-making logic here
    cout << "Benevolent player issues an order." << endl;
    /**
     * TODO: A Benevolent Player only focuses on orders to protect its own terr:
     * deploy : give random number of armies to weak territory
     * advance : transfer armies from adjacent randomly owned territory (neighbor to weak territory) to the weakest territory
     * airlift : (needs card) transfer armies from any owned territory to weak territory
     * bomb : bomb only targets enemy territory so not used
     * blockade : (needs card) Benevolent player would not play this order
     * negotiate : (needs card)  negotiate order targets an enemy player. Benevolent player doesn't have enemies
     */

    vector<Territory* > weakestTerritories = player->toDefend();
    int reinforcementPool = player->getReinforcementPool();

    if(orderName == "deploy"){

        std::cout << "Issued Order: " << "---DEPLOY---" << std::endl;

        //Benevolent Player will deploy its reinforcement pool to its weakest territory
        std::cout << "Current number of armies: " << reinforcementPool << std::endl;

        if(reinforcementPool>0){
            //Find a random number of armies to deploy
            int armiesToDeploy = (rand() % player->getReinforcementPool()) + 1;

            //make sure that the player owner of the weakest territory is owned by player
            if(weakestTerritories.at(0)->getOwnerPlayer() == player){

                //take out from reinforcement pool number of armies meant to deploy
                player->setReinforcementPool(reinforcementPool-armiesToDeploy);

                std::cout << "Creating Deploy Order: ...." << std::endl;
                std::cout << "Player deploying: " << player->getPlayerID() << std::endl;
                std::cout << "Territory being deployed to: " << weakestTerritories.at(0)->getTerritoryName() << std::endl;
                std::cout << "Number of armies deployed: " << armiesToDeploy << std::endl;

                std::shared_ptr<Order> newDeploy = std::make_shared<Deploy>(player, armiesToDeploy, weakestTerritories.at(0));
                player->getOrderList()->addOrder(newDeploy);
                return;
            }else{
                std::cout << "Target Territory does not belong to current Player" << std::endl;
                return;
            }
        }else{
            std::cout << "Sorry you don't have enough armies to deploy" << std::endl;
            return;
        }
    }else if(orderName == "advance"){
        std::cout << "Issued Order: " << "---ADVANCE---" << std::endl;

        if (!weakestTerritories.empty()) {
            // Select weakest territory
            Territory* weakestTerritory = weakestTerritories.at(0);

            // Get neighbors of the weak territory
            vector<Territory*> neighbors = weakestTerritory->getNeighbors();

            // If there are neighbors, transfer armies from a random neighbor to the weak territory
            if (!neighbors.empty()) {

                Territory* randomNeighbor;

                do{
                    //get random neighbor of the weakest territory but
                    // make sure its a territory player owns from 1 to end
                    //advance only works for adjacent territories and benevolent does NOT attack
                    randomNeighbor = neighbors.at(1 + rand() % (neighbors.size() - 1));

                }while(randomNeighbor->getOwnerPlayer() != player);

                int armiesToTransfer = (rand() % randomNeighbor->getNumberOfArmies());

                std::cout << "Creating Advance Order: ...." << std::endl;
                std::cout << "Player deploying: " << player->getPlayerID() << std::endl;
                std::cout << "Territory being advanced to: " << weakestTerritory->getTerritoryName() << std::endl;
                std::cout << "Territory being advanced from: " << randomNeighbor->getTerritoryName() << std::endl;
                std::cout << "Number of armies deployed: " << armiesToTransfer << std::endl;

                //create advance order and add it to player's orderList
                std::shared_ptr<Order> newAdvance = std::make_shared<Advance>(player, armiesToTransfer, randomNeighbor, weakestTerritory);
                player->getOrderList()->addOrder(newAdvance);
                return;

            } else {
                cout << "No neighbors for the weak territory." << endl;
                return;
            }
        } else {
            cout << "No weak territories to advance armies to." << endl;
            return;
        }
    }else if (orderName == "airlift"){
        std::cout << "Issued Order: " << "---Airlift---" << std::endl;

        Hand* playerHand = player->getHand();

        std::cout << "Benevolent Player's current cards: " << std::endl;

        for(Card* card: playerHand->getHand()){
            std::cout << " Card Name: " << card->getType() << endl;
        }

        //Check if player has a airlift card
        if(player->hasCard("airlift")){

            //if yes, issue airlift order
            //from random owned territory as source territory
            //random number of armies
            if(!weakestTerritories.empty()){

                Territory* weakestTerritory = weakestTerritories.at(0);

                //TO-DO : take the strongest territories
                //Make sure to not take entire number of armies from source territory

                Territory* randomTerritory = weakestTerritories.at(1 + rand() % (weakestTerritories.size() - 1));

                int armiesToTransfer = (rand() % randomTerritory->getNumberOfArmies());

                std::cout << "Creating Airlift Order user Airlift card: ...." << std::endl;

                //remove card from hand
                player->removeCard("airlift");

                std::cout << "Player deploying: " << player->getPlayerID() << std::endl;
                std::cout << "Territory being Airlifted to: " << weakestTerritory->getTerritoryName() << std::endl;
                std::cout << "Territory being airlifted from: " << randomTerritory->getTerritoryName() << std::endl;
                std::cout << "Number of armies deployed: " << armiesToTransfer << std::endl;

                //create Airlift order and give to player's orders list
                std::shared_ptr<Order> newAirlift = std::make_shared<Airlift>(player, armiesToTransfer, randomTerritory, weakestTerritory);
                player->getOrderList()->addOrder(newAirlift);
                return;

            }else{
                cout << "No weak territories to airlift armies to." << endl;
                return;
            }
        }else{
            //else, airlift order cannot be issued and return
            cout << "No airlift card owned, cannot call airlift order." << endl;
            return;
        }
    }else{
        std::cout << "Benevolent Player cannot issue any other order but Deploy, Airlift and Advance. " <<
                     "Benevolent Player focuses on protecting its weak territories and does " << std::endl ;
        return;
    }
};

vector<Territory* >  BenevolentPlayerStrategy::toAttack(Player* player) const {
    /**
     * TODO: return the vector of Player p's enemy territories
     * The Benevolent player never attacks other players
     */

    player->setEnemyTerritories({});
    return {};

};

vector<Territory* >  BenevolentPlayerStrategy::toDefend(Player* player) const {
    /**
     * TODO: return the vector of Player p's owned territories
     * The Benevolent player returns a vector of owned territories where we prioritize territories that
     * have ajdacent enemy territories first and also sorting depending on who has more armies
     */

    vector<Territory*> ownedTerritories = player->getPlayerTerritories();

    // Define a custom comparison function to sort territories by the number of adjacent enemy territories
    auto compareByAdjacentEnemies = [player](Territory* a, Territory* b) {
        // Count the number of adjacent enemy territories for each territory
        int adjacentEnemiesA = 0;
        int adjacentEnemiesB = 0;

        for (Territory* neighborA : a->getNeighbors()) {
            if (neighborA->getOwnerPlayer() != player) {
                adjacentEnemiesA++;
            }
        }

        for (Territory* neighborB : b->getNeighbors()) {
            if (neighborB->getOwnerPlayer() != player) {
                adjacentEnemiesB++;
            }
        }

        // Compare by the number of adjacent enemy territories first
        if (adjacentEnemiesA != adjacentEnemiesB) {
            return adjacentEnemiesA > adjacentEnemiesB;
        }

        // If the number of adjacent enemy territories is the same, compare by the number of armies
        return a->getNumberOfArmies() > b->getNumberOfArmies();
    };

    // Sort owned territories based on the number of adjacent enemy territories
    std::sort(ownedTerritories.begin(), ownedTerritories.end(), compareByAdjacentEnemies);

    player->setPlayerTerritories(ownedTerritories);
    return ownedTerritories;

};

string BenevolentPlayerStrategy::getStrategyName() const {
    return "Benevolent";
};

//-----------------------------------------------------------
// NeutralPlayerStrategy CLASS
//-----------------------------------------------------------

/**
* computer player that never issues any order.
*/

void  NeutralPlayerStrategy::issueOrder(Player* p, string orderName) const {
    // Implement the human player's decision-making logic here
    cout << "Neutral player issues an order." << endl;
    /**
     * A neutral player never issues an order
     */

    std::cout << " This Player doesn't issue any orders (Neutral Player)" << std::endl;
};

vector<Territory* >  NeutralPlayerStrategy::toAttack(Player* player) const {
    /**
     * TODO: return the vector of Player p's enemy territories
     * A Neutral player does not have territories to attack
     */

    player->setEnemyTerritories({});
    return {};

};

vector<Territory* >  NeutralPlayerStrategy::toDefend(Player* p) const {
    /**
     * TODO: return the vector of Player p's owned territories
     *?? A neutral player does not defend its territories either
     */

    return {};
};

string NeutralPlayerStrategy::getStrategyName() const {
    return "Neutral";
};

//-----------------------------------------------------------
// CheaterPlayerStrategy CLASS
//-----------------------------------------------------------

/**
* computer player that automatically conquers all
* territories that are adjacent to its own territories (only once per turn).
*/

void CheaterPlayerStrategy::issueOrder(Player* player, string orderName) const {
    // Implement the human player's decision-making logic here
    cout << "Cheater player issues an order." << endl;
    /**
     * The cheater player never issues any orders, he doesn't use cards either, everytime they
     * "issue" an order, they conquer over every enemy
     */

    // Use a set to keep track of conquered territories
    std::unordered_set<Territory*> conqueredTerritories;

    for(Territory* ownedTerritories: player->getPlayerTerritories()){
        for(Territory* neighbor: ownedTerritories->getNeighbors()){

            if(neighbor->getOwnerPlayer() != player &&
            conqueredTerritories.find(neighbor) == conqueredTerritories.end()){

                //change owner of the Territory to current cheater player and add it to owned territories
                neighbor->setOwnerPlayer(player);
                player->addTerritory(neighbor);

                conqueredTerritories.insert(neighbor);
            }
        }
    }

    std::cout << "Cheater Player conquered: " << std::endl;
    for(Territory* territory: conqueredTerritories){
        std::cout << territory->getTerritoryName() << std::endl;
    }

};

vector<Territory* >  CheaterPlayerStrategy::toAttack(Player* player) const {
    /**
     * TODO: return the vector of Player p's enemy territories
     * A cheater does not have a vector to attack, it just conquers adjacent territories
     */

    player->setEnemyTerritories({});
    return {};
};

vector<Territory* >  CheaterPlayerStrategy::toDefend(Player* player) const {
    /**
     * TODO: return the vector of Player p's own territories
     * A cheater does not have territories to defend
     */

    return {};
};

string CheaterPlayerStrategy::getStrategyName() const {
    return "Cheater";
};