#include "Game.h"

Game::Game(const Board& b, Player* south, Player* north) : m_board(b), m_north(north), m_south(south), m_winner(NORTH)
{
    // Initialize m_winner to prevent bugs
    status(m_gameIsOver, m_hasWinner, m_winner);
}

void Game::display() const
{
    int numHoles = m_board.holes();
    // Take into account # of holes for formatting purposes
    int amountSpace = numHoles * 4;

    // Print north player's name to be roughly in middle
    std::cout << std::setw(amountSpace / 2) << "" << m_north->name() << std::endl;

    // Print north holes
    for (int i = 1; i <= numHoles; i++)
        std::cout << std::setw(4) << m_board.beans(NORTH, i);
    
    // Format and print the pots, taking into account the number of holes
    std::cout << std::endl << std::setw(2) << m_board.beans(NORTH, POT) << std::setw(amountSpace) << m_board.beans(SOUTH, POT) << std::endl;
    
    // Print south holes
    for (int i = 1; i <= numHoles; i++)
        std::cout << std::setw(4) << m_board.beans(SOUTH, i);
    
    // Print south player's name to be roughly in middle
    std::cout << std::endl << std::setw(amountSpace / 2) << "" << m_south->name() << std::endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    // No holes on both sides means game is over.
    if (m_board.beansInPlay(NORTH) == 0 && m_board.beansInPlay(SOUTH) == 0)
    {
        over = true;
        if (m_board.beans(NORTH, POT) > m_board.beans(SOUTH, POT))
        {
            hasWinner = true;
            winner = NORTH;
        }
        else if (m_board.beans(NORTH, POT) < m_board.beans(SOUTH, POT))
        {
            hasWinner = true;
            winner = SOUTH;
        }
        // Tie
        else
            hasWinner = false;
    }
    else
        over = false;
}

bool Game::move(Side s)
{
    
    int chosenMove;
    Side endSide = NORTH;
    int endHole;
    
    while (true)
    {
        // If no beans in play, cannot make a move and game ends.
        if (m_board.beansInPlay(s) <= 0)
        {
            if (s == NORTH)
                std::cout << m_north->name() << " has no more beans to sow." << std::endl << "Sweeping remaining beans." << std::endl;
            else
                std::cout << m_south->name() << " has no more beans to sow." << std::endl << "Sweeping remaining beans." << std::endl;
            
            for (int i = 1; i <= m_board.holes(); i++)
                m_board.moveToPot(opponent(s), i, opponent(s));
            return false;
        }
        
        if (s == NORTH)
            chosenMove = m_north->chooseMove(m_board, s);
        else
            chosenMove = m_south->chooseMove(m_board, s);
        
        // Sow based on the move chosen.
        m_board.sow(s, chosenMove, endSide, endHole);
        if (s == NORTH)
            std::cout << m_north->name() << " has chosen hole " << chosenMove << "." << std::endl;
        else
            std::cout << m_south->name() << " has chosen hole " << chosenMove << "." << std::endl;
        
        // Take another turn if placed last bean in pot (ie. continue the loop).
        if (endHole == POT)
        {
            if (s == NORTH)
                std::cout << m_north->name() << " gets another turn!" << std::endl;
            else
                std::cout << m_south->name() << " gets another turn!" << std::endl;
            display();
        }
        
        // Conditions for capture
        else if (endSide == s && m_board.beans(s, endHole) == 1 && m_board.beans(opponent(s), endHole) > 0)
        {
            if (s == NORTH)
                std::cout << m_north->name() << " has made a capture!" << std::endl;
            else
                std::cout << m_south->name() << " has made a capture!" << std::endl;
            m_board.moveToPot(s, endHole, s);
            m_board.moveToPot(opponent(s), endHole, s);
            break;
        }
        // Turn ends without special events
        else
            break;
    }
    return true;
}

void Game::play()
{
    display();
    
    // At least one player is human.
    if (m_north->isInteractive() || m_south->isInteractive())
    {
        while(m_gameIsOver == false)
        {
            move(SOUTH);
            display();
            status(m_gameIsOver, m_hasWinner, m_winner);
            
            if (m_gameIsOver == true)
                break;
            
            move(NORTH);
            display();
            status(m_gameIsOver, m_hasWinner, m_winner);
        }
    }
    // Both players are bots. Have user press enter to continue to prevent entire game playing at once.
    else
    {
        while(m_gameIsOver == false)
        {
            move(SOUTH);
            display();
            status(m_gameIsOver, m_hasWinner, m_winner);
            
            if (m_gameIsOver == true) 
                break;
            
            std::cout << "Press ENTER to continue.";
            std::cin.ignore(10000,'\n');
            
            move(NORTH);
            display();
            status(m_gameIsOver, m_hasWinner, m_winner);
            
            if (m_gameIsOver == true)
                break;
            
            std::cout << "Press ENTER to continue.";
            std::cin.ignore(10000,'\n');
        }
    }
    
    if (m_hasWinner == true)
    {
        if (m_winner == NORTH)
            std::cout << m_north->name() << " is the winner!" << std::endl;
        else
            std::cout << m_south->name() << " is the winner!" << std::endl;
    }
    else
        std::cout << "The game has tied." << std::endl;
}

int Game::beans(Side s, int hole) const
{
    if (hole > m_board.holes() || hole < 0)
        return -1;

    return (m_board.beans(s, hole));
}
