#pragma once
#include "FlyingFoogo.h"
#include <cstdlib>

class Knives
{
private:
    float x;
    float y;
    float velocityX;
    float velocityY;
    bool inAir;
    float screenLimit;

public:
    Knives(float startX, float startY, float targetX, float targetY, float limit)
    {
        x = startX;
        y = startY;
        inAir = true;
        screenLimit = limit;

        
        float diffX = targetX - startX;           // Calculate direction toward player position..x-axis
        float diffY = targetY - startY;           // Calculate direction toward player position..y-axis

        
        float distance = sqrt(diffX * diffX + diffY * diffY);  // Using Distance Formula..
                                                  // s=(x^2 +y^2)^1/2

        
        if (distance == 0)                        // Avoid division by zero error.....
        {
            distance = 1;
        }

        // Normalize direction and set speed to 4
        velocityX = (diffX / distance) * 4.0f;
        velocityY = (diffY / distance) * 4.0f;
    }

    void update()
    {
        if (inAir)
        {
            x = x + velocityX;
            y = y + velocityY;
        }

        
        if (x < 0 || x > screenLimit || y < 0 || y > screenLimit)// Check for screen Limit.....
        {
            inAir = false;
        }
    }

    bool isActive()
    {
        return inAir;
    }

    float getX()
    {
        return x;
    }

    float getY()
    {
        return y;
    }

    void deactivate()
    {
        inAir = false;
    }

    void draw(sf::RenderWindow& window)
    {
        if (inAir)
        {
            // Yellow small rectangle for knife
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


class Tornado : public FlyingFoogaFoog
{
private:
    bool isTeleporting;
    int teleportTimer;
    int teleportDuration;
    int attackTimer;
    int attackDuration;
    float playerX;         // This variable is use to store Player x-axis position for knife throwing..
    float playerY;
    Knives* churi;

public:
    Tornado(float posX, float posY) : FlyingFoogaFoog(posX, posY)
    {
        isTeleporting = false;
        teleportTimer = 0;
        teleportDuration = 60;     // time = Frames/FPS -

        attackTimer = 0;
        attackDuration = 180;     // Throw knife after every 3 seconds......

        playerX = 0;
        playerY = 0;

        churi = nullptr;

        name = "Tornado";
    }

    
    void setPlayerPosition(float pX, float pY)
    {                           // Cheema Use this function for updating player position in every 
        playerX = pX;           // Frame.......
        playerY = pY;
    }





    void movementsUpdate() override
    {
      
        teleportTimer++;

        if (isTeleporting)
        {
            if (teleportTimer >= teleportDuration)             // Teleport Logic
            {
                
                float newX = x + (rand() % 200) - 100;         // X coordinate teleportation Point
                float newY = y + (rand() % 200) - 100;         // Y coordinate telepotation Point

            
                if (newX < 0)
                {
                    newX = 0;
                }
                if (newX > 560)
                {
                    newX = 560;                             // Checker for teleportation points
                }                                           // So that telepotation point won't 
                                                            // come out of the screen
                if (newY < 0)
                {
                    newY = 0;
                }
                if (newY > 560)
                {
                    newY = 560;
                }

                x = newX;
                y = newY;

                isTeleporting = false;
                teleportTimer = 0;
            }
        }
        else
        {
           
            FlyingFoogaFoog::movementsUpdate();      // This is the same inheritated movements by FlyingFooga
                                                     // When Not teleporting
            


            if (teleportTimer > 60)
            {
                if (rand() % 4 == 0)
                {
                    isTeleporting = true;
                    teleportTimer = 0;
                }
            }
        }

        


        // Knife Thrwing Thing

        attackTimer++;

        if (attackTimer >= attackDuration)
        {
            attackTimer = 0;

            
            if (churi == nullptr)
            {
                churi = new Knives(x, y, playerX, playerY, 600.0f);   // Calling Knife class for creating an
            }                                                         // an Object.......
        }


        if (churi != nullptr)
        {
            churi->update();

            if (!churi->isActive())
            {
                delete churi;
                churi = nullptr;
            }
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        // Tornado is cyan colored rectangle
        sf::RectangleShape tornadoShape(sf::Vector2f(40.f, 40.f));
        tornadoShape.setFillColor(sf::Color::Cyan);
        tornadoShape.setPosition(x, y);
        window.draw(tornadoShape);

        // Draw Knives Sprite shyt implementation
        if (churi != nullptr)
        {
            churi->draw(window);
        }
    }

    ~Tornado()
    {
        if (churi != nullptr)
        {
            delete churi;
            churi = nullptr;
        }
    }
};