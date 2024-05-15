// LoggingObserver.h

#ifndef COMP345WARZONE_LOGGINGOBSERVER_H
#define COMP345WARZONE_LOGGINGOBSERVER_H

#include <vector>
#include <string>
#include <fstream>

// Observer class represents the observer in the Observer pattern.
class Observer {
public:
    virtual void update() = 0; // Pure virtual function to be implemented by concrete observers.
};

// Subject class represents the subject in the Observer pattern.
class Subject {
public:
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notifyObservers();

private:
    std::vector<Observer*> observers;
};

// ILoggable class serves as an interface for objects that can be logged.
class ILoggable {
public:
    virtual std::string stringToLog() const = 0; // Pure virtual function to be implemented by loggable objects.
};

// LogObserver class observes changes in ILoggable objects and logs them.
class LogObserver : public Observer {
public:
    LogObserver(const std::string& logFilePath);
    void update() override;
    void setObservedObject(ILoggable* obj);

private:
    std::string logFilePath;
    ILoggable* observedObject{};
};
extern LogObserver logger;

//// Command class represents a command and is a Subject and ILoggable.
//class Command : public Subject, public ILoggable {
//public:
//    std::string stringToLog() const override;
//    void saveEffect(const std::string& effect);
//
//private:
//    std::string effect;
//};
//
//// CommandProcessor class represents a command processor and is a Subject and ILoggable.
//class CommandProcessor : public Subject, public ILoggable {
//public:
//    std::string stringToLog() const override;
//    void saveCommand(const std::string& command);
//
//private:
//    std::string savedCommand;
//};
//
//// GameEngine class represents a game engine and is a Subject and ILoggable.
//class GameEngine : public Subject, public ILoggable {
//public:
//    std::string stringToLog() const override;
//    void transition(const std::string& newState);
//
//private:
//    std::string newState;
//};

#endif //COMP345WARZONE_LOGGINGOBSERVER_H
