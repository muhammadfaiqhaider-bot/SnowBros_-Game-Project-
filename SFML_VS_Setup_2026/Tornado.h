#pragma once
#include "FlyingFoogo.h"
#include "Knife.h"
#include <cstdlib>



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
        teleportDuration = 300;     // time = Frames/FPS

        attackTimer = 0;
        attackDuration = 240;     // Throw knife after every 3 seconds......

        playerX = 0;
        playerY = 0;

        churi = nullptr;

        name = "Tornado";
    }

    void setPlayerPosition(float pX, float pY)
    {                           // Cheema Use this function for updating player position in every
        playerX = pX;           // Frame.......bcz churi will sent according to it 
        playerY = pY;
    }

    void movementsUpdate() override
    {
        teleportTimer++;

        if (isTeleporting)
        {
            // Apply gravity while waiting to teleport
            if (!onGround)
            {
                velocityY = velocityY + 0.5f;
            }
            else
            {
                velocityY = 0;
            }

            y = y + velocityY;

            // Keep inside screen while waiting
            if (y > 520)
            {
                y = 520;
            }
            if (y < 0)
            {
                y = 0;
            }

            if (teleportTimer >= teleportDuration)             // Teleport Logic
            {
                float newX = x + (rand() % 400) - 200;         // X coordinate teleportation Point
                float newY = y + (rand() % 400) - 200;         // Y coordinate telepotation Point

                

                if (newX < 0)
                {
                    newX = 0;
                }
                if (newX > 560)
                {
                    newX = 560;                             // Checker for teleportation points
                }                                           // So that telepotation point won't
                                                            // come out of the screen
                if (newY < 15)
                {
                    newY = 15;
                }
                if (newY > 520)                             // Fixed - was 560 before allowing
                {                                           // tornado to go below ground
                    newY = 520;
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

            if (teleportTimer > 300)
            {
                if (rand() % 6 == 0)
                {
                    isTeleporting = true;
                    teleportTimer = 0;
                }
            }
        }

        // Knife Throwing Thing
        attackTimer++;

        if (attackTimer >= attackDuration)
        {
            attackTimer = 0;

            if (churi == nullptr)
            {
                // Only throw knife if player position is valid
                // playerY must be different from tornado Y for diagonal throw
                churi = new Knives(x, y, playerX, playerY, 600.0f);   // Calling Knife class for creating an
            }                                                           // an Object.......
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