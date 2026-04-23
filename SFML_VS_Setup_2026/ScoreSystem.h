#pragma once
#include <iostream>
#include <string>
#include <cstdlib>  

using namespace std;

class ScoreManager
{
private:
    int currentscore;
    int chaincount;                   // This Chain count is for Multiple kills in 1 snow rolling
    int gemcount;

    int lastscore;                    // needed for chain bonus

public:

    ScoreManager()
    {
        currentscore = 0;
        chaincount = 0;
        gemcount = 0;
        lastscore = 0;
    }



    void addKillEnemyScore(string enemyType)
    {
        int baseScore = 0;

        if (enemyType == "Botom")
            baseScore = rand() % 401 + 100;

        else if (enemyType == "FlyingFoogo")
            baseScore = rand() % 601 + 200;

        else if (enemyType == "Tornado")
            baseScore = rand() % 901 + 300;

        else if (enemyType == "Mogera")
            baseScore = 5000;

        else if (enemyType == "Gamakichi")
            baseScore = 10000;

        lastscore = baseScore;
        currentscore = currentscore + lastscore;
    }

    // Chain bonus
    void addChainKillBonus()
    {
        chaincount++;

        float bonus = lastscore * 0.10f * chaincount;
        currentscore = currentscore + bonus;
    }

    void resetChain()                     // After Chain Killing this fucntion is use to reset the chain killing to 0..........
    {
        chaincount = 0;
    }


    void addGems(int amount)              // This Function is use to add Gems to the Gem Count
    {
        gemcount = gemcount + amount;
    }

    void addBossGems(string bossType)         // Killing Mogera and Gamakichi gives u more gems that's why have saperate funtion that 
    {                                         // gives more gems by killing them 
        if (bossType == "Mogera")
            gemcount += 200;

        else if (bossType == "Gamakichi")
            gemcount += 500;
    }



    // Leaderboard  File handling implement Shyt will do later (okyyy)
    void saveToLeaderboard()
    {
  
    }
    void loadLeaderboard()
    {

    }




    // Getters functions here ......
    int getScore()
    {
        return currentscore;
    }

    int getGems()
    {
        return gemcount;
    }

    int getChainCount()
    {
        return chaincount;
    }
};