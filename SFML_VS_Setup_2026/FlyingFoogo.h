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
        flyingDuration = 120;          // time=frame/FPS -> which makes this 2 seconds of flight
        walkingTimer = 0;
        walkingDuration = 360;


        name = "FlyingFoogaFoog";      // name Changed in Botom to FlyingFoogaFoog
    }

    void movementsUpdate() override
    {
        // ============================================================
        // CRITICAL FIX: Check dead/encased/rolling FIRST before any movement
        // This prevents flying/walking when snowball should be stationary
        // ============================================================

        // If dead - do nothing
        if (isDead)
        {
            return;
        }

        // If rolling - ONLY do rolling physics (no flying, no walking)
        if (isRolling)
        {
            // Stop flying when rolling
            isFlying = false;
            flyingTimer = 0;
            walkingTimer = 0;

            // Apply gravity while rolling
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

        // If encased (but not rolling yet) - STOP all movement, just fall with gravity
        if (snowCovered)
        {
            // Stop flying when encased
            isFlying = false;
            flyingTimer = 0;
            walkingTimer = 0;
            velocityX = 0;

            // Just apply gravity so it falls to ground
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

        if (isFlying)
        {
            // Flying movement - NO gravity, NO Botom physics
            flyingTimer++;

            x = x + velocityX;
            y = y + velocityY;

            // Screen boundary bounce
            if (x < 0)
            {
                x = 0;
                velocityX = -velocityX;
            }
            if (x > 560)
            {
                x = 560;
                velocityX = -velocityX;
            }
            if (y < 0)
            {
                y = 0;
                velocityY = -velocityY;
            }
            if (y > 520)
            {
                y = 520;
                velocityY = -velocityY;
            }

            if (flyingTimer > flyingDuration)
            {
                isFlying = false;
                flyingTimer = 0;

                // Reset to ground walking speed
                int direction;
                if (rand() % 2 == 0)
                {
                    direction = 1;
                }
                else
                {
                    direction = -1;
                }
                velocityX = 1.3f * direction;
                velocityY = 0;              // Reset Y so gravity works again
            }
        }
        else
        {
            // Ground movement - use Botom physics normally
            Botom::movementsUpdate();

            walkingTimer++;
            if (walkingTimer > walkingDuration)
            {
                isFlying = true;
                walkingTimer = 0;

                int dirX;
                int dirY;

                if (rand() % 2 == 0)
                {
                    dirX = 1;
                }
                else
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

                float speed = 2.5f;         // Increased speed for visible diagonal
                velocityX = speed * dirX;
                velocityY = speed * dirY;
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
        if (isFlying)
        {
            sf::RectangleShape tempShape(sf::Vector2f(30.f, 30.f));
            tempShape.setFillColor(sf::Color::Blue);
            tempShape.setPosition(x, y);
            window.draw(tempShape);                                           // To show a contrast when Flying fooga flies it is BLUE
        }                                                                     // when on ground its Green......
        else
        {
            sf::RectangleShape tempShape(sf::Vector2f(30.f, 30.f));
            tempShape.setFillColor(sf::Color::Green);
            tempShape.setPosition(x, y);
            window.draw(tempShape);
        }

        // Show snow overlay based on hit count (partially hit)
        if (hitCount == 1)
        {
            // Light snow - 25% covered
            sf::CircleShape snowOverlay(15.f);
            snowOverlay.setFillColor(sf::Color(255, 255, 255, 80));
            snowOverlay.setPosition(x, y);
            window.draw(snowOverlay);
        }
        else if (hitCount == 2)
        {
            // Heavy snow - 65% covered
            sf::CircleShape snowOverlay(15.f);
            snowOverlay.setFillColor(sf::Color(255, 255, 255, 160));
            snowOverlay.setPosition(x, y);
            window.draw(snowOverlay);
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

    bool getDeathStatus()
    {
        return isDead;
    }

    virtual ~FlyingFoogaFoog()
    {
    }
};