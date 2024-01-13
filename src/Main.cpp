#include "Side.h"
#include "Board.h"
#include "Player.h"
#include "Game.h"
#include "Tests.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

/*
 
 ///////////////////////////////////////////////////MANCALA RULES//////////////////////////////////////////////////////////
 
 Introduction:
 
 Mancala is a generic name for a family of board games that are played with small stones, beans, or seeds and rows of holes
 or pits. The most common version of Mancala is known as Kalah, and it is often played on a board with 6 or 12 small pits
 called houses or pits. Here are the basic rules for playing Mancala:

 
 
 Setup:

 1. The game is typically played by two players, each controlling one half of the board, which consists of 6 or 12 small pits.
 2. Place four stones, beans, or seeds in each of the 12 pits at the beginning of the game.

 
 
 Objective:
 
 The objective of Mancala is to capture more stones than your opponent.

 
 
 Gameplay:

 1. Players take turns picking up all the stones in one of their pits and sowing them counterclockwise into the pits, including their own Mancala (a pit on the right side on each player's row).
 2. Players always move in a counterclockwise direction, and they sow the stones one at a time into each pit, including their Mancala but skipping their opponent's Mancala.
 3. If the last stone is placed into a player's Mancala, that player gets another turn.
 4. If the last stone is placed into an empty pit on the player's side, and the opposite pit from the opponent's side has stones, the player captures both the last stone and the stones in the opposite pit, placing them all in their Mancala. This move is known as a "capture" or "steal."
        
                                                        Player 1
                                                4   4   4   4   4   4  <- Player 1's pits
                       Player 1's Mancala ->  0                       0  <- Player 2's Mancala
                            Player 2's pits ->  4   4   4   4   4   4
                                                        Player 2
 
 

 Ending the Game:
 
 1. The game ends when one player's side is empty. The remaining stones on the other player's side are then captured by that player and placed in their Mancala. The player with the most stones in their Mancala at the end of the game wins.
 2. It's important to note that rules may vary slightly depending on the specific version of Mancala being played. Some variations have additional rules or different setups. Always check the specific rules of the version you are playing to ensure a proper understanding of the game.
 
 */


int main() {
    
    int holes, beans_per_hole;
    char difficulty;
    
    HumanPlayer You("You");
    Player* Them = nullptr;

    cout << "Hello! Let's begin by setting up the game board." << endl;
    cout << "How many holes would you like on each side (standard is 6)? ";
    cin >> holes;
    cin.ignore(10000, '\n');
    cout << "How many beans per hole would you like to start with (standard is 4)? ";
    cin >> beans_per_hole;
    cin.ignore(10000, '\n');

    Board gameboard(holes, beans_per_hole);

    while (true)
    {
        cout << "Press [p] if you would like to play against another player, [e] for an easy bot, and [h] for a hard bot. ";
        cin >> difficulty;
        if (difficulty == 'p')
        {
            Them = new HumanPlayer("Them");
            break;
        }
        else if (difficulty == 'e')
        {
            Them = new BadPlayer("Them");
            break;
        }
        else if (difficulty == 'h')
        {
            Them = new SmartPlayer("Them");
            break;
        }
    }

    Game game(gameboard, &You, Them);
    game.play();

    
    delete Them;
    
    return 0;
}
