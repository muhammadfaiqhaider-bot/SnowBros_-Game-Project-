
#pragma once
#include <SFML/Graphics.hpp>

class Projectile
{
protected:
    float x;
    float y;
    float velocityX;
    float velocityY;
    bool active;
    float screenWidth;
    float screenHeight;

public:
    Projectile(float startX, float startY, float sWidth, float sHeight)
    {
        x = startX;
        y = startY;
        velocityX = 0;
        velocityY = 0;
        active = true;
        screenWidth = sWidth;
        screenHeight = sHeight;
    }

    // Pure virtual - every projectile must implement these
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // Common to all projectiles
    bool isActive()
    {
        return active;
    }

    void deactivate()
    {
        active = false;
    }

    float getX()
    {
        return x;
    }

    float getY()
    {
        return y;
    }

    // Check screen boundary - same logic for all projectiles
    void checkScreenBoundary()
    {
        if (x < 0 || x > screenWidth || y < 0 || y > screenHeight)
        {
            active = false;
        }
    }

    // Normalize direction toward target - same math for all projectiles
    void calculateDirection(float targetX, float targetY, float speed)
    {
        float diffX = targetX - x;           // Calculate direction toward player position..x-axis
        float diffY = targetY - y;           // Calculate direction toward player position..y-axis

        float distance = sqrt(diffX * diffX + diffY * diffY);  // Using Distance Formula..
        // s=(x^2 +y^2)^1/2

        if (distance == 0)                    // Avoid division by zero error.....
        {
            distance = 1;
        }

        // Normalize direction and set speed
        velocityX = (diffX / distance) * speed;
        velocityY = (diffY / distance) * speed;
    }

    virtual ~Projectile()
    {
    }
};