#pragma once
#include <string>
#include "SnowBall.h"
#include <SFML/Graphics.hpp>

class Player
{
protected:

    std::string name;
    float x;
    float y;
    float velocityX;
    float velocityY;
    int direction;
    bool onGround;
    bool isJumping;

    int lives;
    float speed;
    float snowballPower;
    float snowballDistance;

    int score;
    int gemsCount;

    bool lifeStatus;
    bool isInvincible;
    int invincibleTimer;

    bool SpeedBoostStatus;
    bool powerBallactiveStatus;
    bool distanceIncreaseStatus;
    bool hasBalloonMode;
    int powerUpTimer;


    Snowball* snowball;

    sf::Sprite playerSprite;
    sf::Texture playerTexture;



public:
    Player(float posX, float posY, std::string n)
    {
        x = posX;
        y = posY;
        velocityX = 0;
        velocityY = 0;
        direction = 1;
        onGround = false;
        isJumping = false;

        name = n;

        lives = 2;
        speed = 3.0f;
        snowballPower = 1.0f;
        snowballDistance = 200.0f;  // Medium range

        // Economy
        score = 0;
        gemsCount = 0;

        
        lifeStatus = true; 
        isInvincible = false;              // Invinsible is Important status when player got hit by some enemy so there
        invincibleTimer = 0;               // must be small invinsible status when he don't get's damage further more

 
        SpeedBoostStatus = false;            // These are some powerUps some of em are picked up from the gorund 
        powerBallactiveStatus = false;       // or some can be bought from the shop...........
        distanceIncreaseStatus = false;
        hasBalloonMode = false;
        powerUpTimer = 0;


        snowball = nullptr;

    }

    // Pure virtual 
    virtual void movementsUpdate() = 0;
    virtual void displayPlayer(sf::RenderWindow& window) = 0;
    virtual void loseLife()
    {
        if (isInvincible)        // During invinsible mode u can't loose more health...or die...
        {
            return;
        }
        lives--;

        if (lives <= 0)
        {
            lives = 0;
            lifeStatus = false;    // You die game over HUHAHAHA........
        }
        else
        {
            isInvincible = true;
            invincibleTimer = 120;  // 2 seconds at 60 FPS
        }
    }

    // Update invincibility timer - called every frame
    void updateInvincibility()
    {
        if (isInvincible)
        {
            invincibleTimer--;

            if (invincibleTimer <= 0)        // This Funtion at as two minute timer for invinsible mode..
            {
                isInvincible = false;
                invincibleTimer = 0;
            }
        }
    }

    // PowerUp appliers
    void applySpeedBoost()
    {
        SpeedBoostStatus = true;
        speed = speed * 1.5f;       // 50% speed increase from spec
        powerUpTimer = 900;         // 15 seconds at 60 FPS
    }

    void applySnowballPower()
    {
        powerBallactiveStatus = true;
        snowballPower = 2.0f;       // One hit encasing
    }

    void applyDistanceIncrease()
    {
        distanceIncreaseStatus = true;
        snowballDistance = 400.0f;  // Full screen width
    }

    void applyBalloonMode()
    {
        hasBalloonMode = true;
        powerUpTimer = 600;         // 10 seconds at 60 FPS
    }

    // Update powerup timers - called every frame
    void updatePowerUps()
    {
        if ( hasBalloonMode)
        {
            powerUpTimer--;

            if (powerUpTimer <= 0)
            {

             
                if (hasBalloonMode)
                {
                    hasBalloonMode = false;
                }

                powerUpTimer = 0;
            }
        }
        if (SpeedBoostStatus )
        {
            powerUpTimer--;

            if (powerUpTimer <= 0)
            {
                
                if (SpeedBoostStatus)
                {
                    SpeedBoostStatus = false;
                    speed = 3.0f;       // Reset to default speed
                }

                powerUpTimer = 0;
            }
        }
    }

    // Economy
    void addGems(int amount)
    {
        gemsCount = gemsCount + amount;
    }

    void addScore(int amount)
    {
        score = score + amount;
    }

    void spendGems(int amount)
    {
        if (gemsCount >= amount)
        {
            gemsCount = gemsCount - amount;
        }
    }


    void applyGravity()
    {
        float gravity = 0.5f;

        if (!onGround)
        {
            velocityY += gravity;
            y = y + velocityY;
        }
    }


    void setOnGround(bool value)
    {
        onGround = value;
    }

    void setDirection(int d)
    {
        direction = d;
    }

    void snapToGround(float groundY)
    {
        y = groundY;
        velocityY = 0;
    }

    // Getters
    float getPositionX()
    {
        return x;
    }

    float getPositionY()
    {
        return y;
    }

    int getLives()
    {
        return lives;
    }

    int getScore()
    {
        return score;
    }

    int getGemCount()
    {
        return gemsCount;
    }

    float getSpeed()
    {
        return speed;
    }

    float getSnowballDistance()
    {
        return snowballDistance;
    }

    float getSnowballPower()
    {
        return snowballPower;
    }

    bool getIsAlive()
    {
        return lifeStatus;
    }

    bool getIsInvincible()
    {
        return isInvincible;
    }

    bool getOnGround()
    {
        return onGround;
    }

    int getDirection()
    {
        return direction;
    }

    bool getHasBalloonMode()
    {
        return hasBalloonMode;
    }

    std::string getName()
    {
        return name;
    }

    sf::FloatRect getHitBox()
    {
        return playerSprite.getGlobalBounds();
    }

    void setJump(bool val) //Added this so i could call in main when coliding - cheema
    {
        isJumping = val;
    }






    void baseMovement()
    {
        velocityX = 0;

        // Horizontal movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            velocityX = -speed;
            direction = -1;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            velocityX = speed;
            direction = 1;
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && isJumping == 0) //added ts jumping conditition so that bro doesn't infinite jump -Cheema 


        {
            velocityY = -13.0f; //increased jump velo - Cheema
            onGround = false;
            isJumping = true;
        }

        // Gravity - only when NOT on ground
        if (!onGround)
        {
            velocityY += 0.5f;

            // Cap falling speed
            if (velocityY > 8.0f) //decreased cap - Cheema
            {
                velocityY = 8.0f;
            }
        }
        else
        {
            velocityY = 0;    // On ground - no falling
        }

        // Apply movement
        x += velocityX;
        y += velocityY;

        // Screen boundary
        if (x < 0)
        {
            x = 0;
        }
        if (x > 560)
        {
            x = 560;
        }

        // Hard bottom limit
        if (y >= 520.f)
        {
            y = 520.f;
            velocityY = 0;
            onGround = true;
            isJumping = false;
        }

        // Update timers
        updateInvincibility();
        updatePowerUps();
    }




    void throwSnowball()
    {
        if (snowball == nullptr)
        {
            snowball = new Snowball(x, y, direction, snowballDistance, snowballPower, 600.f, 600.f);
        }
    }

    void updateSnowball()
    {
        if (snowball != nullptr)
        {
            snowball->updateTrajactory();

            if (!snowball->isActive())
            {
                delete snowball;
                snowball = nullptr;
            }
        }
    }

    void drawSnowball(sf::RenderWindow& window)
    {
        if (snowball != nullptr)
        {
            snowball->drawProjectiles(window);
        }
    }

    Snowball* getSnowball()
    {
        return snowball;
    }






    virtual ~Player()
    {
        if (snowball != nullptr)
        {
            delete snowball;
            snowball = nullptr;
        }
    }
};