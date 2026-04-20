#pragma once
#include "Projectile.h"

class Knives : public Projectile
{
public:
    Knives(float startX, float startY, float targetX, float targetY, float limit)
        : Projectile(startX, startY, limit, limit)
    {
        // Use parent's calculateDirection method
        calculateDirection(targetX, targetY, 4.0f);
    }

    void update() override
    {
        if (!active)
        {
            return;
        }

        x = x + velocityX;
        y = y + velocityY;

        // Use parent's boundary check
        checkScreenBoundary();               // Check for screen Limit.....
    }

    void draw(sf::RenderWindow& window) override
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