#pragma once
#include "Projectile.h"

class Knives : public Projectile
{
private:
    sf::Texture knifeTexture;
    sf::Sprite knifeSprite;
    bool knifeTextureLoaded;
public:
    Knives(float startX, float startY, float targetX, float targetY, float limit) : Projectile(startX, startY, limit, limit)
    {
        calculateDirection(targetX, targetY, 4.0f);
                                                            // Use Projectile's Class calculateDirection method
                                                             // speed is passed, as throables are kinda fast so pass slightly high value
        knifeTextureLoaded = knifeTexture.loadFromFile("assets/Tornado_Blue.png");
        if (knifeTextureLoaded)
        {
            knifeSprite.setTexture(knifeTexture);
            knifeSprite.setTextureRect(sf::IntRect(290, 895, 65, 25));
            knifeSprite.setScale(30.f / 65.f, 10.f / 25.f); // scale to match original rectangle size
        }
    
    }                                                 

    void updateTrajactory() override
    {
        if (!active)
        {
            return;
        }

        x = x + velocityX;                         // Update Positions along x-axis
    //    y = y + velocityY;                         // Update Positions along y-axis

        checkScreenBoundary();                     // Check for screen Limit.....Either satisfy resolutionso of game that is 600x600
    }

    void drawProjectiles(sf::RenderWindow& window) override
    {
        if (!active)
        {
            return;
        }

        if (knifeTextureLoaded)
        {
            knifeSprite.setPosition(x, y);
            window.draw(knifeSprite);
        }
        else // added knife texture
        {
            sf::RectangleShape knifeShape(sf::Vector2f(15.f, 5.f));
            knifeShape.setFillColor(sf::Color::Yellow);
            knifeShape.setPosition(x, y);
            window.draw(knifeShape);
        }
    }





    ~Knives()
    {
    }
};