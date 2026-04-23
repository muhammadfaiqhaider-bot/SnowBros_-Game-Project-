#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Player;   // Forward declaration

class PowerUps
{
protected:
    float x;                       // X - axis position
    float y;                       // y - axis position
    bool isActive;                 // Is power up there on screen or not ??
    bool isCollected;              // is the Powerup is collected or not ????
    std::string powerupName;       // what's the name of the power up....

    sf::Sprite powerupSprite;
    sf::Texture powerupTexture;

public:
    PowerUps(float posX, float posY, std::string name)     // Default constructor
    {
        x = posX;
        y = posY;
        powerupName = name;
        isActive = true;                                      // Spawns visible on screen
        isCollected = false;
    }

    virtual void updatePowerUp() = 0;                              // Virtual Function
    virtual void DrawPowerUp(sf::RenderWindow& window) = 0;        // Virtual Function
    virtual void applyEffect(Player& player) = 0;

    // Called when player touches powerup
    void collect()
    {
        isCollected = true;
        isActive = false;                                         // Disappears from screen
    }

    // Getters
    sf::FloatRect getHitBox()
    {
        return sf::FloatRect(x, y, 30.f, 30.f);    // Fixed size hitbox
    }

    float getX()
    {
        return x;
    }

    float getY()
    {
        return y;
    }

    bool getActiveStatus()
    {
        return isActive;
    }

    bool getCollectedStatus()
    {
        return isCollected;
    }

    std::string getName()
    {
        return powerupName;
    }

    virtual ~PowerUps()
    {
    }
};