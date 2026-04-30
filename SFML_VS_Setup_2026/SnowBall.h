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
    Snowball(float startX, float startY, int direc, float maximumDist, int power, float sWidth, float sHeight) : Projectile(startX, startY, sWidth, sHeight)
    {
        direction = direc;
        maxDistance = maximumDist;
        snowPower = power;

        velocityX = 6.0f * direction;
        velocityY = 0;

        distanceTravelled = 0;


        shape.setRadius(13.f);
        shape.setFillColor(sf::Color::White);
        shape.setPosition(x, y);
    }

    void updateTrajactory() override
    {
        if (!active)
        {
            return;
        }

        x = x + velocityX;    // As snow ball only moves left right so we just need x -axix...

        distanceTravelled = distanceTravelled + abs(velocityX);  // For traking distance coverage......

        // Deactivate when max distance reached
        if (distanceTravelled >= maxDistance)
        {
            deactivate();
            return;
        }


        if (x > screenWidth)
        {
            x = 0;
            deactivate();
        }
        else if (x < 0)
        {
            x = screenWidth; 
            deactivate();
        }

        shape.setPosition(x, y);
    }

    void drawProjectiles(sf::RenderWindow& window) override
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







    int getDirection()
    {
        return direction;   
    }




    ~Snowball()
    {
    }
};