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


    int frameWidth;     //alright so this is like the shyt we need for animations
    int frameHeight;
    int totalFrames;
    int currentFrame;
    int animTimer;
    int animSpeed;
    bool facingRight;
    bool textureLoaded;




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

        frameWidth = 88;
        frameHeight = 88;
        totalFrames = 1;
        currentFrame = 0;
        animTimer = 0;
        animSpeed = 10;
        facingRight = true;
        textureLoaded = false;

    }

    virtual void movementsUpdate() = 0;
    virtual void DisplayEnemy(sf::RenderWindow& window) = 0;

    virtual bool getIsDead() { return false; }
    virtual bool getIsEncased() { return snowCovered; }
    virtual bool getIsRolling() { return false; }
    virtual void kickRoll(int direction) {}
    virtual void instantKill() {}
    virtual float getRollVelocityX() { return 0.f; }
    virtual void setHitWall(bool value) {}
    virtual void setOnGround(bool value) {}
    virtual void snapToGround(float groundY) { y = groundY; velocityY = 0; }


    // Reducing Health Logic
    virtual void reduceHealth()
    {
        health--;

        // When health hits 0 enemy becomes fully encased
        if (health <= 0)
        {
            health = 0; // Prevent negative health
            snowCovered = true;
            
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

    //Needed ts getter for collision checks in main. - cheema 
    float getVelocityY()
    {
        return velocityY;
    }

    virtual void setTint(sf::Color color)
    {
        enemySprite.setColor(color);
    }
    // CHEEEMA This is a hitbox function that returns the invisible rectangular boundary around my sprite which is used for collision detection.
    // Nilla i aint blind - Cheema
    sf::FloatRect getHitBox()
    {
        return enemySprite.getGlobalBounds();
    }

    virtual ~Enemy()
    {
    }
};