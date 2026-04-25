#pragma once
#include "Player.h"

class Cheema : public Player
{
public:
    Cheema(float x, float y) : Player(x, y, "Character2")
    {
        lives = 5;
        speed = 3.0f;
        snowballPower = 1.0f;
        snowballDistance = 400.0f;  // Long range fot Cheema snow ball.........
    }

    void movementsUpdate() override
    {
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

        // Yellow rectangle for Character2
        sf::RectangleShape charShape(sf::Vector2f(40.f, 40.f));
        charShape.setFillColor(sf::Color::Yellow);
        charShape.setPosition(x, y);
        window.draw(charShape);
    }
};