
#include "commandProcessing.h"
#include "GameEngine.h"
#include <iostream> //Include Input/Output
#include <string> //Include String Library
#include <algorithm>

using namespace std;

void testCommandProcessor(){

    std::cout << "-----------------TESTING COMMAND PROCESSOR-----------------" << std::endl;
    char userChoiceChar[100] = {0}; //used to get string with space
    bool exit = false;

    State newStateObj;
    while (!exit){
        cout << "\n\nEnter -console or -file <filename> or -exit: ";
        cin.getline(userChoiceChar,40);
        string userChoice(userChoiceChar); //converts char array to string

        //Sets the initial state to 0
        newStateObj.changeState(0);
        newStateObj.setintCurrentState(newStateObj.GEState);

        //CONSOLE
        if (userChoice == "-console"){
            string userContinue = "Y";
            CommandProcessor tempCommand;
            commandProcessing::Command newCommand;

            //While the user wants to enter more commands
            while (userContinue == "Y"){
                cout << newStateObj;
                int initialState = newStateObj.getintCurrentState(); //InitialState set to the integer value of the current state
                int newState = tempCommand.getCommand(initialState); //Call to getCommand()
                newStateObj.changeState(newState);
                newStateObj.setintCurrentState(newStateObj.GEState);
                cout << newStateObj;
                cout << "\nWould you like to enter another command?";
                cin >> userContinue;
                //Converts the string to upper
                transform(userContinue.begin(), userContinue.end(), userContinue.begin(), ::toupper);
            };

            //FILE
        } else if (userChoice.find("-file") != std::string::npos){//Checks if userChoice contains -file
            FileCommandProcessorAdaptor tempFileCommand;
            int initialState = newStateObj.getintCurrentState();
            tempFileCommand.getCommand(userChoice, initialState);//Call to get command

            //EXIT
        } else if (userChoice == "-exit"){
            exit = true;

            //INVALID
        } else {
            cout << "\nInvalid command. Please try again.";
        }
    }
    std::cout << "---------------end: TESTING COMMAND-PROCESSOR--------------" << endl;
}
