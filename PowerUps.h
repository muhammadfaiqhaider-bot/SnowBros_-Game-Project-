#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Player;   // Forward declaration

class PowerUps
{
protected:
    float x;                       // X - axis position
    float y;                       // y - axis position
    bool isActive;                 // Is power up there available ont the screen screen or not.......
    bool isCollected;              // is the Powerup is being colected............ 
    std::string powerupName;       // what is the name of the power up....

    sf::Sprite powerupSprite;
    sf::Texture powerupTexture;
    float velocityY;

public:
    PowerUps(float posX, float posY, std::string name)   
    {
        x = posX;
        y = posY;
        powerupName = name;
        isActive = true;                                      // Spawns visible on screen
        isCollected = false;
        velocityY = 0.f; // initialize vertical velocity so gravity works predictably
    }

    // Default physics: apply gravity to the power-up. Subclasses may override but should call base implementation.
    virtual void updatePowerUp()
    {
        // simple gravity
        velocityY += 0.5f;
        if (velocityY > 8.f) velocityY = 8.f;
        y += velocityY;
    }
    virtual void DrawPowerUp(sf::RenderWindow& window) = 0;        // Virtual Function
    virtual void applyEffect(Player& player) = 0;

    // Called when player touches powerup
    void collect()
    {
        isCollected = true;                                   // is being collected and than
        isActive = false;                                         // Disappears from screen
    }

    // Getters
    sf::FloatRect getHitBox()
    {
        return sf::FloatRect(x, y, 20.f, 20.f);    // Fixed size hitbox
    }

    void setPosition(float nx, float ny)
    {
        x = nx;
        y = ny;
    }

    void setVelocityY(float v)
    {
        velocityY = v;
    }

    float getVelocityY() { return velocityY; }

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