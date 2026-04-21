#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Enemy
{
protected:
    float x;
    float y;
    std::string name;
    float velocityX;
    float velocityY;
    int health;
    bool snowCovered;
    sf::Sprite enemySprite;             // these two variables are for storing actual Images / also they are classes indide 
    sf::Texture enemyTexture;           // sfml library

public:
    Enemy(float posX, float posY, std::string n)
    {
        x = posX;
        y = posY;
        name = n;
        velocityX = 0;
        velocityY = 0;
        health = 1;
        snowCovered = false;
    }

    virtual void movementsUpdate() = 0;
    virtual void DisplayEnemy(sf::RenderWindow& window) = 0;


    // Reducing Health Logic
    virtual void reduceHealth()      
    {
        health--;

        // When health hits 0 enemy becomes fully encased
        if (health <= 0)
        {
            snowCovered = true;
            health = 0; // Prevent negative health
        }
    }

    // Check if Fully encased means is enemy covered with snow if YES means now it will be ready to roll...
    bool isFullyEncased()
    {
        return snowCovered;
    }

    // Getter for retrinving position  both x and y coordinate (our game resolution is 600x600 so its according to that 
    float getPositionX()
    {
        return x;
    }
    float getPositionY()
    {
        return y;
    }

    
    bool getSnowCoveredStatus()
    {
        return snowCovered;
    }

    // Getter for Health..
    int getHealth()
    {
        return health;
    }

    // Getname Getter,,,
    std::string getName()
    {
        return name;
    }



    // CHEEEMA This is a hitbox function that returns the invisible rectangular boundary around my sprite which is used for collision detection.
    sf::FloatRect getHitBox()
    {
        return enemySprite.getGlobalBounds();
    }

    virtual ~Enemy()
    {
    }
};









