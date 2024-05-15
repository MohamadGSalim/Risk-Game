#include "TournamentDriver.h"

void tournamentGameReport(std::vector<std::string> mapFiles, std::vector<std::string> playerStrategies, int numberOfGames, int maxNumberOfTurns) {
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

    std::cout << "Enter the name of 1-5 Map Files {AnatomyMap, AsiaMap, BrazilMap, etc...}. When finished, Enter: done\n";
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
    std::cout << "Enter the max number of turns in each game (10-50)\n";
    while (check)
    {
        cin >> maxNumberOfTurns;

        if (maxNumberOfTurns > 9 && maxNumberOfTurns < 51)
        {
            check = false;
        }
        else
            std::cout << "Invalid Input. Try again.\n";
    }

    tournamentGameReport(mapFiles, playerStrategies, numberOfGames, maxNumberOfTurns);
}

// Test function for tournament mode
void testTournament(){
    tournamentMode();
}