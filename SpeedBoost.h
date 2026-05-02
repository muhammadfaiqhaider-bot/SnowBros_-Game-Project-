#pragma once
#include "PowerUps.h"
#include "PLayer.h"

class SpeedBoost : public PowerUps
{
public:
    SpeedBoost(float posX, float posY) : PowerUps(posX, posY, "SpeedBoost")    // Take x, y positions as spawning point......
    {
        isActive = true;
    }

    void updatePowerUp() override
    {
        // Apply base gravity so spawned power-ups fall to ground
        PowerUps::updatePowerUp();
    }

    void applyEffect(Player& player) override
    {
        player.applySpeedBoost();                                   // In player class logic for speeding is already present so it will happen there
    }                                                               // All calculations is there.......

    void DrawPowerUp(sf::RenderWindow& window) override
    {
        if (isActive && !isCollected)
        {
            // Yellow circle - speed boost
            sf::CircleShape shape(15.0f);
            shape.setFillColor(sf::Color::Yellow);
            shape.setPosition(x, y);
            window.draw(shape);
        }
    }
};