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

    // Sprite sheet info for Tornado
    // Total sheet: 248x115, frame width: 110, frame height: 115, gap between frames: 28
    int frameGap;

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

        // Sprite sheet setup for Tornado only 2 frames bcz i m lazy ....
        // Sheet: 248x115 total, each frame: 110x115, gap between frames: 28
        frameWidth = 110;
        frameHeight = 115;
        totalFrames = 2;
        currentFrame = 0;
        animTimer = 0;
        animSpeed = 15;         // Switch frame every 15 ticks  smooth flying animation
        frameGap = 28;

        if (enemyTexture.loadFromFile("assets/Tornado_Blue.png"))
        {
            textureLoaded = true;
            enemySprite.setTexture(enemyTexture);
            enemySprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
            // Scale to 40x40 display size (same as tornado shape was before)
            float sx = 40.f / (float)frameWidth;
            float sy = 40.f / (float)frameHeight;
            enemySprite.setScale(sx, sy);
            enemySprite.setPosition(x, y);
        }
    }

    void setPlayerPosition(float pX, float pY)
    {                           // Cheema Use this function for updating player position in every
        playerX = pX;           // Frame.......bcz churi will be sent according to it 
        playerY = pY;
    }

    void movementsUpdate() override
    {
 

        // If dead  do nothing and clean up knife
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

        // If rolling only do rolling physics no teleporting no attacks
        if (isRolling)
        {
            // stop teleporting when rolling
            isTeleporting = false;
            teleportTimer = 0;
            attackTimer = 0;

            // delete knife if exists    very importacnt to delete things i have fae cocequences......I wnpn't this sht ever again....
            // soo cheema be carefull......
            if (churi != nullptr)
            {
                delete churi;
                churi = nullptr;
            }

            // Apply rolling physics
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
                // Vanish after 3 seconds - ensure rolling state is cleared to avoid ghost collisions
                isDead = true;
                isRolling = false;
                rollVelocityX = 0;

                // Clean up any active knife immediately
                if (churi != nullptr)
                {
                    delete churi;
                    churi = nullptr;
                }
            }

            return;    
        }

        // If encased  STOP all movement
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

            // Just fall to ground in short gravity dali hai.....
            if (!onGround)
            {
                velocityY = velocityY + 0.5f;
                if (velocityY > 8.0f)
                {
                    velocityY = 8.0f;
                }
                y = y + velocityY;
            }
            else
            {
                velocityY = 0;
            }

            return;     
        }

        if (isDead)
        {
            isRolling = false;
            rollVelocityX = 0;
            if (churi != nullptr)
            {
                delete churi;
                churi = nullptr;
            }
            return;
        }


        teleportTimer++;

        if (isTeleporting)
        {
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

        // Flying animation  switch between 2 frames (frame 0 and frame 1)
        // Same logic as Botom: currentFrame * (frameWidth + frameGap) gives correct x offset
        if (textureLoaded)
        {
            animTimer++;
            if (animTimer >= animSpeed)
            {
                animTimer = 0;
                currentFrame = (currentFrame + 1) % totalFrames;    // Toggles between 0 and 1
            }

            int left = currentFrame * (frameWidth + frameGap);      // Frame 0: x=0, Frame 1: x=138
            enemySprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
            enemySprite.setPosition(x, y);
        }

        // Knife Throwing Thing
        attackTimer++;

        if (attackTimer >= attackDuration)
        {
            attackTimer = 0;

            if (churi == nullptr)
            {
                // Throw a horizontal-only knife toward player's X position.
                // Set targetY to this tornado's y so the knife travels only along x-axis.
                float targetX = playerX;
                float targetY = y; // keep same vertical level for pure horizontal throw
                churi = new Knives(x, y, targetX, targetY, 600.0f);
            }
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
            // death ke sprite sheet's aye gi yahan pr
            return;
        }

        // rolling snowball....
        if (isRolling)
        {
            sf::CircleShape rollingBall(15.0f);
            rollingBall.setFillColor(sf::Color::White);
            rollingBall.setPosition(x, y);
            window.draw(rollingBall);
            return;
        }

        // fully encased...
        if (snowCovered)
        {
            sf::CircleShape snowBall(20.0f);
            snowBall.setFillColor(sf::Color::White);
            snowBall.setPosition(x, y);
            window.draw(snowBall);

            return;
        }

        // normal zinda contion wala display........
        if (textureLoaded)
        {
            window.draw(enemySprite);
        }
        else
        {
            // if by any chance sprite sheet doesn't load so recatgnle apears
            sf::RectangleShape tornadoShape(sf::Vector2f(40.f, 40.f));
            tornadoShape.setFillColor(sf::Color::Cyan);
            tornadoShape.setPosition(x, y);
            window.draw(tornadoShape);
        }

        // Show snow overlay based on hit count (partially hit)
        if (hitCount == 1)
        {
            // Light snow - 25% covered
            sf::CircleShape snowOverlay(20.f);
            snowOverlay.setFillColor(sf::Color(255, 255, 255, 80));
            snowOverlay.setPosition(x, y);
            window.draw(snowOverlay);
        }
 

        // Draw Knives Sprite shyt implementation
        if (churi != nullptr)
        {
            churi->drawProjectiles(window);
        }
    }



    bool isKnifeHittingPlayer(float playerX, float playerY) override
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

    bool isProjectileHittingPlayer(float playerX, float playerY) override
    {
        return isKnifeHittingPlayer(playerX, playerY);
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