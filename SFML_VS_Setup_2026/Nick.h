#pragma once
#include "Player.h"

class Nick : public Player
{
public:
    Nick(float x, float y) : Player(x, y, "Nick")
    {
        lives = 5;
        speed = 3.0f;
        snowballPower = 1.0f;
        snowballDistance = 200.0f;
    }

    void movementsUpdate() override
    {                                         // Movements are the same for player mechanics would be same sao whole 
        baseMovement();                       // fucntion is called form the base........
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