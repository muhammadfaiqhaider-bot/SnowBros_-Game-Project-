#pragma once
#include "PowerUps.h"
#include "PLayer.h"

class DistanceIncrease : public PowerUps
{
public:
    DistanceIncrease(float posX, float posY) : PowerUps(posX, posY, "DistanceIncrease")    // Take x, y positions as spawning point......
    {
        isActive = true;
        powerupTexture.loadFromFile("assets/DistanceIncrease.png");
		powerupSprite.setTexture(powerupTexture);
        powerupSprite.setScale(30.f / powerupTexture.getSize().x, 30.f / powerupTexture.getSize().y);
        powerupSprite.setPosition(x, y);

    }

    void updatePowerUp() override
    {
        // Apply gravity so it falls to the ground
        PowerUps::updatePowerUp();
    }

    void applyEffect(Player& player) override
    {
        player.applyDistanceIncrease();                             // In player class logic for Distance increase of Snowball is already present
    }                                                               // All calculations is there.......

    void DrawPowerUp(sf::RenderWindow& window) override
    {
        if (isActive && !isCollected)
        {

            window.draw(powerupSprite);
        }
    }
};