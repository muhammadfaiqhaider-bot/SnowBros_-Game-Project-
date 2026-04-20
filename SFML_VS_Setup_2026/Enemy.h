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
    sf::Sprite enemySprite;
    sf::Texture enemyTexture;

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

    // Check if fully encased (ready to be rolled)
    bool isFullyEncased()
    {
        return snowCovered;
    }

    // Getters for Retrieving positions
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

    // Health getter (Student A needs this)
    int getHealth()
    {
        return health;
    }

    std::string getName()
    {
        return name;
    }

    // Hitbox for collision (Student A needs this)
    sf::FloatRect getHitBox()
    {
        return enemySprite.getGlobalBounds();
    }

    virtual ~Enemy()
    {
    }
};









