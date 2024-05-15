#ifndef COMP345WARZONE_TOURNAMENTDRIVER_H
#define COMP345WARZONE_TOURNAMENTDRIVER_H

#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <cstdlib> // For std::rand and std::srand
#include <ctime>   // For std::time
#include <algorithm>
#include <iomanip>  // For std::setw

using namespace std;

void testTournament();
void tournamentMode();
void tournamentGameReport(std::vector<std::string> mapFiles, std::vector<std::string> playerStrategies, int numberOfGames, int maxNumberOfTurns);


#endif //COMP345WARZONE_TOURNAMENTDRIVER_H
