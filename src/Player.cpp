#include "Player.h"

Player::Player(std::string name) : m_name(name)
{
    
}

std::string Player::name() const
{
    return m_name;
}

bool Player::isInteractive() const
{
    return false;
}


bool HumanPlayer::isInteractive() const
{
    return true;
}

HumanPlayer::HumanPlayer(std::string name) : Player(name)
{
    
}

BadPlayer::BadPlayer(std::string name) : Player(name)
{
    
}

SmartPlayer::SmartPlayer(std::string name) : Player(name)
{
    
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    if (b.beansInPlay(s) <= 0)
        return -1;
    
    int chooseHole;
    while (true)
    {
        std::cout << "Choose a hole to sow, " << name() << ": ";
        std::cin >> chooseHole;
        if (chooseHole >= 1 && chooseHole <= b.holes() && b.beans(s, chooseHole) > 0)
            break;
        else
            std::cout << "You can't sow from that hole, silly." << std::endl;
    }
    
    return chooseHole;
}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    int chooseHole = -1;
    
    // The first hole with a non-zero amount of beans is chosen
    for (int i = 1; i <= b.holes(); i++)
    {
        if (b.beans(s, i) > 0)
        {
            chooseHole = i;
            break;
        }
    }
    
    return chooseHole;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    int bestHole;
    int value;
    int depth = 1;
    
    // Start timer to make sure chooseMove doesn't take more than 5 seconds.
    Timer timer;
    timer.start();
    
    minimax(b, s, bestHole, value, depth, timer);
    
    return bestHole;
}


void SmartPlayer::minimax(const Board& b, Side s, int& bestHole, int& value, int depth, Timer& timer) const
{
    // Stop recursive call if can't make a move, or if depth is greater than 5, or if we are about to hit 5 seconds.
    if (b.beansInPlay(s) <= 0 || depth > 5 || timer.elapsed() > 4800)
    {
        value = evaluate(b, s);
        bestHole = -1;
        return;
    }
    
    // Choose bestHole to be the first nonzero hole.
    for (int i = 1; i <= b.holes(); i++)
    {
        if (b.beans(s, i) > 0)
        {
            bestHole = i;
            break;
        }
    }
    
    // Assume value is the worst it can be for given side.
    if (s == NORTH)
        value = INT_MAX;
    else
        value = INT_MIN;
    
    Side endSide = NORTH;
    int endHole;
    // Loop through all possible moves.
    for (int i = bestHole; i <= b.holes(); i++)
    {
        // Skip empty holes
        if (b.beans(s, i) == 0)
            continue;
        
        // Construct copy of board and make a move.
        Board nextMove = b;
        nextMove.sow(s, i, endSide, endHole);
        
        
        int valueNext;
        int bestHoleNext;
        
        // If endHole is POT, can make another move so call minimax with the same conditions.
        if (endHole == POT)
        {
            minimax(nextMove, s, bestHoleNext, valueNext, depth, timer);
            // If we find a better game position than the current best one, change bestHole to be the one that leads to the better position
            if ((s == NORTH && valueNext <= value) || (s == SOUTH && valueNext >= value))
            {
                bestHole = i;
                value = valueNext;
            }
            continue;
        }
        
        // Conditions for capture.
        if (endSide == s && nextMove.beans(s, endHole) == 1 && nextMove.beans(opponent(s), endHole) > 0)
        {
            nextMove.moveToPot(s, endHole, s);
            nextMove.moveToPot(opponent(s), endHole, s);
        }
        
        // Turn has ended at this point, call minimax w/ conditions for opponent's move.
        minimax(nextMove, opponent(s), bestHoleNext, valueNext, depth + 1, timer);
        
        // If we find a better game position than the current best one, change bestHole to be the one that leads to the better position
        if ((s == NORTH && valueNext <= value) || (s == SOUTH && valueNext >= value))
        {
            bestHole = i;
            value = valueNext;
        }
    }
    return;
}


int SmartPlayer::evaluate(const Board&b, Side s) const
{
    // Four positions where player is guaranteed to win. Evaluate these first before taking difference between SOUTH and NORTH pots.
    
    // 1. The player's pot has the majority of beans or the opponent's pot has the majority of beans.
    if (b.beans(s, POT) >= b.totalBeans() / 2 + 1)
    {
        if (s == NORTH)
            return INT_MIN;
        else
            return INT_MAX;
    }
    else if (b.beans(opponent(s), POT) >= b.totalBeans() / 2 + 1)
    {
        if (s == NORTH)
            return INT_MAX;
        else
            return INT_MIN;
    }
    
    Side endSide = NORTH;
    int endHole;

    // 2. The player's pot will have the majority of beans after one turn.
    for (int i = 1; i < b.holes(); i++)
    {
        Board evalBoard = b;

        if (evalBoard.beans(s, i) == 0)
            continue;

        evalBoard.sow(s, i, endSide, endHole);
        if (evalBoard.beans(s, POT) >= evalBoard.totalBeans() / 2 + 1)
        {
            if (s == NORTH)
                return INT_MIN;
            else
                return INT_MAX;
        }
    }

    // 3. All of the player's holes are empty, and opponent will be able to sweep the rest of the beans to win.
    if (b.beansInPlay(s) == 0 && (b.beansInPlay(opponent(s)) + b.beans(opponent(s), POT)) >= b.totalBeans() / 2 + 1)
    {
        if (s == NORTH)
            return INT_MAX;
        else
            return INT_MIN;
    }
    
    // 4. All of the opponent's holes are empty, and the player will be able to take a turn (without giving the opponent's side beans, which would thus give the opponent the chance to move) and sweep the rest of the beans to win.
    if (b.beansInPlay(opponent(s)) == 0 && (b.beansInPlay(s) + b.beans(s, POT)) >= b.totalBeans() / 2 + 1)
    {
        for (int i = 1; i < b.holes(); i++)
        {
            Board evalBoard = b;

            if (evalBoard.beans(s, i) == 0)
                continue;

            evalBoard.sow(s, i, endSide, endHole);
            if (evalBoard.beansInPlay(opponent(s)) == 0 && endHole != POT)
            {
                if (s == NORTH)
                    return INT_MIN;
                else
                    return INT_MAX;
            }
        }
    }


//    for (int i = 1; i < b.holes(); i++)
//    {
//        Side endSide;
//        int endHole;
//        Board evalBoard = b;
//
//        if (evalBoard.beans(s, i) == 0)
//            continue;
//
//        evalBoard.sow(s, i, endSide, endHole);
//        if (endSide == s && evalBoard.beans(s, endHole) == 1 && evalBoard.beans(opponent(s), endHole) > 0)
//        {
//            if (s == NORTH)
//                return (INT_MIN / 2);
//            else
//                return (INT_MAX / 2);
//        }
//    }
//
//    for (int i = 1; i < b.holes(); i++)
//    {
//        Board evalBoard = b;
//
//        if (evalBoard.beans(s, i) == 0)
//            continue;
//
//        evalBoard.sow(s, i, endSide, endHole);
//        if (endHole == POT && endSide == s)
//        {
//            if (s == NORTH)
//                return (INT_MIN / 3);
//            else
//                return (INT_MAX / 3);
//        }
//    }

    // If there are no winning conditions, take difference between SOUTH and NORTH pots
    return (b.beans(SOUTH, POT) - b.beans(NORTH, POT));
}

Player::~Player()
{
    
}
