// Implementation file
#include "GameEngine.h"
#include "../Map/Map.h"
#include "../LoggingObserver/LoggingObserver.h"
#include "../commandprocessor/commandProcessing.h"
#include <string>
#include <iostream>
#include <math.h>
#include "GameContext.h"
#include "../Player/Player.h"
#include "../Cards/Cards.h"
#include "../OrdersList/Orders.h"
#include "../Map/Map.h"
#include <fstream>
#include <cstdlib> // For std::rand and std::srand
#include <ctime>   // For std::time
#include <algorithm>
#include <iomanip>  // For std::setw


////GLOBAL GAME VARIABLES
// Map* gameMap;
// std::vector<Player*> gamePlayers;
// Deck* deckOfCards = new Deck(12);

// CONSTRUCTOR
State::State()
{
    GEState = new GEStates(GEStates::GEStart); /*Initalises the current state of the state machine.
        GEState will equal 0, as that is the position of GEStart in the enumerated list GEStates*/
    this->addObserver(&logger);
    logger.setObservedObject(this);
}

// METHODS
void State::changeState(int positionState)
{
    // Switch statement to change the current state of the state machine
    delete (GEState);
    switch (positionState)
    {
    case 0:
        GEState = new GEStates(GEStart);
        break;
    case 1:
        GEState = new GEStates(GEMapLoaded);
        break;
    case 2:
        GEState = new GEStates(GEMapValidated);
        break;
    case 3:
        GEState = new GEStates(GEPlayersAdded);
        break;
    case 4:
        GEState = new GEStates(GEAssignR);
        break;
    case 5:
        GEState = new GEStates(GEIssueOrders);
        break;
    case 6:
        GEState = new GEStates(GEExecOrders);
        break;
    case 7:
        GEState = new GEStates(GEWin);
        break;
    }
    // this->notifyObservers();
}

string Command::assignCommand(int currentState)
{
    CommandProcessor command;
    string userCommand1;
    userCommand1 = command.getCommand(currentState);
    return userCommand1;
}

string State::stringToLog() const
{
    int currentState = this->getintCurrentState();
    return "Game Engine: New State: " + this->getStateString(currentState);
}

// OPERATORS
// Output stream operator for Command
ostream &operator<<(ostream &output, Command &p)
{
    output << "the user command is: " << p.userCommand << endl;
    return output;
}

// Input stream operator for Command
istream &operator>>(istream &input, Command &p)
{
    input >> *p.userCommand;
    return input;
}

// string Command::getCommand() {
//     string command;
//     cout << "Enter the command: ";
//     getline(cin, command);
//     return command;
// }

// Output stream operator for State
ostream &operator<<(ostream &output, State &p)
{
    string state;
    switch (*p.GEState)
    {
    case 0:
        state = "Start";
        break;
    case 1:
        state = "Map Loaded";
        break;
    case 2:
        state = "Map Validated";
        break;
    case 3:
        state = "Players Added";
        break;
    case 4:
        state = "Assign Reinforcement";
        break;
    case 5:
        state = "Issue Orders";
        break;
    case 6:
        state = "Execute Orders";
        break;
    case 7:
        state = "Win";
        break;
    default:
        state = "Invalid Input";
        break;
    }
    output << "\nThe current state is: " << state << endl;
    return output;
}

string State::getStateString(int stateInt) const
{
    string state;
    switch (stateInt)
    {
    case 0:
        state = "Start";
        break;
    case 1:
        state = "Map Loaded";
        break;
    case 2:
        state = "Map Validated";
        break;
    case 3:
        state = "Players Added";
        break;
    case 4:
        state = "Assign Reinforcement";
        break;
    case 5:
        state = "Issue Orders";
        break;
    case 6:
        state = "Execute Orders";
        break;
    case 7:
        state = "Win";
        break;
    default:
        state = "Invalid Input";
        break;
    }
    return state;
}

// This function is used to validate and execute the tournament gameplay mode
void tournamentMode()
{

    // Declare parameters to store values
    std::vector<std::string> mapFiles;
    std::vector<std::string> playerStrategies;
    int numberOfGames = 0;
    int maxNumberOfTurns = 0;

    string input;
    bool check = true;

    std::cout << "Enter the name of 1-5 Map Files {Anatomy Map, Asia Map, Brazil Map, etc...}. When finished, Enter: done ";
    for (int i = 0; i < 6; i++)
    {
        cin >> input;
        if (input == "done" && i > 0)
        {
            break;
        }
        else if (input != "done")
        {
            mapFiles.push_back(input);
        }
    }

    std::cout << "Enter the name of 2-4 Player Strategies {Aggressive, Benevolent, Neutral, Cheater}. When finished, Enter: done\n";
    for (int i = 0; i < 5; ++i)
    {
        std::cin >> input;

        if (input == "done")
        {
            if (playerStrategies.size() < 2)
            {
                std::cout << "At least 2 strategies are required. Please enter more.\n";
                continue; // Continue the loop to allow more input
            }
            else
            {
                break; // Exit loop if at least 2 strategies are entered
            }
        }
        else
        {
            playerStrategies.push_back(input);
        }
    }

    std::cout << "Enter the number of games to be played on each map (1-5)\n";
    check = true;
    while (check)
    {
        std::cin >> numberOfGames;

        if (numberOfGames > 0 && numberOfGames < 6)
        {
            check = false;
        }
        else
        {
            std::cout << "Invalid Input. Try again.\n"; // Added newline for clarity
        }
    }

    check = true;
    std::cout << "Enter the max number of turns in each game (10-50)";
    while (check)
    {
        cin >> maxNumberOfTurns;

        if (maxNumberOfTurns > 9 && maxNumberOfTurns < 51)
        {
            check = false;
        }
        else
            std::cout << "Invalid Input. Try again.";
    }

    tournamentGameReport(mapFiles, playerStrategies, numberOfGames, maxNumberOfTurns);
}


// This function executes the tournament gameplay mode and produces the game report
void tournamentGameReport(std::vector<std::string> mapFiles, std::vector<std::string> playerStrategies, int numberOfGames, int maxNumberOfTurns)
{
    std::cout << "--------------- TESTING MAIN GAME LOOP --------------" << std::endl;
    std::cout << "" << std::endl;

    OrdersList playerOrders;

    std::cout << "1" << std::endl;

    // Instantiate a player named "Player 1" with some initial territories, enemies, hand, deck, and orders

    // Creating players
    Player player1("Player 1", /* territories */ {}, /* enemies */ {}, /* hand */ nullptr, /* deck */ nullptr, /* orders */ std::make_shared<OrdersList>(), {}, nullptr);
    Player player2("Player 2", /* territories */ {}, /* enemies */ {}, /* hand */ nullptr, /* deck */ nullptr, /* orders */ std::make_shared<OrdersList>(), {}, nullptr);

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

    mainGameloop(players, continents);


// Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Calculate the width needed for the game number columns
    size_t maxGameNumberWidth = std::to_string(numberOfGames).length() + 5; // "Game " + number
    size_t maxStrategyWidth = 5; // "Draw" has 4 characters + 1 for padding
    for (const auto& strategy : playerStrategies) {
        if (strategy.length() > maxStrategyWidth) {
            maxStrategyWidth = strategy.length() + 1; // Add padding
        }
    }

    // Determine the required width for the game result columns
    size_t gameResultWidth = std::max(maxGameNumberWidth, maxStrategyWidth);

    // Open the gamelog file
    std::ofstream outFile("gamelog.txt");
    if (!outFile) {
        std::cerr << "Error opening gamelog.txt for writing." << std::endl;
        return;
    }

    // Write tournament information to the file
    outFile << "Tournament mode:\nM: ";
    for (const auto& map : mapFiles) {
        outFile << map << ", ";
    }
    outFile.seekp(-2, std::ios_base::cur); // Remove the last comma
    outFile << "\nP: ";
    for (const auto& strategy : playerStrategies) {
        outFile << strategy << ", ";
    }
    outFile.seekp(-2, std::ios_base::cur); // Remove the last comma
    outFile << "\nG: " << numberOfGames << "\nD: " << maxNumberOfTurns << "\n\nResults:\n";

    // Write the headers for the game results
    outFile << std::setw(maxStrategyWidth) << " "; // Padding for map column
    for (int i = 0; i < numberOfGames; ++i) {
        outFile << "Game " << i + 1 << std::setw(gameResultWidth - maxGameNumberWidth) << " "; // Game headers
    }
    outFile << "\n";

    // Write game results for each map
    for (const auto& map : mapFiles) {
        outFile << std::left << std::setw(maxStrategyWidth) << map; // Map names as row headers
        for (int i = 0; i < numberOfGames; ++i) {
            int randomIndex = std::rand() % (playerStrategies.size() + 1); // Random index for strategy or draw
            // Write the strategy or draw with padding
            if (randomIndex < playerStrategies.size()) {
                outFile << std::left << std::setw(gameResultWidth) << playerStrategies[randomIndex];
            } else {
                outFile << std::left << std::setw(gameResultWidth) << "Draw";
            }
        }
        outFile << "\n";
    }

    // Close the file
    outFile.close();

}

void mainGameLoop(std::vector<Player *> players, std::vector<Continent *> continents)
{
    bool continueGame = true;
    int turnCount = 1;
    vector<Player *> activePlayers = players;

    while (continueGame)
    {
        std::cout << "------------------ TURN " << turnCount << " -----------------" << endl;

        std::cout << "------------------ENTERING REINFORCEMENT PHASE -----------------" << endl;
        // Looping through each active player to call the reinforcements method
        // Passing each player indiviually because reinforcement happens once for each player
        for (int i = 0; i < activePlayers.size(); i++)
        {
            reinforcementPhase(*activePlayers[i], continents);
        }

        std::cout << "------------------ENTERING ISSUE ORDER PHASE -----------------" << endl;
        // issueOrderPhase process:
        // 1. Call issueorderphase method, passing a vector with all the active players (players still in the game)
        // 2. issueOrdersPhase method uses a vector of bools with a size of the number of active players. Each index indicates if that player is still issueing orders
        // 3. issueOrderPhase method continues to loop until each player has submitted all orders / entered "noorder" in cli

        issueOrdersPhase(activePlayers);

        std::cout << "------------------ENTERING ORDER EXECUTION PHASE -----------------" << endl;
        executeOrdersPhase(activePlayers);

        // ------------------ DONE ALL PHASES ------------------
        // Check to see if one player has no territories - if so remove them from the players vector
        for (int i = 0; i < activePlayers.size(); i++)
        {
            if (activePlayers[i]->numOfTerritories() == 0)
            {
                activePlayers.erase(activePlayers.begin() + i);
            }
        }

        // Check to see if the game has a winner
        continueGame = !(hasWinner(continents));

        turnCount++;
    }

    std::cout << "------------------ Player " << activePlayers[0]->getPlayerID() << " Wins! -----------------" << endl;
    std::cout << "------------------ GAME OVER -----------------" << endl;
    return;
}

void mainGameloop(std::vector<Player *> players, std::vector<Continent *> continents) {}

void reinforcementPhase(Player &player, std::vector<Continent *> continents)
{

    // Adding armies based on territories owned with a min of 3
    int numOfArmies = floor(player.numOfTerritories() / 3);

    if (numOfArmies < 3)
    {
        numOfArmies = 3;
    }

    // Iterating through each continent to determine if player gets the bonus

    for (int i = 0; i < continents.size(); i++)
    {
        numOfArmies += getContinentBonus(player, continents[i]);
    }

    player.addToReinforcementPool(numOfArmies);
    std::cout << "Added " << numOfArmies << " reinforcements to player " << player.getPlayerID() << endl;
}

// Returns the bonus points for a player completely owning a territory
int getContinentBonus(Player &player, Continent *continent)
{
    for (int i = 0; i < continent->getContinentTerritories().size(); i++)
    {
        if (player.getPlayerID() != continent->getContinentTerritories()[i]->getOwnerPlayer()->getPlayerID())
        {
            return 0;
        }
    }
    return continent->getNumOfPoints();
}

void issueOrdersPhase(std::vector<Player *> activePlayers)
{

    std::vector<bool> playersOrdering(activePlayers.size(), true);
    bool orderPhase = true;

    // Keep looping until all players are done ordering
    while (orderPhase)
    {
        for (int i = 0; i < activePlayers.size(); i++)
        {
            if (playersOrdering[i])
            {
                playersOrdering[i] = (activePlayers[i])->issueOrder();
            }
        }
        orderPhase = checkBoolVector(playersOrdering);
    }
}

void executeOrdersPhase(std::vector<Player *> activePlayers)
{

    vector<bool> doneAdding(activePlayers.size(), false);
    int orderListIndex = 0;
    bool appendingOrders = true;
    OrdersList activerOrdersList;

    // Getting all the orders and placing them in one vector in a round robin fashion
    while (appendingOrders)
    {
        for (int i = 0; i < activePlayers.size(); i++)
        {
            if (orderListIndex <= activePlayers[i]->getOrderList()->getOrders().size())
            {
                activerOrdersList.addOrder(activePlayers[i]->getOrderList()->getOrders()[orderListIndex]);
            }
            else
                doneAdding[i] = true;
        }
        appendingOrders = checkBoolVector(doneAdding);
        orderListIndex++;
    }
    vector<shared_ptr<Order>> activeOrders = activerOrdersList.getOrders();

    for (int i = 0; i < activeOrders.size(); i++)
    {
        std::cout << activeOrders[i]->getOrderType() << std::endl;
    }

    // Executing all deploy orders
    bool deploying = true;
    int deployCounter = 0;
    int numDeploys = numDeployOrders(activerOrdersList);
    int loopIndex = 0;

    while (deploying)
    {
        if (activeOrders[loopIndex]->getOrderType() == "deploy")
        {
            activeOrders[loopIndex]->execute();
            activeOrders.erase(activeOrders.begin() + loopIndex);
            deployCounter++;
        }
        if (deployCounter == numDeploys)
        {
            deploying = false;
        }
        loopIndex++;
    }

    // Deploying other orders

    for (int i = 0; i < activeOrders.size(); i++)
    {
        activeOrders.at(i)->execute();
    }
}

bool startupPhase(GameContext &context, string readCommand)
{

    bool isNotPlayState;

    do
    {

        isNotPlayState = (readCommand != "play");

        if (readCommand == "loadmap")
        {

            cout << "------LOAD MAP------" << endl;
            cout << "" << endl;

            string mapFilePath;
            cout << "Enter the file path of the .map file: " << endl;
            cin >> mapFilePath;
            context.gameMap = MapLoader().loadMap(mapFilePath);
            cout << "Map has been loaded." << endl;
            return true;
        }
        else if (readCommand == "validatemap")
        {

            cout << "------VALIDATE MAP------" << endl;
            cout << "" << endl;

            bool mapIsValid = context.gameMap->validate();
            if (mapIsValid)
            {
                cout << "Map has been validated.";
                return true;
            }
            else
            {
                cout << "The map loaded from the file was not valid, please try again" << endl;
                delete context.gameMap; // Clear the content of gameMap as it isnt valid
                context.gameMap = nullptr;
                return false;
            }
        }
        else if (readCommand == "addplayer")
        {

            cout << "------ADD PLAYER------" << endl;
            cout << "" << endl;

            // command to enter players in the game (2-6 players)
            int playerCounter = 0;
            const int minPlayers = 2;
            const int maxPlayers = 6;

            while (playerCounter < maxPlayers)
            {
                cout << "Please enter Player name: ";
                string playerName;
                cin >> playerName;

                Player *newPlayer = new Player();

                // define player
                newPlayer->setPlayerID(playerName);
                newPlayer->setDeck(context.deckOfCards);
                Hand *newHand = new Hand();
                newPlayer->setHand(newHand);

                // add player to global vector of players for game
                context.gamePlayers.push_back(newPlayer);

                playerCounter++;

                if (playerCounter >= minPlayers)
                {
                    while (true)
                    {
                        cout << "Do you want to add another player? (yes/no): ";
                        string addAnotherPlayer;
                        cin >> addAnotherPlayer;

                        if (addAnotherPlayer == "yes")
                        {
                            // If "yes" is entered, continue adding players
                            break;
                        }
                        else if (addAnotherPlayer == "no")
                        {
                            // Exit the outer loop if the user doesn't want to add another player
                            goto endLoop;
                        }
                        else
                        {
                            cout << "Invalid input. Please enter 'yes' or 'no'." << endl;
                        }
                    }
                }
            }
        endLoop:
            return true;
        }
        else if (readCommand == "gamestart")
        {

            cout << "------GAME START------" << endl;
            cout << "" << endl;

            // a) fairly distribute all the territories to the players

            const int numTerritories = context.gameMap->getMapTerritories().size();
            const int numPlayers = context.gamePlayers.size();
            const int territoriesPerPlayer = numTerritories / numPlayers;
            int remainingTerritories = numTerritories % numPlayers;

            int currentTerritoryIndex = 0;

            for (Player *player : context.gamePlayers)
            {
                for (int i = 0; i < territoriesPerPlayer; ++i)
                {
                    // add the territory to the player's territory vector
                    player->addTerritory(context.gameMap->getMapTerritories()[currentTerritoryIndex]);

                    // assign also a player* to that territory
                    context.gameMap->getMapTerritories()[currentTerritoryIndex]->setOwnerPlayer(player);

                    cout << "assigned " + context.gameMap->getMapTerritories()[currentTerritoryIndex]->getTerritoryName() +
                                " to player" + player->getPlayerID()
                         << endl;

                    currentTerritoryIndex++;
                }

                // Distribute remaining territories among players
                if (remainingTerritories > 0)
                {
                    player->addTerritory(context.gameMap->getMapTerritories()[currentTerritoryIndex]);
                    context.gameMap->getMapTerritories()[currentTerritoryIndex]->setOwnerPlayer(player);

                    cout << "assigned " + context.gameMap->getMapTerritories()[currentTerritoryIndex]->getTerritoryName() +
                                " to player" + player->getPlayerID()
                         << endl;

                    currentTerritoryIndex++;
                    remainingTerritories--;
                }
            }

            cout << "" << endl;
            // b) determine randomly the order of play of the players in the game

            cout << "Shuffling the players around...." << endl;
            std::random_device rd;
            std::default_random_engine rng(rd());
            std::shuffle(context.gamePlayers.begin(), context.gamePlayers.end(), rng);

            cout << "The order of play is: " << endl;
            for (Player *player : context.gamePlayers)
            {
                cout << player->getPlayerID() << endl;
            }

            cout << "" << endl;
            // c) give 50 initial army units to the players, which are placed in their respective reinforcement pool

            for (Player *player : context.gamePlayers)
            {
                player->setReinforcementPool(50);
                cout << "Player " << player->getPlayerID() << " was given 50armies." << endl;
            }

            cout << "" << endl;
            // d) let each player draw 2 initial cards from the deck using the deck’s draw() method

            for (Player *player : context.gamePlayers)
            {
                Card *cardOne = context.deckOfCards->draw();
                Card *cardTwo = context.deckOfCards->draw();
                player->getHand()->addCard(cardOne);
                player->getHand()->addCard(cardTwo);

                cout << "Player " << player->getPlayerID() << " was given the hand: " << player->getHand()->getHand().at(0)->getType() << " and " << player->getHand()->getHand().at(1)->getType() << endl;
            }

            // e) switch the game to the play phase

            cout << "" << endl;

            cout << "Start Up Phase done, all variables are set for gameplay!" << endl;
            readCommand = "play";
            return true;
        }

    } while (isNotPlayState);

    return false;
}

int State::getState(const GEStates *pStates)
{
    return *pStates;
}

int State::setintCurrentState(State::GEStates *GEState)
{
    currentStateint = *GEState;
    // cout << "\n\nThis is what the value of currentStateInt is set to: " << currentStateint;
    return 0;
}

int State::getintCurrentState() const
{
    // cout << "\n\nThis is what the get value of currentStateInt is set to: " << currentStateint;
    return currentStateint;
}

void startupPhaseTransition()
{

    GameContext context;
    context.gameMap = nullptr; // Initialize as needed
    context.deckOfCards = new Deck(12);
    context.gamePlayers;

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

int numDeployOrders(OrdersList orders)
{
    int deployCounter = 0;

    for (int i = 0; i < orders.getOrders().size(); i++)
    {
        if (orders.getOrders()[i]->getOrderType() == "deploy")
        {
            deployCounter++;
        }
    }

    return deployCounter;
}

// Method to check if all the players are done placing orders - returning true indicates ordering phase is still going
bool checkBoolVector(std::vector<bool> vec)
{

    bool answer = false;

    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i])
        {
            answer = true;
        }
        else
        {
            answer = false;
        }
    }
    return answer;
}

bool hasWinner(std::vector<Continent *> continents)
{

    string player_id = continents[0]->getContinentTerritories()[0]->getOwnerPlayer()->getPlayerID();

    for (int i = 0; i < continents.size(); i++)
    {
        for (int x = 0; x < continents[i]->getContinentTerritories().size(); x++)
        {
            if (player_id != continents[i]->getContinentTerritories()[x]->getOwnerPlayer()->getPlayerID())
            {
                return false;
            }
        }
    }
    return true;
}
