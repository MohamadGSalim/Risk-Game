#ifndef ORDERS_H
#define ORDERS_H

#include "Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"

#include <vector>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <string>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../LoggingObserver/LoggingObserver.h"
using namespace std;


class Player;
class Territory;

// ........... 0. ORDER ..............
class Order : public Subject, public ILoggable
{
protected:
  shared_ptr<string> orderType;
  Player *issuingPlayer;
  shared_ptr<string> effect;


public:
  // DEFAULT CONSTRUCTOR to initialize any data members or set up the object.
  Order();

  // PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object.
  Order(const std::string &type, const std::string &effect); // Overloaded constructor with orderType parameter

  // PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object.
  Order(const std::string &orderType, Player *issuingPlayer, const std::string &effect );

  // COPY CONSTRUCTOR to initialize an object from another object.
  Order(const Order &orderObject);

  // ASSIGNMENT OPERATOR to assign an object to another object.
  virtual Order &operator=(const Order &orderObject);

  // Virtual print function to be overridden by derived classes
  virtual void print(ostream &output) const;

  // STREAM INSERTION OPERATOR to print the contents of an object.
  friend ostream &operator<<(ostream &output, const Order &orderObject);

  // VALIDATE
  virtual bool validate() = 0;

  // EXECUTE
  virtual void execute() = 0;

  // STRING-TO-LOG
  string stringToLog() const override;

  // GETTERS
  string getOrderType();
  Player *getIssuingPlayer() const;
  string getEffect();

  // SETTERS
  void setOrderType(const string &orderType);
  void setIssuingPlayer(Player *issuingPlayer); // Takes a pointer to Player
  void setEffect(const string &effect);

};


// ........... 0. ORDER ..............

// ........... 1. DEPLOY ..............
class Deploy : public Order
{
private:
  shared_ptr<int> numArmies;
  shared_ptr<Territory> targetTerritory;

public:
  // DEFAULT CONSTRUCTOR to initialize any data members or set up the object.
  Deploy();

  // PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object.
  Deploy(Player *issuingPlayer, int numArmies, const Territory &targetTerritory);

  // COPY CONSTRUCTOR to initialize an object from another object.
  Deploy(const Deploy &deployObject);

  // ASSIGNMENT OPERATOR to assign an object to another object.
  Deploy &operator=(const Deploy &deployObject);

  // Virtual print function to be overridden by derived classes
  void print(ostream &output) const override;

  // STREAM INSERTION OPERATOR to print the contents of an object.
  //  friend ostream &operator<<(ostream &output, const Deploy &deployObject);

  // VALIDATE
  bool validate() override;

  // EXECUTE
  void execute() override;

  // GETTERS
  int getNumArmies();
  Territory getTargetTerritory();

  // SETTERS
  void setNumArmies(const int &numArmies);
  void setTargetTerritory(const Territory &targetTerritory);
  void setIssuingPlayer(const Player &issuingPlayer);
};
// ........... 1. DEPLOY ..............

// ........... 2. ADVANCE ..............
class Advance : public Order
{
private:
  shared_ptr<int> numArmies;
  shared_ptr<Territory> sourceTerritory;
  shared_ptr<Territory> targetTerritory;

public:
  // DEFAULT CONSTRUCTOR to initialize any data members or set up the object.
  Advance();

  // PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object.
  Advance(Player *issuingPlayer, const int numArmies, const Territory &sourceTerritory, const Territory &targetTerritory);

  // COPY CONSTRUCTOR to initialize an object from another object.
  Advance(const Advance &advanceObject);

  // ASSIGNMENT OPERATOR to assign an object to another object.
  Advance &operator=(const Advance &advanceObject);

  // Virtual print function to be overridden by derived classes
  void print(ostream &output) const override;

  // STREAM INSERTION OPERATOR to print the contents of an object.
  //  friend ostream &operator<<(ostream &output, const Advance &advanceObject);

  // VALIDATE
  bool validate() override;

  // EXECUTE
  void execute() override;

  // GETTERS
  int getNumArmies();
  Territory getSourceTerritory();
  Territory getTargetTerritory();

  // SETTERS
  void setNumArmies(const int &numArmies);
  void setSourceTerritory(const Territory &sourceTerritory);
  void setTargetTerritory(const Territory &targetTerritory);
};
// ........... 2. ADVANCE ..............

// ........... 3. BOMB ..............
class Bomb : public Order
{
private:
  // string *targetTerritory;
  shared_ptr<Territory> targetTerritory;

public:
  // DEFAULT CONSTRUCTOR to initialize any data members or set up the object.
  Bomb();

  // PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object.
  Bomb(Player *issuingPlayer, const Territory &targetTerritory);

  // COPY CONSTRUCTOR to initialize an object from another object.
  Bomb(const Bomb &bombObject);

  // ASSIGNMENT OPERATOR to assign an object to another object.
  Bomb &operator=(const Bomb &bombObject);

  // Virtual print function to be overridden by derived classes
  void print(ostream &output) const override;

  // STREAM INSERTION OPERATOR to print the contents of an object.
  //  friend ostream &operator<<(ostream &output, const Bomb &bombObject);

  // VALIDATE
  bool validate() override;

  // EXECUTE
  void execute() override;

  // GETTERS
  Territory getTargetTerritory();

  // SETTERS
  void setTargetTerritory(const Territory &targetTerritory);
};
// ........... 3. BOMB ..............

// ........... 4. BLOCKADE ..............
class Blockade : public Order
{
private:
  // string *targetTerritory;
  shared_ptr<Territory> targetTerritory;

public:
  // DEFAULT CONSTRUCTOR to initialize any data members or set up the object.
  Blockade();

  // PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object.
  Blockade(Player *issuingPlayer, const Territory &targetTerritory);

  // COPY CONSTRUCTOR to initialize an object from another object.
  Blockade(const Blockade &blockadeObject);

  // ASSIGNMENT OPERATOR to assign an object to another object.
  Blockade &operator=(const Blockade &blockadeObject);

  // Virtual print function to be overridden by derived classes
  void print(ostream &output) const override;

  // STREAM INSERTION OPERATOR to print the contents of an object.
  //  friend ostream &operator<<(ostream &output, const Blockade &blockadeObject);

  // VALIDATE
  bool validate() override;

  // EXECUTE
  void execute() override;

  // GETTERS
  Territory getTargetTerritory();

  // SETTERS
  void setTargetTerritory(const Territory &targetTerritory);
};
// ........... 4. BLOCKADE ..............

// ........... 5. AIRLIFT ..............
class Airlift : public Order
{
private:
  shared_ptr<int> numArmies;
  shared_ptr<Territory> sourceTerritory;
  shared_ptr<Territory> targetTerritory;

public:
  // DEFAULT CONSTRUCTOR to initialize any data members or set up the object.
  Airlift();

  // PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object.
  Airlift(Player *issuingPlayer, const int numArmies, const Territory &sourceTerritory, const Territory &targetTerritory);

  // COPY CONSTRUCTOR to initialize an object from another object.
  Airlift(const Airlift &airliftObject);

  // ASSIGNMENT OPERATOR to assign an object to another object.
  Airlift &operator=(const Airlift &airliftObject);

  // Virtual print function to be overridden by derived classes
  void print(ostream &output) const override;

  // STREAM INSERTION OPERATOR to print the contents of an object.
  //  friend ostream &operator<<(ostream &output, const Airlift &airliftObject);

  // VALIDATE
  bool validate() override;

  // EXECUTE
  void execute() override;

  // GETTERS
  int getNumArmies();
  Territory getSourceTerritory();
  Territory getTargetTerritory();

  // SETTERS
  void setNumArmies(int numArmies);
  void setSourceTerritory(const Territory &sourceTerritory);
  void setTargetTerritory(const Territory &targetTerritory);
};
// ........... 5. AIRLIFT ..............

// ........... 6. NEGOTIATE ..............
class Negotiate : public Order
{
private:
  Player *targetPlayer;

public:
  // DEFAULT CONSTRUCTOR to initialize any data members or set up the object.
  Negotiate();

  // PARAMETERIZED CONSTRUCTOR to initialize any data members or set up the object.
  Negotiate(Player *issuingPlayer, Player *targetPlayer);

  // COPY CONSTRUCTOR to initialize an object from another object.
  Negotiate(const Negotiate &negotiateObject);

  // ASSIGNMENT OPERATOR to assign an object to another object.
  Negotiate &operator=(const Negotiate &negotiateObject);

  // Virtual print function to be overridden by derived classes
  void print(ostream &output) const override;

  // STREAM INSERTION OPERATOR to print the contents of an object.
  //  friend ostream &operator<<(ostream &output, const Negotiate &negotiateObject);

  // VALIDATE
  bool validate() override;

  // EXECUTE
  void execute() override;

  // GETTERS
  Player *getTargetPlayer() const;

  // SETTERS
  void setTargetPlayer(Player *targetPlayer); // Takes a pointer to Player
};
// ........... 6. NEGOTIATE ..............

// ........... 7. ORDERS LIST ..............
class OrdersList : public Subject, public ILoggable
{
private:
  std::shared_ptr<std::vector<std::shared_ptr<Order>>> orders;

public:
  // Default constructor
  OrdersList();

  // Parameterized constructor using shared_ptr
  OrdersList(const std::vector<std::shared_ptr<Order>> &initOrders);

  // Copy constructor
  OrdersList(const OrdersList &otherList);

  // Assignment operator
  OrdersList &operator=(const OrdersList &otherList);

  // Stream insertion operator
  friend std::ostream &operator<<(std::ostream &output, const OrdersList &ordersListObject);

  // Getters
  const std::vector<std::shared_ptr<Order>> &getOrders() const;

  // Setters
  void setOrders(const std::vector<std::shared_ptr<Order>> &newOrders);

  // Add order
  void addOrder(const std::shared_ptr<Order> &order);

  // Remove order
  void removeOrder(size_t index);

  // Move order
  void moveOrder(size_t index, OrdersList &otherOrdersList);

  // Execute all orders
  void executeAll();

  // STRING-TO-LOG
  string stringToLog() const override;
};
// ........... 7. ORDERS LIST ..............
#endif