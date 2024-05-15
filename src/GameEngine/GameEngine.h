// Header File
#pragma once
#include <iostream>
#include <string>
#include "../Player/Player.h"
#include "GameContext.h"
#include "../LoggingObserver/LoggingObserver.h"

using namespace std;

// Private - any attributes
// Public - any methods
// Methods - class declarations/free functions

class State: public Subject, ILoggable{
public:
	/*Enumerated list GEStates (GameEngine States) for the different states of the game*/
	enum GEStates {
		GEStart=0, GEMapLoaded=1, GEMapValidated=2, GEPlayersAdded=3,
		GEAssignR=4, GEIssueOrders=5, GEExecOrders=6, GEWin=7,
	};

	GEStates *GEState;

	// Default constructor
	State();

    void changeState(int positionState);

    static int getState(const GEStates* GEState);

    int setintCurrentState(GEStates* GEState);

    int getintCurrentState() const;
	
	friend ostream& operator << (ostream& output, State &p);

private:
    int currentStateint = 0;

    string stringToLog() const override;
    string getStateString(int stateInt) const;
};

/*Some actions or commands may eventually trigger a transition to another state,
which is what controls the flow of the game.*/
class Command
{

public:
	Command()
	{ // Default constructor
		userCommand = new string("nocommand");
	}

	string assignCommand(int intState);
	
	friend ostream &operator << (ostream &output,Command &p);
	friend istream &operator >> (istream &input,Command &p);

private:
	string *userCommand; // userCommand to only be accessed within the scope of the class
};

void mainGameLoop(vector<Player *>, vector<Continent *> continents);
void reinforcementPhase(Player &player, vector<Continent *> continents);
void issueOrdersPhase(vector<Player*>);
void executeOrdersPhase(vector<Player*>);
bool checkBoolVector(vector<bool>);
bool hasWinner(std::vector<Continent *>);
int getContinentBonus(Player &player,Continent* continent);
int numDeployOrders(OrdersList);
void startupPhaseTransition();
bool startupPhase(GameContext& context,string readCommand);
void tournamentMode();
void tournamentGameReport(std::vector<std::string> mapFiles, std::vector<std::string> playerStrategies, int numberOfGames, int maxNumberOfTurns);
void mainGameloop(vector<Player *>, vector<Continent *>);