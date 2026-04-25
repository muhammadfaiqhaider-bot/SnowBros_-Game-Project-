#pragma once
#include "Enemy.h"
#include "MogeraChild.h"

class Mogera : public Enemy
{
private:
    float spawnTimer;
    float childSpawnInterval;
    float playerX;
    float playerY;
    bool isDefeated;

    static const int numberOfChilds = 10;
    MogeraChild child[numberOfChilds];

public:
    Mogera(float posX, float posY) : Enemy(posX, posY, "Mogera")
    {

        health = 10;           // Health Increased

        spawnTimer = 0;
        childSpawnInterval = 180;      // Spawn child every 3 seconds simple Maths time= frames(180)/fps(60)..

        playerX = 0;     // 0 bcz Mogera is stationary ......
        playerY = 0;

        isDefeated = false;
    }


    void setPlayerPosition(float pX, float pY) // every frame this function is called and 
    {                                          // player coordinates are passed........
        playerX = pX;
        playerY = pY;
    }

    void movementsUpdate() override
    {

        if (isDefeated)                      // base condition if it is defeated we ll be instantly 
        {                                    // backed from the function..........
            return;
        }


        spawnTimer++;
        if (spawnTimer >= childSpawnInterval)    // This is the child Mogera spawn logic
        {
            for (int i = 0; i < numberOfChilds; i++)
            {
                if (!child[i].isActive())
                {
                    child[i].spawn(x, y, playerX);
                    break;
                }
            }
            spawnTimer = 0;
        }


        for (int i = 0; i < numberOfChilds; i++)
        {
            if (child[i].isActive())
            {
                child[i].movementsUpdate();      // Function called and its movmement is adjusted.....
            }
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (isDefeated)
        {
            return;
        }


        sf::RectangleShape mogeraShape(sf::Vector2f(60.f, 60.0f)); // Mogera Displayed With large green rectangle
        mogeraShape.setFillColor(sf::Color(0, 180, 0));           // Dark green
        mogeraShape.setPosition(x, y);
        window.draw(mogeraShape);


        sf::RectangleShape healthBarBackground(sf::Vector2f(80.0f, 10.0f));         // Red Bar on the Top of Mogera indicating his health 
        healthBarBackground.setFillColor(sf::Color::Red);                           // Decreasing Health........which is overlapped by 
        healthBarBackground.setPosition(x, y - 15.0f);                              // Green bar which is shring with the health decreasing
        window.draw(healthBarBackground);


        float healthPercent = (float)health / 10.0f;
        sf::RectangleShape healthBarForeground(sf::Vector2f(80.f * healthPercent, 10.f));
        healthBarForeground.setFillColor(sf::Color::Green);
        healthBarForeground.setPosition(x, y - 15.f);
        window.draw(healthBarForeground);



        for (int i = 0; i < numberOfChilds; i++)
        {
            if (child[i].isActive())
            {
                child[i].DisplayEnemy(window);
            }
        }
    }

    void reduceHealth() override
    {
        health--;

        if (health <= 0)
        {
            health = 0;
            isDefeated = true;
            snowCovered = true;
        }
    }

    bool getIsDefeated()
    {
        return isDefeated;
    }



    void setChildOnGround(int index, bool value)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            child[index].setOnGround(value);
        }
    }

    int getNumberOfChilds()
    {
        return numberOfChilds;
    }

    float getChildX(int index)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            return child[index].getPositionX();
        }
        return -1;
    }





    float getChildY(int index)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            return child[index].getPositionY();
        }
        return -1;
    }

    virtual ~Mogera()
    {
    }
};