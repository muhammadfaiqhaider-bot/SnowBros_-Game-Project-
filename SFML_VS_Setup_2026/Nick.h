#pragma once
#include "Player.h"

class Nick : public Player
{
public:
    Nick(float x, float y) : Player(x, y, "Nick")
    {
        lives = 2;
        speed = 3.0f;
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

        // Blue rectangle for Nick
        sf::RectangleShape nickShape(sf::Vector2f(40.f, 40.f));
        nickShape.setFillColor(sf::Color::Blue);
        nickShape.setPosition(x, y);
        window.draw(nickShape);
    }
};