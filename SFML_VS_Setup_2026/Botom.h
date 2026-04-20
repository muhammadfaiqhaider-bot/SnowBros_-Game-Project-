#pragma once
#include "Enemy.h"
#include <cstdlib>

class Botom : public Enemy
{
protected:
    bool hitWall;
    bool onGround;

public:
    Botom(float xPos, float yPos) : Enemy(xPos, yPos, "Botom")
    {
        health = 1;
        hitWall = false;
        onGround = false;


        int direction;
        if (rand() % 2 == 0)
        {
            direction = 1;
        } 
        else
        {
            direction = -1;
        }
        velocityX = 2.0 * direction;
        velocityY = 0;
    }

    void movementsUpdate() override
    {

        if (hitWall)
        {
            velocityX = -velocityX;            // Collision Logic if I collide with wall Botom will change its direction.....
            hitWall = false;
        }

        
        if (!onGround)                         // Gravity logic If not on ground so pull's down until touches the ground.....
        {
            velocityY = velocityY + 0.5f;      // Gravity Applied
        }
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
    {                                        // Cheema use this  when Botom lands on flat surfa
        onGround = value;
    }

    virtual ~Botom()
    {
    }
};