#pragma once
#include "Enemy.h"
#include <cstdlib>

class Botom : public Enemy
{
protected:
    bool hitWall;
    bool onGround;

public:
    Botom(float xPos, float yPos) : Enemy(xPos, yPos, "Botom")   // This is parametrized Function whenever in main my Botom is created it 
    {                                                            // Does demand its X and Y coordinate for object creation 
        
        health = 1;
        hitWall = false;
        onGround = false;
        int direction;                              
        if (rand() % 2 == 0)                                 // Randomly direction Assigned 1 for right 
        {                                                    // -1 for left...........
            direction = 1;
        } 
        else
        {
            direction = -1;
        }
        velocityX = 1.3 * direction;                    // Velocity is setted to 1.3 by testing...Some physics applied here like my game is setted
        velocityY = 0;                                  // to 60 frames and direction is either 1 or -1 so  (1x1.3x60) that much pixels are moved along 
    }                                                   // X-axis......




    void movementsUpdate() override // Ovverriden Function........
    {

        if (hitWall)
        {
            velocityX = -velocityX;            // Collision Logic if I collide with wall Botom will change its direction.....
            hitWall = false;
        }

        
        if (!onGround)                         // Gravity logic If not on ground so pull's down until touches the ground.....
        {
            velocityY = velocityY + 0.5f;      // Gravity Applied here +0.5 is bcz scrren resolutions are not like normal graphs it works a bit 
        }                                      // Different...top left is (0,0) and +y means u move downwards...
        else
        {
            velocityY = 0;                     // Stop falling when on ground
        }



        
        x = x +  velocityX;                    // Apply movement along x axis
        y = y +  velocityY;                    // Apply movement along y axis

        
        if (x < 0)                             // Screen boundary check here MY screen Resolution is 600x600
        {                                      // So i'll try to merge things inside it ........
            x = 0;
            hitWall = true;
        }

        if (x > 560)                            // 560 bcz to remain things in game screen...
        {
            x = 560;
            hitWall = true;
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        sf::RectangleShape tempShape(sf::Vector2f(40.f, 40.f));
        tempShape.setFillColor(sf::Color::Red);
        tempShape.setPosition(x, y);
        window.draw(tempShape);
    }


    


    
    void setHitWall(bool value)              // Cheema  calls this when Botom hits a wall or some edges.......
    {
        hitWall = value;
    }

    
    void setOnGround(bool value)    
    {                                        // Cheema use this  when Botom lands on flat surfaxe
        onGround = value;
    }

    virtual ~Botom()
    {
    }
};