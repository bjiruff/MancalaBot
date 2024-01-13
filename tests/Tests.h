#ifndef Tests_h
#define Tests_h

#include "Side.h"
#include "Board.h"
#include "Player.h"
#include "Game.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;


void doBoardTests()
{
    Board b(3, 2);
    assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
                    b.beans(SOUTH, POT) == 0  &&  b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);
    assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
                b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, POT) == 2  &&
        b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
                b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
        b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
}

void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board b(3, 2);
    b.setBeans(SOUTH, 2, 0);
    cout << "=========" << endl;
    int n = hp.chooseMove(b, SOUTH);
    cout << "=========" << endl;
    assert(n == 1  ||  n == 3);
    n = bp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    n = sp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
}

void doGameTests()
{
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
      //    Homer
      //   0  1  2
      // 0         0
      //   2  0  0
      //    Bart
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
    g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
      //   0  1  0
      // 0         3
      //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(NORTH);
      //   1  0  0
      // 0         3
      //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
      //   1  0  0
      // 0         3
      //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

    g.move(NORTH);
      //   0  0  0
      // 1         4
      //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
}

void boardTests()
{
    // Test if beans, holes, beansInPlay, and totalBeans functions are working correctly for board with 6 holes and 4 beans per hole
    Board a(6, 4);
    assert(a.holes() == 6);
    assert(a.beans(NORTH, 1) == 4);
    assert(a.beans(NORTH, POT) == 0);
    assert(a.beansInPlay(NORTH) == 24);
    assert(a.totalBeans() == 48);

    // Test if beans, holes, beansInPlay, and totalBeans functions are working correctly for board with 3 holes and 3 beans per hole
    Board b(3, 3);
    assert(b.holes() == 3);
    assert(b.beans(SOUTH, 1) == 3);
    assert(b.beans(SOUTH, POT) == 0);
    assert(b.beansInPlay(NORTH) == 9);
    assert(b.totalBeans() == 18);

    // Test if moveToPot works correctly
    Board c(6, 4);
    assert(c.moveToPot(NORTH, POT, NORTH) == false);
    assert(c.moveToPot(NORTH, 7, NORTH) == false);
    assert(c.moveToPot(NORTH, 3, NORTH) == true);
    assert(c.beans(NORTH, 3) == 0 && c.beans(NORTH, POT) == 4);
    assert(c.moveToPot(SOUTH, 3, NORTH) == true);
    assert(c.beans(SOUTH, 3) == 0 && c.beans(NORTH, POT) == 8);

    // Test if setBeans works correctly
    Board d(6, 4);
    assert(d.setBeans(NORTH, POT, -1) == false);
    assert(d.setBeans(NORTH, 7, 1) == false);
    assert(d.setBeans(NORTH, POT, 10) == true);
    assert(d.beansInPlay(NORTH) == 24 && d.totalBeans() == 58);
    assert(d.setBeans(SOUTH, 1, 0) == true);
    assert(d.beansInPlay(SOUTH) == 20 && d.totalBeans() == 54);

    // Test if sow works correctly for invalid holes
    Board e(6, 4);
    Side s1;
    int h1;
    assert(e.sow(NORTH, POT, s1, h1) == false);
    assert(e.setBeans(SOUTH, 1, 0) == true);
    assert(e.sow(SOUTH, 1, s1, h1) == false);
    assert(e.beansInPlay(SOUTH) == 20 && e.totalBeans() == 44);

    // Test to see if sow work from NORTH case
    Board f(6, 4);
    assert(f.sow(NORTH, 1, s1, h1) == true);
    assert(f.beansInPlay(NORTH) == 20);
    assert(f.beans(NORTH, 1) == 0);
    assert(f.beans(NORTH, POT) == 1);
    assert(f.beans(SOUTH, 3) == 5);
    assert(s1 == SOUTH && h1 == 3);

    // Test to see if sow works from SOUTH case
    Board g(6, 4);
    assert(g.sow(SOUTH, 3, s1, h1) == true);
    assert(g.beansInPlay(SOUTH) == 23);
    assert(g.beans(SOUTH, 3) == 0);
    assert(g.beans(SOUTH, POT) == 1);
    assert(g.beans(SOUTH, 4) == 5);
    assert(s1 == SOUTH && h1 == POT);

    // Test to see if sow works with large amount of beans
    Board h(6, 0);
    assert(h.setBeans(NORTH, 2, 20) == true);
    assert(h.sow(NORTH, 2, s1, h1) == true);
    assert(h.beansInPlay(SOUTH) == 11 && h.beansInPlay(NORTH) == 7);
    assert(h.beans(NORTH, POT) == 2 && h.beans(SOUTH, POT) == 0);
    assert(s1 == SOUTH && h1 == 5);

    // Second test to see if sow works with large amount of beans
    Board i(6, 0);
    assert(i.setBeans(SOUTH, 3, 20) == true);
    assert(i.sow(SOUTH, 3, s1, h1) == true);
    assert(i.beansInPlay(SOUTH) == 9 && i.beansInPlay(NORTH) == 9);
    assert(i.beans(NORTH, POT) == 0 && i.beans(SOUTH, POT) == 2);
    assert(s1 == NORTH && h1 == 4);
}

void playerTests()
{

    // Test to see if name and isInteractive functions work
    BadPlayer a("Bad");
    assert(a.name() == "Bad");
    assert(a.isInteractive() == false);
    HumanPlayer b("Human");
    assert(b.name() == "Human");
    assert(b.isInteractive() == true);
    SmartPlayer c("Smart");
    assert(c.name() == "Smart");
    assert(c.isInteractive() == false);

    // Testing board #1
    Board t1(6, 0);
    t1.setBeans(NORTH, 1, 10);
    t1.setBeans(NORTH, 2, 1);
    t1.setBeans(NORTH, POT, 10);
    t1.setBeans(SOUTH, POT, 15);

    // HumanPlayer test for board #1
    assert(b.chooseMove(t1, SOUTH) == -1);
    // b.chooseMove(t1, NORTH);

    // BadPlayer test for board #1
    assert(a.chooseMove(t1, SOUTH) == -1);
    assert(a.chooseMove(t1, NORTH) == 1);

    // SmartPlayer test for board #1, see if SmartPlayer chooses winning move
    assert(c.chooseMove(t1, SOUTH) == -1);
    assert(c.chooseMove(t1, NORTH) == 2);

    // Testing board #2
    Board t2(6, 0);
    t2.setBeans(NORTH, 2, 1);
    t2.setBeans(NORTH, 4, 1);
    t2.setBeans(NORTH, 6, 10);
    t2.setBeans(NORTH, POT, 10);
    t2.setBeans(SOUTH, 3, 10);
    t2.setBeans(SOUTH, POT, 10);

    // BadPlayer test for board #2
    assert(a.chooseMove(t2, NORTH) == 2);
    assert(a.chooseMove(t2, SOUTH) == 3);

    // SmartPlayer test for board #2, see if SmartPlayer chooses winning move
    assert(c.chooseMove(t2, NORTH) == 4);
    assert(c.chooseMove(t2, SOUTH) == 3);

    // Testing board #3
    Board t3(6, 0);
    t3.setBeans(SOUTH, POT, 2);
    t3.setBeans(SOUTH, 1, 1);
    t3.setBeans(SOUTH, 3, 1);
    t3.setBeans(SOUTH, 6, 1);
    t3.setBeans(SOUTH, POT, 5);
    t3.setBeans(NORTH, 1, 1);
    t3.setBeans(NORTH, 4, 1);
    t3.setBeans(NORTH, 6, 1);

    // BadPlayer test for board #3
    assert(a.chooseMove(t3, NORTH) == 1);
    assert(a.chooseMove(t3, SOUTH) == 1);

    // SmartPlayer test for board #3, see if SmartPlayer chooses winning move
    assert(c.chooseMove(t3, SOUTH) == 6);

}

void gameTests()
{
    // Test to see if display is working
    SmartPlayer sp("Smart");
    SmartPlayer sp2("Smart2");
    HumanPlayer hp("Human");
    HumanPlayer hp2("Human2");
    BadPlayer bp("Bad");
    BadPlayer bp2("Bad2");
    Board a(6, 4);
    Board b(3, 0);
    Board c(5, 10);
    b.setBeans(SOUTH, POT, 10);
    Game g1(a, &sp, &bp2);
    g1.display();
    Game g2(b, &sp, &bp);
    g2.display();
    Game g3(c, &sp, &bp);
    g3.display();

    // Test #1 for status function
    bool over;
    bool hasWinner;
    Side winner = NORTH;
    Board d(6, 0);
    Game g4(d, &sp, &bp);
    g4.status(over, hasWinner, winner);
    assert(over == true && hasWinner == false && winner == NORTH);

    // Test #2 for status function
    Board e(6, 1);
    Game g5(e, &sp, &bp);
    g5.status(over, hasWinner, winner);
    assert(over == false && hasWinner == false && winner == NORTH);

    // Test #3 for status function
    Board f(6, 0);
    f.setBeans(SOUTH, POT, 1);
    Game g6(f, &sp, &bp);
    g6.status(over, hasWinner, winner);
    assert(over == true && hasWinner == true && winner == SOUTH);

    // Test move function for two HumanPlayers
    Board g(6, 0);
    g.setBeans(SOUTH, 1, 1);
    g.setBeans(SOUTH, 6, 2);
    Game g7(g, &hp, &hp2);
    //assert(g7.move(SOUTH) == true);
    assert(g7.move(NORTH) == false);
    assert(g7.beans(SOUTH, POT) == 3);

    // Test move function for two BadPlayers
    Game g8(g, &bp, &bp2);
    assert(g7.move(NORTH) == false);
    assert(g7.beans(SOUTH, POT) == 3);

    // Test move function for two SmartPlayers
    Game g9(g, &sp, &sp2);
    assert(g9.move(NORTH) == false);
    assert(g9.beans(SOUTH, POT) == 3);

    // Test beans function
    assert(g.beans(SOUTH, 2) == 0);
    assert(g.beans(SOUTH, POT) == 0);
    assert(g.beans(NORTH, POT) == 0);
    assert(g.beans(SOUTH, 7) == -1);
    assert(g.beans(SOUTH, -1) == -1);
    assert(g.beans(SOUTH, 6) == 2);
    assert(g.beans(SOUTH, 1) == 1);
    assert(g.beans(NORTH, 6) == 0);
}

void playTests()
{
    Board a(6, 4);

    SmartPlayer sp("Smart");
    SmartPlayer sp2("Smart2");
    HumanPlayer hp("Human");
    HumanPlayer hp2("Human2");
    BadPlayer bp("Bad");
    BadPlayer bp2("Bad2");

    // See if two HumanPlayers can play
//    Game g1(a, &hp, &hp2);
//    g1.play();

    // See if two BadPlayers can play
    Game g2(a, &bp, &bp2);
    g2.play();

    // See if a SmartPlayer and a BadPlayer can play, the SmartPlayer should win, test #1, SmartPlayer is NORTH
    Game g3(a, &bp, &sp);
    g3.play();

    // See if a SmartPlayer and a BadPlayer can play, the SmartPlayer should win, test #2, SmartPlayer is SOUTH
    Game g4(a, &sp, &bp);
    g4.play();

    // See if two SmartPlayers can play, should be relatively even
    Game g5(a, &sp, &sp2);
    g5.play();
}

#endif
