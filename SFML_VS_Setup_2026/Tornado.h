#pragma once
#include "FlyingFoogo.h"
#include "Knife.h"
#include <cstdlib>



class Tornado : public FlyingFoogaFoog
{
private:
    bool isTeleporting;        // Special Feature can teleport 
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
        attackDuration = 240;     // Throw knife after every 4 seconds......

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
        // ============================================================
        // CRITICAL FIX: Check dead/encased/rolling FIRST before any movement
        // This prevents teleporting/attacking when snowball should be stationary
        // ============================================================

        // If dead - do nothing and clean up knife
        if (isDead)
        {
            // Stop all attacks when dead
            if (churi != nullptr)
            {
                delete churi;
                churi = nullptr;
            }
            return;
        }

        // If rolling - ONLY do rolling physics (no teleporting, no attacks)
        if (isRolling)
        {
            // Stop teleporting when rolling
            isTeleporting = false;
            teleportTimer = 0;
            attackTimer = 0;

            // Delete knife if exists
            if (churi != nullptr)
            {
                delete churi;
                churi = nullptr;
            }

            // Apply rolling physics (same as Botom)
            if (!onGround)
            {
                velocityY = velocityY + 1.4f;
                if (velocityY > 8.0f)
                {
                    velocityY = 8.0f;
                }
            }
            else
            {
                velocityY = 0;
            }

            // Move rolling snowball
            x = x + rollVelocityX;
            y = y + velocityY;

            // change dir on colliding upon wall.......
            if (x <= 0)
            {
                x = 0;
                rollVelocityX = -rollVelocityX;
            }
            if (x >= 560)
            {
                x = 560;
                rollVelocityX = -rollVelocityX;
            }

            // Count down roll timer
            rollTimer++;
            if (rollTimer >= rollDuration)
            {
                isDead = true;      // Vanish after 3 seconds
            }

            return;     // Exit early - don't do normal movement
        }

        // If encased (but not rolling yet) - STOP all movement
        if (snowCovered)
        {
            // Stop everything when encased
            isTeleporting = false;
            teleportTimer = 0;
            attackTimer = 0;
            velocityX = 0;

            // Delete knife if exists
            if (churi != nullptr)
            {
                delete churi;
                churi = nullptr;
            }

            // Just fall to ground
            if (!onGround)
            {
                velocityY = velocityY + 0.5f;
                if (velocityY > 8.f)
                {
                    velocityY = 8.f;
                }
                y = y + velocityY;
            }
            else
            {
                velocityY = 0;
            }

            return;     // Exit early - stationary snowball
        }

        // ============================================================
        // NORMAL MOVEMENT (only runs if NOT dead/rolling/encased)
        // ============================================================

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
                velocityY = 0;      // Reset velocity after teleport
            }
        }
        else
        {
            FlyingFoogaFoog::movementsUpdate();      // This is the same inheritated movements by FlyingFooga
            // When Not teleporting

            if (teleportTimer > 300)
            {
                if (rand() % 3 == 0)
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
                churi = new Knives(x, y, playerX, playerY, 600.0f);        // Calling Knife class for creating an
            }                                                              // an Object.......
        }

        if (churi != nullptr)
        {
            churi->updateTrajactory();

            if (!churi->isActive())
            {
                delete churi;
                churi = nullptr;
            }
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        // Don't draw if dead
        if (isDead)
        {
            return;
        }

        // ROLLING SNOWBALL (kicked and moving)
        if (isRolling)
        {
            sf::CircleShape rollingBall(22.0f);
            rollingBall.setFillColor(sf::Color::White);
            rollingBall.setOutlineColor(sf::Color(180, 220, 255));
            rollingBall.setOutlineThickness(3.0f);
            rollingBall.setPosition(x, y);
            window.draw(rollingBall);
            return;
        }

        // FULLY ENCASED (stationary) 
        if (snowCovered)
        {
            sf::CircleShape snowBall(20.0f);
            snowBall.setFillColor(sf::Color::White);
            snowBall.setOutlineColor(sf::Color(180, 220, 255));
            snowBall.setOutlineThickness(2.0f);
            snowBall.setPosition(x, y);
            window.draw(snowBall);

            // Small indicator showing its kickable
            sf::RectangleShape leftArrow(sf::Vector2f(8.0f, 3.0f));
            leftArrow.setFillColor(sf::Color::Cyan);
            leftArrow.setPosition(x - 12.0f, y + 18.0f);
            window.draw(leftArrow);

            sf::RectangleShape rightArrow(sf::Vector2f(8.0f, 3.0f));
            rightArrow.setFillColor(sf::Color::Cyan);
            rightArrow.setPosition(x + 44.0f, y + 18.0f);
            window.draw(rightArrow);

            return;
        }

        // NORMAL DISPLAY (alive, not encased)
        // Tornado is cyan colored rectangle
        sf::RectangleShape tornadoShape(sf::Vector2f(40.f, 40.f));
        tornadoShape.setFillColor(sf::Color::Cyan);
        tornadoShape.setPosition(x, y);
        window.draw(tornadoShape);

        // Show snow overlay based on hit count (partially hit)
        if (hitCount == 1)
        {
            // Light snow - 25% covered
            sf::CircleShape snowOverlay(20.f);
            snowOverlay.setFillColor(sf::Color(255, 255, 255, 80));
            snowOverlay.setPosition(x, y);
            window.draw(snowOverlay);
        }
        else if (hitCount == 2)
        {
            // Heavy snow - 65% covered
            sf::CircleShape snowOverlay(20.f);
            snowOverlay.setFillColor(sf::Color(255, 255, 255, 160));
            snowOverlay.setPosition(x, y);
            window.draw(snowOverlay);
        }

        // Draw Knives Sprite shyt implementation
        if (churi != nullptr)
        {
            churi->drawProjectiles(window);
        }
    }



    bool isKnifeHittingPlayer(float playerX, float playerY)
    {
        if (churi == nullptr || !churi->isActive())
        {
            return false;
        }

        sf::FloatRect knifeBounds(
            churi->getX(),
            churi->getY(),
            15.f, 5.f
        );

        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

        if (knifeBounds.intersects(playerBounds))
        {
            // Deactivate knife after hit
            churi->deactivate();
            return true;
        }

        return false;
    }


    
    bool getEncaedStatusTor()
    {
        return snowCovered;
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