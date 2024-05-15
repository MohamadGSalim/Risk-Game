#include "commandProcessing.h"
#include "GameEngine.h"
#include <iostream>
#include <utility>
#include <vector>
#include <fstream> //Include file stream library

//Enables different operating systems to read files
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
using namespace std;
using namespace commandProcessing;

//CONSTRUCTORS
CommandProcessor::CommandProcessor(){
    this->addObserver(&logger);
    logger.setObservedObject(this);
}

commandProcessing::Command::Command(){
    this->addObserver(&logger);
    logger.setObservedObject(this);
}

string CommandProcessor::stringToLog() const{
    auto lastCommand = commands.back(); // Get the last command
    return "Command: " + lastCommand.command;
}

string commandProcessing::Command::stringToLog() const{
    return "Command's Effect: " + this->effect;
}

//Get command function that takes initial state as a parameter and runs the command processing functions
int CommandProcessor::getCommand(int initialState) {
    string userCommand;
    userCommand = readCommand(); //Assigns the return of readCommand to userCommand
    saveCommand(userCommand, commands);
    int newState = validate(userCommand, initialState);
    //returns the new state for the state object
    return newState;
}

//Reads an input from the user and returns it
string CommandProcessor::readCommand() {
    string userCommand;
    cout << "\nEnter a command: ";
    cin >> userCommand;
    return userCommand;
}

//Adator for readCommand - takes the file name as a parameter
void FileCommandProcessorAdaptor::readCommand(string fileName) {
    //Erases the -file from the string
    fileName.erase(0,6);
    cout << fileName;
    //Call to readline function
    FileLineReader::readLineFromFile(fileName, commands);
}

commandProcessing::Command& commandProcessing::Command::operator=(const Command& other) {
    if (this != &other) {
        // Copy the values from 'other' to 'this'
        this->command = other.command;
        this->effect = other.effect;
    }
    return *this;
}

//Adaptor for get command
void FileCommandProcessorAdaptor::getCommand(std::string fileName, int initialState) {
    readCommand(std::move(fileName));
    State fileState;
    int newState = 0;
    //Whilst i is less than the size of the vector, do getCommand
    for (int i = 0; i < commands.size(); i++){
        cout << "\n\n\nTHIS IS FILE ELEMENT  " << i;
        commandProcessing::Command fileCommand;
        fileCommand = commands.at(i);
        string userCommand = fileCommand.command; //Assigns commands at position i to userCommand
        newState = validate(userCommand, initialState);
        //Changes the state of Game engine and sets the global varaible Set int current state to the current state
        fileState.changeState(newState);
        fileState.setintCurrentState(fileState.GEState);
        cout << fileState;
        //Sets initalState to the new state - the next iteration will pick up from the changed state
        initialState = newState;
    }
}

//Read from the file and append it to the vector
void FileLineReader::readLineFromFile(const string& fileName, vector<commandProcessing::Command>& commands){
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    string test(buff);
    //string test = current_working_dir::current_path();
    test.append("/../CommandProcessor/");
    test.append(fileName);
    ifstream file (test);

    string line;
    std::string iny;
    while(getline(file,line)) {
        commandProcessing::Command newCommand;
        newCommand.command = line;
        cout << "\n" << line;
        commands.push_back(newCommand);
    }

    cout << "\nThis is the front of the file vector: " << commands.front().command;
    cout << "T\nhis is the back of the file vector: " << commands.back().command;
}

//Save the command by pushing it to the back of the vector
int CommandProcessor::saveCommand(string &userCommand, vector<commandProcessing::Command>& commands) {
    commandProcessing::Command newCommand;
    newCommand.command = userCommand;
    commands.push_back(newCommand);

    cout << "\nThis is the front of the vector: " << commands.front().command;
    cout << "\nThis is the back of the vector: " << commands.back().command;

    this->notifyObservers();
    return 0;
}

//Validates the command based on the current state
int CommandProcessor::validate(string &userCommand, int intState){
    bool valid = false;

    string effect = "noEffect";

    switch (intState){
        case 0:
            if (userCommand.find("loadmap") != std::string::npos){
                effect = "transition";
                valid = true;
            }
            break;

        case 1:
            if (userCommand.find("loadmap") != std::string::npos|| userCommand.find("validatemap") != std::string::npos){
                if (userCommand.find("validate") != std::string::npos){
                    effect = "transition";
                }
                valid = true;
            }
            break;

        case 2:
            if (userCommand.find("addplayer") != std::string::npos){
                effect = "transition";
                valid = true;
            }
            break;

        case 3:
            //commands.at(currentCommandIndex)
            if (userCommand.find("addplayer") != std::string::npos || userCommand == "gamestart"){
                if (userCommand == "gamestart"){
                    effect = "transition";
                }
                valid = true;
            }
            break;

        case 7:
            if (userCommand == "replay" || userCommand == "quit"){
                if (userCommand == "replay"){
                    effect = "restart";
                } else {
                    effect = "end";
                }
                valid = true;
            }
            break;

        case 8:
            if (userCommand.find("tournament") != std::string::npos){
                effect = "transition";
                valid = true;
            }
            break;

        default:
            valid = false;
            break;
    }

    if (intState == 4 || intState == 5 || intState == 6){
        valid = true;
        effect = "pass";
    }

    if (valid){
        cout << "\n\nThe command was valid.";
    } else {
        cout << "\n\nThe command was invalid.";
    }

    commandProcessing::Command commandEffect;
    int currentState = commandEffect.saveEffect(valid, effect, intState);
    cout << "\nCommand effect: " << commandEffect.effect;
    return currentState;
}

//Saves the effect and causes the state transition (if needed)
int commandProcessing::Command::saveEffect(bool valid, string newEffect, int initialState) {
    if (valid && newEffect != "noEffect"){
        State tempObj;
        int newState;
        //Replay goes back to state 0
        if (newEffect == "restart"){
            newState = 0;
            tempObj.changeState(newState);
        } else if (newEffect == "end"){
            effect = newEffect;
            newState = initialState;
        } else {
            newState = initialState+=1;
            tempObj.changeState(newState);
            tempObj.setintCurrentState(tempObj.GEState);
            effect = std::move(newEffect);
        }
        this->notifyObservers();
        return newState;
    } else {
        effect = "no effect";
        this->notifyObservers();
        return initialState;
    }
}