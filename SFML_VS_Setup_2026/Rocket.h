#pragma once
#include "Projectile.h"

class Rocket : public Projectile
{
public:
    Rocket(float startX, float startY,
        float playerX, float playerY,
        float w, float h)
        : Projectile(startX, startY, w, h)
    {
        // Use parent's calculateDirection method
        calculateDirection(playerX, playerY, 4.0f);
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
        checkScreenBoundary();
    }

    void draw(sf::RenderWindow& window) override
    {
        if (!active)
        {
            return;
        }

        // Orange rocket rectangle
        sf::RectangleShape rocketShape(sf::Vector2f(15.f, 8.f));
        rocketShape.setFillColor(sf::Color(255, 100, 0));   // Orange
        rocketShape.setPosition(x, y);
        window.draw(rocketShape);

        // Explosion effect - small red circle at front
        sf::CircleShape explosionTip(5.f);
        explosionTip.setFillColor(sf::Color::Red);
        explosionTip.setPosition(x + 10.f, y - 1.f);
        window.draw(explosionTip);
    }

    ~Rocket()
    {
    }
};