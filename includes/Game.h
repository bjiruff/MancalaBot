#ifndef Game_h
#define Game_h

#include "Side.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
#include <iomanip>


class Game
{
    
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side s);
    void play();
    int beans(Side s, int hole) const;
    
private:
    Board m_board;
    Player* m_north;
    Player* m_south;
    bool m_gameIsOver;
    bool m_hasWinner;
    Side m_winner;
    
};

#endif
