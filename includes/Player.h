#ifndef Player_h
#define Player_h

#include <iostream>
#include <string>
#include <climits>
#include "Side.h"
#include "Board.h"


class Player
{
    
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
    
private:
    std::string m_name;
    
};


class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
private:
};

class BadPlayer : public Player
{
public:
    BadPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
private:
};


class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
private:
    int evaluate(const Board& b, Side s) const;
    void minimax(const Board& b, Side s, int& bestHole, int& value, int depth, Timer& timer) const;
};

#endif
