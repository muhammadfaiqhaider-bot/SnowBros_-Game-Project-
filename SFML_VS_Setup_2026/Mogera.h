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
        
        health = 10;                // Health Increased

        spawnTimer = 0;
        childSpawnInterval = 180;    // Spawn child every 3 seconds simple Maths time= frames(180)/fps(60)..

        playerX = 0;               // 0 bcz Mogera is stationary ......
        playerY = 0;               //

        isDefeated = false;
    }

    
    void setPlayerPosition(float pX, float pY) // every frame this function is called and 
    {                                          // player coordinates are passed........
        playerX = pX;
        playerY = pY;
    }

    void movementsUpdate() override
    {

        if (isDefeated)                      // Kinda base condition if it is defeated we ll be instantly 
        {                                    // backed from the function..........
            return;
        }


       

        
        spawnTimer++;
        if (spawnTimer >= childSpawnInterval)  // This is the child Mogera spawn logic
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

        // Mogera - large green rectangle (boss takes more space)
        sf::RectangleShape mogeraShape(sf::Vector2f(80.f, 80.f));
        mogeraShape.setFillColor(sf::Color(0, 180, 0));     // Dark green
        mogeraShape.setPosition(x, y);
        window.draw(mogeraShape);

        // Draw health bar above Mogera
        // Background of health bar (red)
        sf::RectangleShape healthBarBackground(sf::Vector2f(80.f, 10.f));
        healthBarBackground.setFillColor(sf::Color::Red);
        healthBarBackground.setPosition(x, y - 15.f);
        window.draw(healthBarBackground);

        // Foreground of health bar (green) - shrinks as health decreases
        float healthPercent = (float)health / 10.0f;
        sf::RectangleShape healthBarForeground(sf::Vector2f(80.f * healthPercent, 10.f));
        healthBarForeground.setFillColor(sf::Color::Green);
        healthBarForeground.setPosition(x, y - 15.f);
        window.draw(healthBarForeground);

        // Draw all active children
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