// Driver File
#include "GameEngine.h"
#include "../CommandProcessor/commandProcessing.h"
#include <iostream>  //Include Input/Output
#include <string>    //Include String Library
using namespace std; // Using the standard namespace

// Free function to test the current state of currentState with objects passes as reference
void testGameStates()
{

    std::cout << "-----------------TESTING GAME-ENGINE-----------------" << std::endl;
    cout << endl;

    bool end = false;

    State currentState;    // Initalises the object currentState as a member of class State
    Command customCommand; // Initalises the object customCommand as a member of class Command
    cout << currentState;

    // currentState.getState(currentState.GEState);

    while (end == false)
    { // While loop to run indefinitely
        //		std::cout << "\nEnter the command: "; //Prompts the user to input a command
        //		cin >> customCommand; // Assigns the input to customCommand
        // use a switch statement for state machine transitions

        int intState = State::getState(currentState.GEState);
        currentState.setintCurrentState(currentState.GEState);

        currentState.getintCurrentState();

        cout << "\nCurrent state: " << intState;

        string transition = customCommand.assignCommand(intState);

        switch (*currentState.GEState)
        {       // Takes attribute GEState of currentState as the enum value
        case 0: // State: Start
            // If statement to check appropriate commands
            if (transition == "loadmap")
            {
                currentState.changeState(1); // Call to member function changeState with a parameter of 1
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 1: // State: Map Loaded
            if (transition == "validatemap")
            {
                currentState.changeState(2);
            }
            /*State transition goes back into map loaded,
            so the state doesn't change. It isn't an invalid command so the break
            tidies up the code and solves a logic error.*/
            else if (transition == "loadmap")
            {
                break;
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 2: // State: Map Validated
            if (transition == "addplayer")
            {
                currentState.changeState(3);
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 3: // State: Players Added
            if (transition == "assigncountries")
            {
                currentState.changeState(4);
            }
            else if (transition == "addplayer")
            {
                break;
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 4: // Assign Reinforcement
            if (transition == "issueorder")
            {
                currentState.changeState(5);
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 5: // Issue Orders
            if (transition == "endissueorders")
            {
                currentState.changeState(6);
            }
            else if (transition == "issueorder")
            {
                break;
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 6: // Execute Orders
            if (transition == "endexecorders")
            {
                currentState.changeState(4);
            }
            else if (transition == "execorder")
            {
                break;
            }
            else if (transition == "win")
            {
                currentState.changeState(7);
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 7:
            // Win
            if (transition == "play")
            {
                currentState.changeState(0);
            }
            else if (transition == "end")
            {
                end = true;
                exit(0);
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        default: // Default in the instance an invalid parameter is passed into the switch statement
            std::cout << "A valid command was not given" << endl;
            break;
        }

        cout << currentState; // Outputs to the console the current state
        cout << endl;
    }
}

void testStartUpPhase()
{

    std::cout << "--------------- TESTING START UP PHASE --------------" << std::endl;
    std::cout << "" << std::endl;

    GameContext context;
    context.gameMap = nullptr;
    context.deckOfCards = new Deck(12);
    context.gamePlayers;

    bool end = false;

    State currentState;    // Initalises the object currentState as a member of class State
    Command customCommand; // Initalises the object customCommand as a member of class Command
    cout << currentState;

    // currentState.getState(currentState.GEState);

    while (end == false)
    { // While loop to run indefinitely
        // use a switch statement for state machine transitions

        int intState = State::getState(currentState.GEState);
        currentState.setintCurrentState(currentState.GEState);

        currentState.getintCurrentState();

        cout << currentState << endl;

        // string transition = customCommand.assignCommand(intState);
        string transition;
        std::cout << "Enter command: ";
        std::cin >> transition;

        switch (*currentState.GEState)
        {       // Takes attribute GEState of currentState as the enum value
        case 0: // State: Start
            // If statement to check appropriate commands
            if (transition == "loadmap")
            {
                startupPhase(context, transition);
                currentState.changeState(1); // Call to member function changeState with a parameter of 1
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 1: // State: Map Loaded
            if (transition == "validatemap")
            {
                if (startupPhase(context, transition))
                {
                    currentState.changeState(2);
                }
                else
                {
                    cout << "MAP NOT VALID, RETURN TO LOADMAP" << endl;
                    currentState.changeState(0);
                }
            }
            /*State transition goes back into map loaded,
            so the state doesn't change. It isn't an invalid command so the break
            tidies up the code and solves a logic error.*/
            else if (transition == "loadmap")
            {
                break;
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 2: // State: Map Validated
            if (transition == "addplayer")
            {
                startupPhase(context, transition);
                currentState.changeState(3);
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        case 3: // State: Players Added
            if (transition == "gamestart")
            {
                startupPhase(context, transition);
                currentState.changeState(4);
                end = true;
            }
            else if (transition == "addplayer")
            {
                break;
            }
            else
            {
                std::cout << "Invalid command" << endl;
            }
            break;

        default: // Default in the instance an invalid parameter is passed into the switch statement
            std::cout << "A valid command was not given" << endl;
            break;
        }

        cout << currentState; // Outputs to the console the current state
        cout << endl;
    }

    std::cout << "--------------- END OF TESTING START UP PHASE --------------" << std::endl;
    std::cout << "" << std::endl;
}
//
// int main(){
//    testStartUpPhase();
//}

void testMainGameLoop()
{
    std::cout << "--------------- TESTING MAIN GAME LOOP --------------" << std::endl;
    std::cout << "" << std::endl;

    OrdersList playerOrders;

    std::cout << "1" << std::endl;

    // Instantiate a player named "Player 1" with some initial territories, enemies, hand, deck, and orders

    // Creating players
    Player player1("Player 1", /* territories */ {}, /* enemies */ {}, /* hand */ nullptr, /* deck */ nullptr, /* orders */ std::make_shared<OrdersList>(), {});
    Player player2("Player 2", /* territories */ {}, /* enemies */ {}, /* hand */ nullptr, /* deck */ nullptr, /* orders */ std::make_shared<OrdersList>(), {});

    std::cout << "2" << std::endl;
    // Creating test continents
    Continent *c1 = new Continent("Northern Alberta", 5);
    Continent *c2 = new Continent("Southern Alberta", 10);

    // Creating test territories
    Territory *t1 = new Territory("Territory01", 382, 194, c1);
    t1->setOwnerPlayer(&player1);
    Territory *t2 = new Territory("Territory1A", 492, 345, c1);
    t2->setOwnerPlayer(&player1);
    Territory *t3 = new Territory("Territory02", 689, 187, c1);
    t3->setOwnerPlayer(&player2);
    Territory *t4 = new Territory("Territory03", 852, 246, c2);
    t4->setOwnerPlayer(&player2);

    c1->setContinentTerritories({t1, t2, t3});
    c2->setContinentTerritories({t4});

    // Creating vector of continents
    vector<Continent *> continents = {c1, c2};
    // Creating test vectors of territories
    vector<Territory *> player1_territories = {t1, t2};
    vector<Territory *> player2_territories = {t3, t4};

    player1.setPlayerTerritories(player1_territories);
    player2.setPlayerTerritories(player2_territories);
    player1.setEnemyTerritories(player2_territories);
    player2.setEnemyTerritories(player1_territories);

    player1.setReinforcementPool(15);
    player2.setReinforcementPool(0);

    std::vector<Player *> players = {&player1, &player2};
    Hand *playerHand = new Hand();
    Card *playerCard = new Card(Card::Type::Bomb);

    playerHand->addCard(playerCard);
    player2.setHand(playerHand);

    auto deployOrder = std::make_shared<Deploy>(&player1, 5, t1);

    player1.getOrderList()->addOrder(deployOrder);

    mainGameLoop(players, continents);
    //
    //    std::cout << "--------------- TESTING REINFORCEMENT PHASE --------------" << std::endl;
    //    std::cout << "--------------- CURRENT REINFORCEMENTS --------------" << std::endl;
    //    std::cout << player1.getPlayerID() <<": " << player1.getReinforcementPool() << std::endl;
    //    std::cout << player2.getPlayerID() <<": " << player2.getReinforcementPool() << std::endl;
    //
    //    reinforcementPhase(player1, continents);
    //    reinforcementPhase(player2, continents);
    //    std::cout << "--------------- REINFORCEMENTS AFTER PHASE --------------" << std::endl;
    //    std::cout << player1.getPlayerID() <<": " << player1.getReinforcementPool() << std::endl;
    //    std::cout << player2.getPlayerID() <<": " << player2.getReinforcementPool() << std::endl;
    //
    //    std::cout << "--------------- TESTING ISSUE ORDER PHASE --------------" << std::endl;
    //    player2.setReinforcementPool(0);
    //
    //    issueOrdersPhase(players);
}
