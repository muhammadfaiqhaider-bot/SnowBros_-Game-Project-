#pragma once
#include "PowerUps.h"
#include "PLayer.h"

class SnowballPower : public PowerUps
{
public:
    SnowballPower(float posX, float posY) : PowerUps(posX, posY, "SnowballPower")   // Take x, y positions as spawning point......
    {
        isActive = true;
		powerupTexture.loadFromFile("assets/SnowballPower.png"); // Load the texture for the snowball power-up
        powerupSprite.setTexture(powerupTexture);
        powerupSprite.setScale(30.f / powerupTexture.getSize().x, 30.f / powerupTexture.getSize().y); // Scale to desired size
		powerupSprite.setPosition(x, y);

    }

    void updatePowerUp() override
    {
        // No use here bcz in player class there is already function for increaseing speed so we ll just simply use that........
    }

    void applyEffect(Player& player) override
    {
        player.applySnowballPower();                                // In player class snowball power logic is already present
    }                                                               // All calculations is there.......

    void DrawPowerUp(sf::RenderWindow& window) override
    {
        if (isActive && !isCollected)
        {   
			window.draw(powerupSprite);

        }
    }
};