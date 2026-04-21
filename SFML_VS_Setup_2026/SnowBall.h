#pragma once
#include "Projectile.h"
#include "Enemy.h"
#include <cmath>

class Snowball : public Projectile
{
private:
    int direction;
    float distanceTravelled;
    float maxDistance;
    int snowPower;
    sf::CircleShape shape;

public:
    Snowball(float startX, float startY, int dir, float maxDist, int power, float sWidth, float sHeight)
        : Projectile(startX, startY, sWidth, sHeight)
    {
        direction = dir;
        maxDistance = maxDist;
        snowPower = power;

        velocityX = 6.0f * direction;
        velocityY = 0;

        distanceTravelled = 0;

        // White circle for snowball
        shape.setRadius(15.f);
        shape.setFillColor(sf::Color::White);
        shape.setPosition(x, y);
    }

    void update() override
    {
        if (!active)
        {
            return;
        }

        // Move horizontally
        x = x + velocityX;

        // Track distance travelled
        distanceTravelled = distanceTravelled + abs(velocityX);

        // Deactivate when max distance reached
        if (distanceTravelled >= maxDistance)
        {
            deactivate();
            return;
        }

        // Screen wrap logic
        if (x > screenWidth)
        {
            x = 0;          // Exit right enter left
        }
        else if (x < 0)
        {
            x = screenWidth; // Exit left enter right
        }

        shape.setPosition(x, y);
    }

    void draw(sf::RenderWindow& window) override
    {
        if (!active)
        {
            return;
        }

        window.draw(shape);
    }

    // Called when snowball hits enemy
    void onHitEnemy(Enemy* enemy)
    {
        for (int i = 0; i < snowPower; i++)
        {
            enemy->reduceHealth();
        }
        deactivate();
    }

    sf::FloatRect getHitBox()
    {
        return shape.getGlobalBounds();
    }

    ~Snowball()
    {
    }
};