// LoggingObserver.cpp

#include <iostream>
#include <algorithm>
#include "LoggingObserver.h"

void Subject::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void Subject::removeObserver(Observer* observer) {
    auto it = std::find(observers.begin(), observers.end(), observer);

    if (it != observers.end()) {
        observers.erase(it);
    }
}

void Subject::notifyObservers() {
    for (Observer* observer : observers) {
        observer->update();
    }
}

LogObserver::LogObserver(const std::string& logFilePath) : logFilePath(logFilePath) {}

void LogObserver::update() {
    std::ofstream logFile(logFilePath, std::ios::app);
    if (logFile.is_open()) {
        logFile << observedObject->stringToLog() << std::endl;
        logFile.close();
    } else {
        std::cout << "Error: Unable to open log file." << std::endl;
    }
}

void LogObserver::setObservedObject(ILoggable* obj) {
    observedObject = obj;
}

//std::string Command::stringToLog() const {
//    return "Command's Effect: " + effect;
//}
//
//void Command::saveEffect(const std::string& effectMessage) {
//    effect = effectMessage;
//    notifyObservers(); // Notify observers after effect is saved
//}
//
//std::string CommandProcessor::stringToLog() const {
//    return "Command: " + savedCommand;
//}
//
//void CommandProcessor::saveCommand(const std::string& command) {
//    savedCommand = command;
//    notifyObservers(); // Notify observers after command is saved
//}
//
//std::string GameEngine::stringToLog() const {
//    return "Game Engine new state: " + newState;
//}
//
//void GameEngine::transition(const std::string& newState) {
//    this->newState = newState;
//    notifyObservers(); // Notify observers after game engine state is changed
//}
