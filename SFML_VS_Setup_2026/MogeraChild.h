#pragma once
#include "Enemy.h"

class MogeraChild : public Enemy
{
private:
    bool active;
    bool onGround;
    bool killedBySnowball;

public:
    MogeraChild() : Enemy(0, 0, "MogeraChild")
    {
        active = false;
        onGround = false;
        velocityX = 0;
        velocityY = 0;
        killedBySnowball = false;

        // Sprite sheet setup for MogeraChild (single frame - 135x153)
        // Only 1 frame so no animation switching needed, just load and draw
        frameWidth = 135;
        frameHeight = 153;
        totalFrames = 1;
        currentFrame = 0;
        animTimer = 0;
        animSpeed = 1;      // Doesnt matter bcz only 1 frame anyway

        if (enemyTexture.loadFromFile("assets/Mogera.png"))
        {
            textureLoaded = true;
            enemySprite.setTexture(enemyTexture);
            enemySprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
            // Scale to 20x20 to match the original orange rectangle size
            float sx = 20.f / (float)frameWidth;
            float sy = 20.f / (float)frameHeight;
            enemySprite.setScale(sx, sy);
        }
    }

    void spawn(float startX, float startY, float playerX)
    {
        x = startX;                                                    // This is the Important function to realease Child Mogera's in the the direction 
        y = startY;                                                    // where our person is and set velocity according to it 
        active = true;
        onGround = false;

        if (playerX > x)
        {
            velocityX = 1.3f;
        }
        else
        {
            velocityX = -1.3f;
        }

        velocityY = 0;
    }

    void movementsUpdate() override
    {
        if (!active)
        {
            return;
        }

        if (!onGround)
        {
            velocityY = velocityY + 0.5f;
        }
        else
        {
            velocityY = 0;
        }

        x = x + velocityX;
        y = y + velocityY;

        // Turn around at walls instead of deactivating
        if (x < 0)
        {
            x = 0;
            velocityX = -velocityX;     // Reverse direction
        }

        if (x > 560)
        {
            x = 560;
            velocityX = -velocityX;     // Reverse direction
        }

        // Stop at ground level
        if (y > 520)
        {
            y = 520;
            onGround = true;
        }

        // Update sprite position every frame (single frame so no rect switching needed)
        if (textureLoaded)
        {
            enemySprite.setPosition(x, y);
        }
    }

    // Called when hit by a snowball - child dies immediately
    void instantKill() override
    {
        active = false;
        killedBySnowball = true;
    }

    // Check collision with player - deactivate and signal hit
    bool checkPlayerCollision(float playerX, float playerY)
    {
        if (!active) return false;
        sf::FloatRect childBounds(x, y, 20.f, 20.f);
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);
        if (childBounds.intersects(playerBounds))
        {
            active = false;
            return true;
        }
        return false;
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (!active)
        {
            return;
        }

        if (textureLoaded)
        {
            window.draw(enemySprite);
        }
        else
        {
            sf::RectangleShape childShape(sf::Vector2f(20.f, 20.f));   // Orange Rectangle is used to denote Child Mogera's 
            childShape.setFillColor(sf::Color(255, 165, 0));
            childShape.setPosition(x, y);
            window.draw(childShape);
        }
    }

    bool isActive()
    {
        return active;
    }

    void setOnGround(bool value)
    {
        onGround = value;
    }

    void snapToGround(float groundY)
    {
        y = groundY;
        velocityY = 0;
        onGround = true;
    }


    void deactivate()
    {
        active = false;
    }

    float getVelocityY()
    {
        return velocityY;
    }

    virtual ~MogeraChild()
    {
    }
};