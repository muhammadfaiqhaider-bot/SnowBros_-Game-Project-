#pragma once
#include "Projectile.h"

class Knives : public Projectile
{
public:
    Knives(float startX, float startY, float targetX, float targetY, float limit) : Projectile(startX, startY, limit, limit)
    {
        
        calculateDirection(targetX, targetY, 4.0f);      // Use Projectile's Class calculateDirection method
    }

    void updateTrajactory() override
    {
        if (!active)
        {
            return;
        }

        x = x + velocityX;                         // Update Positions along x-axis
        y = y + velocityY;                         // Update Positions along y-axis

        checkScreenBoundary();                     // Check for screen Limit.....Either satisfy resolutionso of game that is 600x600
    }

    void drawProjectiles(sf::RenderWindow& window) override
    {
        if (!active)
        {
            return;
        }

        // Yellow small rectangle for knife
        sf::RectangleShape knifeShape(sf::Vector2f(15.f, 5.f));
        knifeShape.setFillColor(sf::Color::Yellow);
        knifeShape.setPosition(x, y);
        window.draw(knifeShape);
    }

    ~Knives()
    {
    }
};