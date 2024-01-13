#include "Board.h"
#include <iostream>
#include <iomanip>

Board::Board(int nHoles, int nInitialBeansPerHole) : m_nHoles((nHoles <= 0) ? 1 : nHoles), m_northPotBeans(0), m_southPotBeans(0),     m_northHoles(m_nHoles, (nInitialBeansPerHole < 0) ? 0 : nInitialBeansPerHole), m_southHoles(m_nHoles, (nInitialBeansPerHole < 0) ? 0 : nInitialBeansPerHole)
{

    
}

int Board::holes() const
{
    return m_nHoles;
}


int Board::beans(Side s, int hole) const
{
    if (hole > m_nHoles || hole < 0)
        return -1;
    else if (hole == POT && s == NORTH)
        return m_northPotBeans;
    else if (hole == POT && s == SOUTH)
        return m_southPotBeans;
    else if (s == NORTH)
        return m_northHoles[hole - 1];
    else
        return m_southHoles[hole - 1];
}

int Board::beansInPlay(Side s) const
{
    int numBeans = 0;
    if (s == NORTH)
        for (int i = 0; i < m_nHoles; i++)
            numBeans += m_northHoles[i];
    else
        for (int i = 0; i < m_nHoles; i++)
            numBeans += m_southHoles[i];

    return numBeans;
        
}

int Board::totalBeans() const
{
    int totalBeans = 0;
    for (int i = 0; i < m_nHoles; i++)
        totalBeans = totalBeans + m_northHoles[i] + m_southHoles[i];
    totalBeans = totalBeans + m_northPotBeans + m_southPotBeans;
    return totalBeans;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if (hole > m_nHoles || hole < 1)
        return false;
    else if ((s == NORTH && m_northHoles[hole - 1] <= 0) || (s == SOUTH && m_southHoles[hole - 1] <= 0))
        return false;
    
    int beansLeftToSow = 0;
    bool oneRound = false;
    
    // Sow instructions for NORTH.
    if (s == NORTH)
    {
        beansLeftToSow = m_northHoles[hole - 1];
        m_northHoles[hole - 1] = 0;
        while (beansLeftToSow > 0)
        {
            endSide = NORTH;
            // If haven't already made a cycle (ie. the sow process has just started), start from input hole. Else, start from ending hole (hole #6).
            if (oneRound == false)
            {
                for (int i = hole - 2; i >= 0 && beansLeftToSow > 0; i--)
                {
                    m_northHoles[i]++;
                    beansLeftToSow--;
                    endHole = i + 1;
                }
                oneRound = true;
            }
            else
                for (int i = m_nHoles - 1; i >= 0 && beansLeftToSow > 0; i--)
                {
                    m_northHoles[i]++;
                    beansLeftToSow--;
                    endHole = i + 1;
                }
            
            // Put bean in pot if there are beans left
            if (beansLeftToSow > 0)
            {
                m_northPotBeans++;
                beansLeftToSow--;
                endHole = POT;
            }
            else
                break;

            if (beansLeftToSow > 0)
                endSide = SOUTH;
            
            // Put beans in opposing side's holes
            for (int i = 0; i < m_nHoles && beansLeftToSow > 0; i++)
            {
                m_southHoles[i]++;
                beansLeftToSow--;
                endHole = i + 1;
            }
        }
    }
    // Sow instructions for SOUTH.
    else
    {
        beansLeftToSow = m_southHoles[hole - 1];
        m_southHoles[hole - 1] = 0;
        while (beansLeftToSow > 0)
        {
            endSide = SOUTH;
            // If haven't already made a cycle (ie. the sow process has just started), start from input hole. Else, start from beginning hole (hole #1).
            if (oneRound == false)
            {
                for (int i = hole; i < m_nHoles && beansLeftToSow > 0; i++)
                {
                    m_southHoles[i]++;
                    beansLeftToSow--;
                    endHole = i + 1;
                }
                oneRound = true;
            }
            else
                for (int i = 0; i < m_nHoles && beansLeftToSow > 0; i++)
                {
                    m_southHoles[i]++;
                    beansLeftToSow--;
                    endHole = i + 1;
                }
            
            // Put bean in pot if there are beans left
            if (beansLeftToSow > 0)
            {
                m_southPotBeans++;
                beansLeftToSow--;
                endHole = POT;
            }
            else
                break;
            
            if (beansLeftToSow > 0)
                endSide = NORTH;
            
            // Put beans in opposing side's holes
            for (int i = m_nHoles - 1; i >= 0 && beansLeftToSow > 0; i--)
            {
                m_northHoles[i]++;
                beansLeftToSow--;
                endHole = i + 1;
            }
        }
    }
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if (hole > m_nHoles || hole < 1)
        return false;
    // 4 cases: 1. potOwner = NORTH and side = NORTH, 2. potOwner = NORTH and side = SOUTH, 3. potOwner = SOUTH and side = NORTH, 4. potOwner = SOUTH and side = SOUTH
    if (potOwner == NORTH)
    {
        if (s == NORTH)
        {
            m_northPotBeans += m_northHoles[hole - 1];
            m_northHoles[hole - 1] = 0;
        }
        else
        {
            m_northPotBeans += m_southHoles[hole - 1];
            m_southHoles[hole - 1] = 0;
        }
    }
    else
    {
        if (s == NORTH)
        {
            m_southPotBeans += m_northHoles[hole - 1];
            m_northHoles[hole - 1] = 0;
        }
        else
        {
            m_southPotBeans += m_southHoles[hole - 1];
            m_southHoles[hole - 1] = 0;
        }
    }
    return true;
}

bool Board::setBeans(Side s, int hole, int beans)
{
    if (hole > m_nHoles || hole < 0 || beans < 0)
        return false;
    
    if (hole == POT && s == NORTH)
        m_northPotBeans = beans;
    else if (hole == POT && s == SOUTH)
        m_southPotBeans = beans;
    else if (s == NORTH)
        m_northHoles[hole - 1] = beans;
    else
        m_southHoles[hole - 1] = beans;
    
    return true;
}


