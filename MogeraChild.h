#pragma once
#include "Enemy.h"

class MogeraChild : public Enemy
{
private:
    bool active;
    bool onGround;
    bool killedBySnowball;

public:
    MogeraChild() : Enemy(0, 0, "MogeraChild")
    {
        active = false;
        onGround = false;
        velocityX = 0;
        velocityY = 0;
        killedBySnowball = false;
    }

    void spawn(float startX, float startY, float playerX)
    {
        x = startX;                                                    // This is the Important function to realease Child Mogera's in the the direction 
        y = startY;                                                    // where our person is and set velocity according to it 
        active = true;
        onGround = false;

        if (playerX > x)
        {
            velocityX = 1.3f;
        }
        else
        {
            velocityX = -1.3f;
        }

        velocityY = 0;
    }

    void movementsUpdate() override
    {
        if (!active)
        {
            return;
        }

        if (!onGround)
        {
            velocityY = velocityY + 0.5f;
        }
        else
        {
            velocityY = 0;
        }

        x = x + velocityX;
        y = y + velocityY;

        // Turn around at walls instead of deactivating
        if (x < 0)
        {
            x = 0;
            velocityX = -velocityX;     // Reverse direction
        }

        if (x > 560)
        {
            x = 560;
            velocityX = -velocityX;     // Reverse direction
        }

        // Stop at ground level
        if (y > 520)
        {
            y = 520;
            onGround = true;
        }
    }

    // Called when hit by a snowball - child dies immediately
    void instantKill() override
    {
        active = false;
        killedBySnowball = true;
    }

    // Check collision with player - deactivate and signal hit
    bool checkPlayerCollision(float playerX, float playerY)
    {
        if (!active) return false;
        sf::FloatRect childBounds(x, y, 20.f, 20.f);
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);
        if (childBounds.intersects(playerBounds))
        {
            active = false;
            return true;
        }
        return false;
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (!active)
        {
            return;
        }

        sf::RectangleShape childShape(sf::Vector2f(20.f, 20.f));   // Orange Rectangle is used to denote Child Mogera's 
        childShape.setFillColor(sf::Color(255, 165, 0));
        childShape.setPosition(x, y);
        window.draw(childShape);
    }

    bool isActive()
    {
        return active;
    }

    void setOnGround(bool value)
    {
        onGround = value;
    }

    void snapToGround(float groundY)
    {
        y = groundY;
        velocityY = 0;
        onGround = true;
    }


    void deactivate()
    {
        active = false;
    }

    float getVelocityY()
    {
        return velocityY;
    }

    virtual ~MogeraChild()
    {
    }
};