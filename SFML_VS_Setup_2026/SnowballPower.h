#pragma once
#include "PowerUps.h"
#include "PLayer.h"

class SnowballPower : public PowerUps
{
public:
    SnowballPower(float posX, float posY) : PowerUps(posX, posY, "SnowballPower")   // Take x, y positions as spawning point......
    {
        isActive = true;
    }

    void updatePowerUp() override
    {
        // No update logic needed for now
    }

    void applyEffect(Player& player) override
    {
        player.applySnowballPower();                                // In player class snowball power logic is already present
    }                                                               // All calculations is there.......

    void DrawPowerUp(sf::RenderWindow& window) override
    {
        if (isActive && !isCollected)
        {
            // White circle - snowball power
            sf::CircleShape shape(15.0f);
            shape.setFillColor(sf::Color::White);
            shape.setPosition(x, y);
            window.draw(shape);
        }
    }
};