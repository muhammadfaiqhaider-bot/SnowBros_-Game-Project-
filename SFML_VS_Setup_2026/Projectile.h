
#pragma once
#include <SFML/Graphics.hpp>


// This is abstract class for throwable aobjects like knife rockets and 
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
        screenWidth = sWidth;                          // screen width that is 600 to rmain my throwable inside screen
        screenHeight = sHeight;                        
    }

    // Virtual Functions for polimorphism
    virtual void updateTrajactory() = 0;
    virtual void drawProjectiles(sf::RenderWindow& window) = 0;

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
    void checkScreenBoundary()
    {
        if (x < 0 || x > screenWidth || y < 0 || y > screenHeight)
        {
            active = false;
        }
    }

    // Normalize direction toward target same math for all projectiles
    void calculateDirection(float targetX, float targetY, float speed)
    {
        float diffX = targetX - x;                               // Calculate direction toward player position..x-axis
        float diffY = targetY - y;                               // Calculate direction toward player position..y-axis

        float distance = sqrt(diffX * diffX + diffY * diffY);     // s=(x^2 +y^2)^1/2
                                                            

        if (distance == 0)                                      // Avoid division by zero error.....
        {
            distance = 1;
        }

        
        velocityX = (diffX / distance) * speed;               // Normalise Speed means : Get speed value for x and y axis so that 
        velocityY = (diffY / distance) * speed;               // speed should be according to frames we have,,......
    }

    virtual ~Projectile()
    {
    }
};