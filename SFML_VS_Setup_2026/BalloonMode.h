#pragma once
#include "PowerUps.h"
#include "PLayer.h"

class BalloonMode : public PowerUps
{
public:
    BalloonMode(float posX, float posY) : PowerUps(posX, posY, "BalloonMode")      // Take x, y positions as spawning point......
    {
        isActive = true;
    }

    void updatePowerUp() override
    {
        // No update logic needed for now
    }

    void applyEffect(Player& player) override
    {
        player.applyBalloonMode();                                  // In player class balloon mode logic is already present
    }                                                               // All calculations is there.......

    void DrawPowerUp(sf::RenderWindow& window) override
    {
        if (isActive && !isCollected)
        {
            // Pink circle - balloon mode
            sf::CircleShape shape(15.0f);
            shape.setFillColor(sf::Color(255, 105, 180));   // Pink
            shape.setPosition(x, y);
            window.draw(shape);
        }
    }
};