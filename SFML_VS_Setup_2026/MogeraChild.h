#pragma once
#include "Enemy.h"

class MogeraChild : public Enemy
{
private:
    float velocityX;
    bool active;

public:
    MogeraChild() : Enemy(0, 0, "MogeraChild")
    {
        velocityX = 0;
        active = false;
    }

    void spawn(float startX, float startY, float playerX)
    {
        x = startX;
        y = startY;
        active = true;

        if (playerX > x)
        {
            velocityX = 2;
        }
        else
        {
            velocityX = -2;
        }

    }

    void movementsUpdate() override
    {
        if (!active)
        {
            return;
        }

        x = x +  velocityX;

        // simple boundary
        if (x < 0 || x > 600)
        {
            active = false;
        }

    }

    void DisplayEnemy() override
    {
        if (!active)
        {
            return;
        }

    }

    bool isActive()
    {
        return active;
    }

    void deactivate()
    {
        active = false;
    }
};