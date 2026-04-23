#pragma once
#include "PowerUps.h"
#include "PLayer.h"

class DistanceIncrease : public PowerUps
{
public:
    DistanceIncrease(float posX, float posY) : PowerUps(posX, posY, "DistanceIncrease")    // Take x, y positions as spawning point......
    {
        isActive = true;
    }

    void updatePowerUp() override
    {
        // No update logic needed for now
    }

    void applyEffect(Player& player) override
    {
        player.applyDistanceIncrease();                             // In player class logic for Distance increase of Snowball is already present
    }                                                               // All calculations is there.......

    void DrawPowerUp(sf::RenderWindow& window) override
    {
        if (isActive && !isCollected)
        {
            // Blue circle - distance increase
            sf::CircleShape shape(15.0f);
            shape.setFillColor(sf::Color::Blue);
            shape.setPosition(x, y);
            window.draw(shape);
        }
    }
};