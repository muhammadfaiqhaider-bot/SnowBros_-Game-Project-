#pragma once
#include "Player.h"

class Faiq : public Player
{
public:
    Faiq(float x, float y) : Player(x, y, "Character1")
    {
        lives = 4;              // 1 Life Reduced
        speed = 4.7f;           // Faster than Nickk (speed increased)
        snowballPower = 1.0f;
        snowballDistance = 200.0f;
    }

    void movementsUpdate() override
    {
        // Baki he has all the same movements.......
        baseMovement();
    }

    void displayPlayer(sf::RenderWindow& window) override
    {
        if (isInvincible)
        {
            if (invincibleTimer % 10 < 5)
            {
                return;
            }
        }

        // Red rectangle for Character1
        sf::RectangleShape charShape(sf::Vector2f(40.f, 40.f));
        charShape.setFillColor(sf::Color::Red);
        charShape.setPosition(x, y);
        window.draw(charShape);
    }
};