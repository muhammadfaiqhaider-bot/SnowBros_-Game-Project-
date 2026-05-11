#pragma once
#include "Botom.h"
#include <cstdlib>

class FlyingFoogaFoog : public Botom
{
protected:
    bool isFlying;
    int flyingTimer;
    int flyingDuration;
    int walkingTimer;
    int walkingDuration;


    sf::Texture foogaTexture;
    sf::Sprite foogaSprite;
    bool foogaTextureLoaded;
    int foogaAnimFrame;   // 0 or 1 for flying cycle
    int foogaAnimTimer;
    static const int FOOGA_ANIM_SPEED = 8;

    // Frame 1 - idle/walk
    static const int WALK_X = 56;
    static const int WALK_Y = 30;
    static const int WALK_W = 90;
    static const int WALK_H = 155;

    // Frame 2 - flying pose 1
    static const int FLY1_X = 223;
    static const int FLY1_Y = 17;
    static const int FLY1_W = 152;
    static const int FLY1_H = 172;

    // Frame 3 - flying pose 2
    static const int FLY2_X = 431;
    static const int FLY2_Y = 17;
    static const int FLY2_W = 152;
    static const int FLY2_H = 172;

public:
    FlyingFoogaFoog(float xPos, float yPos) : Botom(xPos, yPos)
    {
        isFlying = false;
        flyingTimer = 0;
        flyingDuration = 120;          // time=frame/FPS -> which makes this 2 seconds of flight
        walkingTimer = 0;
        walkingDuration = 360;


        name = "FlyingFoogaFoog";      // name Changed in Botom to FlyingFoogaFoog
        foogaAnimFrame = 0;
        foogaAnimTimer = 0;
        foogaTextureLoaded = false;

        if (foogaTexture.loadFromFile("assets/FlyingFoogaFoog_Blue.png"))
        {
            foogaTextureLoaded = true;
            foogaSprite.setTexture(foogaTexture);
            foogaSprite.setTextureRect(sf::IntRect(WALK_X, WALK_Y, WALK_W, WALK_H));
            foogaSprite.setScale(40.f / WALK_W, 40.f / WALK_H);
        }
    }

    void movementsUpdate() override
    {


        // If dead  do nothing
        if (isDead)
        {
            return;      // this is the most importanct check to verify first if dead than leave the function .....
        }

        // If rolling  ONLY do rolling physics no flying thingy no walking shooking.....
        if (isRolling)
        {
            // Stop flying when rolling
            isFlying = false;
            flyingTimer = 0;       // timer reset to zeroooo..
            walkingTimer = 0;        // smae walking..

            // Apply gravity while rolling
            if (!onGround) // apply gravity on rolling only when not on lplatform..... encased enemy......so that move to the bottom.....
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

            // change direction  on colliding upon wall.......
            if (x <= 0)
            {
                x = 0;
                rollVelocityX = -rollVelocityX;    // negative negative = positive......
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

            return; 
        }

        // If encased but no rolling started.....stop all left right uper neeche moments........
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

            return; 
        }



        if (isFlying)
        {
            // Flying movement  no gravity no  Botom physics
            flyingTimer++;

            x = x + velocityX;
            y = y + velocityY;

            // Screen boundary checks for verificatinon not move outside the screen........
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

            if (flyingTimer > flyingDuration) // flying timer is like stop watch for flying and duration is the targeted time.....
            {
                isFlying = false;       // flying satus off 
                flyingTimer = 0;

                // give his walking moment backs
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
            // Ground movement  use Botom physics normally ....... polimorphism happens here.........
            Botom::movementsUpdate();

            walkingTimer++;
            if (walkingTimer > walkingDuration)// there is walking timer same like flying timer......
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

                float speed = 2.5f;  // flying speed is setted thora sa fast......
                velocityX = speed * dirX;
                velocityY = speed * dirY;
            }
        }
    }



    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (isDead)
        {
            // Death wali sprite sheet dalne ka no time.........
            return;
        }

        // rolling snowball...... (kicked and moving)
        if (isRolling)
        {
            sf::CircleShape rollingBall(15.0f);
            rollingBall.setFillColor(sf::Color::White);
            rollingBall.setOutlineColor(sf::Color(180, 220, 255));
            rollingBall.setPosition(x, y);
            window.draw(rollingBall);
            return;
        }

        // dully encased...... (stationary) 
        if (snowCovered)
        {
            sf::CircleShape snowBall(15.0f);
            snowBall.setFillColor(sf::Color::White);
            snowBall.setOutlineColor(sf::Color(180, 220, 255));
            snowBall.setPosition(x, y);
            window.draw(snowBall);

            return;
        }


        if (!foogaTextureLoaded)
        {
            sf::RectangleShape temp(sf::Vector2f(30.f, 30.f));
            temp.setFillColor(isFlying ? sf::Color::Blue : sf::Color::Green);
            temp.setPosition(x, y);
            window.draw(temp);
            return;
        }

        //  animation thin
        if (isFlying)
        {
            // Cycle between fly frame 1 and 2
            foogaAnimTimer++;
            if (foogaAnimTimer >= FOOGA_ANIM_SPEED)
            {
                foogaAnimTimer = 0;
                foogaAnimFrame = (foogaAnimFrame + 1) % 2;
            }

            if (foogaAnimFrame == 0)
                foogaSprite.setTextureRect(sf::IntRect(FLY1_X, FLY1_Y, FLY1_W, FLY1_H));
            else
                foogaSprite.setTextureRect(sf::IntRect(FLY2_X, FLY2_Y, FLY2_W, FLY2_H));

            // Scale to consistent size
            float scaleX = 60.f / FLY1_W;
            float scaleY = 60.f / FLY1_H;
            // Sprite faces diagonal left on sheet
            // velocityX > 0 means flying right so flip horizontally
            if (velocityX > 0)
            {
                foogaSprite.setScale(-scaleX, scaleY);
                foogaSprite.setPosition(x + 60.f, y);
            }
            else
            {
                foogaSprite.setScale(scaleX, scaleY);
                foogaSprite.setPosition(x, y);
            }
        }// when on ground its Green......
        else
        {
            // Walking - frame 1 only, flip based on direction
            foogaAnimFrame = 0;
            foogaAnimTimer = 0;

            foogaSprite.setTextureRect(sf::IntRect(WALK_X, WALK_Y, WALK_W, WALK_H));

            float scaleX = 40.f / WALK_W;
            float scaleY = 40.f / WALK_H;
            // Flip based on walking direction
            // velocityX > 0 = moving right
            if (velocityX > 0)
            {
                foogaSprite.setScale(-scaleX, scaleY);
                foogaSprite.setPosition(x + 40.f, y);
            }
            else
            {
                foogaSprite.setScale(scaleX, scaleY);
                foogaSprite.setPosition(x, y);
            }
        }

        window.draw(foogaSprite);

        // Snow overlay if partially hit

        // Show snow overlay based on hit count (partially hit)
        if (hitCount == 1)
        {
            // Light snow - 25% covered
            sf::CircleShape snowOverlay(15.f);
            snowOverlay.setFillColor(sf::Color(255, 255, 255, 80));
            snowOverlay.setPosition(x, y);
            window.draw(snowOverlay);
        }
    }

    void setTint(sf::Color color) override
    {
        enemySprite.setColor(color);    // for when its walking (uses botom sprite)
        foogaSprite.setColor(color);    // for flying frames
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