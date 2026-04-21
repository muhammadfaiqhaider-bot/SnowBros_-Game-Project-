#pragma once
#include "Player.h"

class Character2 : public Player
{
public:
    Character2(float x, float y) : Player(x, y, "Character2")
    {
        lives = 2;
        speed = 3.0f;
        snowballPower = 1.0f;
        snowballDistance = 450.0f;  // Longer range than Nick
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

        // Yellow rectangle for Character2
        sf::RectangleShape charShape(sf::Vector2f(40.f, 40.f));
        charShape.setFillColor(sf::Color::Yellow);
        charShape.setPosition(x, y);
        window.draw(charShape);
    }
};