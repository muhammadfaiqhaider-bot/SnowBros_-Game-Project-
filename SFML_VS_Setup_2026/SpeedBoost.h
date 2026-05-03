#pragma once
#include "PowerUps.h"
#include "PLayer.h"

class SpeedBoost : public PowerUps
{
public:
    SpeedBoost(float posX, float posY) : PowerUps(posX, posY, "SpeedBoost")    // Take x, y positions as spawning point......
    {
        isActive = true;
		powerupTexture.loadFromFile("assets/SpeedBoost.png");
		powerupSprite.setTexture(powerupTexture);
		powerupSprite.setScale(30.f / powerupTexture.getSize().x, 30.f / powerupTexture.getSize().y); // scale to 30x30 pixels
		powerupSprite.setPosition(x, y);

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

            window.draw(powerupSprite);
        }
    }
};