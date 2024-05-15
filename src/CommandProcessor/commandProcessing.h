#ifndef COMP345WARZONE_COMMANDPROCESSING_H
#define COMP345WARZONE_COMMANDPROCESSING_H
#endif //COMP345WARZONE_COMMANDPROCESSING_H

#include <string> //Include String Library
#include "../GameEngine/GameEngine.h"
#include <vector>
#include "../LoggingObserver/LoggingObserver.h"
using namespace std;

namespace commandProcessing{
    class Command : public Subject, ILoggable{
    public:
        string command;
        string effect;

        Command();
        int saveEffect(bool valid, string effect, int initialState);

        Command& operator=(const Command& other);

    protected:
        string stringToLog() const override;
    };}

class CommandProcessor : public Subject, ILoggable{
public:
    vector<commandProcessing::Command> commands;
    int getCommand(int intState);
    int validate(string &userCommand, int initialState);

    CommandProcessor();

protected:
    virtual string readCommand();
    int saveCommand(string &userCommand, vector<commandProcessing::Command>& commands);
    string stringToLog() const override;
};

class FileCommandProcessorAdaptor : public CommandProcessor{
public:
    void getCommand(string fileName, int initialState);
private:
    virtual void readCommand(string fileName);
};

class FileLineReader : public FileCommandProcessorAdaptor{
public:
    static void readLineFromFile(const string& fileName, vector<commandProcessing::Command> &commands);
};

