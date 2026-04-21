#pragma once
#include "Player.h"

class Character1 : public Player
{
public:
    Character1(float x, float y) : Player(x, y, "Character1")
    {
        lives = 2;
        speed = 4.5f;           // Faster than Nick
        snowballPower = 1.0f;
        snowballDistance = 300.0f;
    }

    void movementsUpdate() override
    {
        baseMovement();
    }

    void displayPlayer(sf::RenderWindow& window) override
    {
        // Blinking effect when invincible
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