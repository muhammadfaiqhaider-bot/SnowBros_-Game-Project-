#pragma once
#include "Enemy.h"

class MogeraChild : public Enemy
{
private:
    bool active;
    bool onGround;

public:
    MogeraChild() : Enemy(0, 0, "MogeraChild")
    {
        active = false;
        onGround = false;
        velocityX = 0;
        velocityY = 0;
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
            velocityY = velocityY + 0.5f;                   // This function is used to bring mogera childs to ground
        }
        else
        {
            velocityY = 0;
        }

        x = x + velocityX;
        y = y + velocityY;

        // Deactivate when goes off screen
        if (x < 0 || x > 600)
        {
            active = false;
        }

        // Stop at ground level
        if (y > 520)
        {
            y = 520;
            onGround = true;
        }
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




    void deactivate()
    {
        active = false;
    }

    virtual ~MogeraChild()
    {
    }
};