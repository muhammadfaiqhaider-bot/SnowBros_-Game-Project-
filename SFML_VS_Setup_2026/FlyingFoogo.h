#pragma once
#include "Botom.h"
#include <cstdlib>

class FlyingFoogaFoog : public Botom
{
private:
    bool isFlying;
    int flyingTimer;
    int flyingDuration;
    int walkingTimer;
    int walkingDuration;

public:
    FlyingFoogaFoog(float xPos, float yPos) : Botom(xPos, yPos)
    {
        isFlying = false;
        flyingTimer = 0;
        flyingDuration = 120;          // time=frame/FPS -> which mkes this 2 seconds of flight
        walkingTimer = 0;
        walkingDuration = 360;


        name = "FlyingFoogaFoog";      //name Changed in Botom to FlyingFoogaFoog
    }

    void movementsUpdate() override
    {
        if (isFlying)
        {
            x = x + velocityX;         // Apply flying movement
            y = y + velocityY;


            if (x < 0)                 // Screen boundary check during flight
            {
                x = 0;
                velocityX = -velocityX;
            }

            if (x > 560)
            {
                x = 560;
                velocityX = -velocityX;
            }

            if (y < 10)
            {
                y = 10;
                velocityY = -velocityY;
            }

            if (y > 560)
            {
                y = 560;
                velocityY = -velocityY;
            }

            flyingTimer++;


            if (flyingTimer > flyingDuration)
            {
                isFlying = false;
                flyingTimer = 0;


                int direction;
                if (rand() % 2 == 0)     // Now lands to the ground and attain its old speed.....
                {
                    direction = 1;          // This is the same Botom ground moving Logic
                }
                else
                {
                    direction = -1;
                }
                velocityX = 2.0f * direction;
                velocityY = 0;
            }
        }
        else
        {

            Botom::movementsUpdate();        //Inherite all moments from Botom + additional features added


            walkingTimer++;
            if (walkingTimer > walkingDuration)
            {
                isFlying = true;
                walkingTimer = 0;


                int dirX;                    //Now as walk time is over so Now i have to assign flying Directions
                int dirY;

                if (rand() % 2 == 0)
                {
                    dirX = 1;                    //         \ | /
                }                                //        <- . ->         These are the possible direction where flyings happens
                else                             //         / | \

                {
                    dirX = -1;
                }

                if (rand() % 2 == 0)
                {
                    dirY = 1;
                }
                else
                {
                    dirY = -1;
                }


                float speed = 1.8f;
                velocityX = speed * dirX;            // Velocity = 1.8 * direction X ..
                velocityY = speed * dirY;
            }
        }
    }



    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (isFlying)
        {
            sf::RectangleShape tempShape(sf::Vector2f(30.f, 30.f));
            tempShape.setFillColor(sf::Color::Blue);
            tempShape.setPosition(x, y);
            window.draw(tempShape);                                           // To show a contrast when Flying fooga flies it is BLUE
        }                                                                     //when on ground its Green......
        else
        {
            sf::RectangleShape tempShape(sf::Vector2f(30.f, 30.f));
            tempShape.setFillColor(sf::Color::Green);
            tempShape.setPosition(x, y);
            window.draw(tempShape);
        }
    }

    bool getIsFlying()
    {
        return isFlying;
    }

    bool getEncaedStatusFlying()
    {
        return snowCovered;
    }

    virtual ~FlyingFoogaFoog()
    {
    }
};