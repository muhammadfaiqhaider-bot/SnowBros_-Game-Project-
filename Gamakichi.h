#pragma once
#include "Enemy.h"
#include "Rocket.h"
#include "PLayer.h"
#include <cmath>
#include <iostream>



class Gamakichi : public Enemy
{
private:
    int phase;
    int attackTimer;
    int attackDuration;
    int phaseTimer;
    bool isDefeated;
    bool phaseChanged;                                // Flag to prevent phase reset every frame

    static const int MAX_ROCKETS = 5;
    Rocket* rockets[MAX_ROCKETS];

    float playerX;
    float playerY;

    bool onGround;
    float velocityY;
    int frameGap;

public:
    Gamakichi(float posX, float posY) : Enemy(posX, posY, "Gamakichi")
    {                                                              // Gamakichi He is the final boss so he ll  have zada power
        health = 35;
        phase = 1;
        attackTimer = 0;                                          // diffeence in firing rockets......
        attackDuration = 100;                                     // 100 frames means 1.6 something seconds gap
        phaseTimer = 0;
        isDefeated = false;
        phaseChanged = false;

        playerX = 0;
        playerY = 0;

        onGround = false;
        velocityY = 0;

        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            rockets[i] = nullptr;
        }

        // Sprite-sheet setup for Gamakichi (stationary with 2 frames)
        // Frame height = 629, total width used = 1086, gap between frames = 230
        frameWidth =  1086;   
        frameHeight = 629;
        totalFrames = 2;
        currentFrame = 0;
        animTimer = 0;
        animSpeed = 20;
        frameGap = 230;

        if (enemyTexture.loadFromFile("assets/Gamakichi_Face.png"))
        {
            textureLoaded = true;
            enemySprite.setTexture(enemyTexture);
            enemySprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
            // scale to roughly 100x120 used previously
            float sx = 100.f / (float)frameWidth;
            float sy = 120.f / (float)frameHeight;
            enemySprite.setScale(sx, sy);
            enemySprite.setPosition(x, y);
        }
    }

    void setPlayerPosition(float playerPosX, float playerPosY)
    {
        playerX = playerPosX;
        playerY = playerPosY;
    }

    void movementsUpdate() override
    {
        // Dont update if defeated
        if (isDefeated)
        {
            return;
        }
        // Gamakichi is fixed Enemy and its x y coordinate can't be changed
        if (x < 0)
        {
            x = 0;
        }

        if (!onGround)
        {
            velocityY += 0.5f;
            if (velocityY > 8.f)
            {
                velocityY = 8.f;
            }
            y += velocityY;
        }
        else
        {
            velocityY = 0;
        }

        if (x > 520)        // 600 - 80 (boss width)
        {
            x = 520;
        }
        if (y < 0)
        {
            y = 0;
        }
        if (y > 480)        // 600 - 120 (boss height)
        {
            y = 480;
        }

        attackTimer++;
        phaseTimer++;

        phaseChanger();
        handleAttack();
        updateRockets();

        // idle animation
        if (textureLoaded && totalFrames > 1)
        {
            animTimer++;
            if (animTimer >= animSpeed)
            {
                animTimer = 0;
                currentFrame = (currentFrame + 1) % totalFrames;
            }

            int left = currentFrame * (frameWidth + frameGap);
            enemySprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
            enemySprite.setPosition(x, y);
        }
    }

    void phaseChanger()
    {
        // In start Phase 1 of Gamkichi is running through as its helath hits 25 phase 2 triggers where the Speed or intervals
        if (health <= 25 && phase == 1)    // of gamakichi attacks increased.......less than a 1.5 sec 
        {
            phase = 2;
            attackDuration = 80;
            phaseTimer = 0;
        }

        // When health reaches 15 phase 3 triggers where attack seep is very fast rockets are thown on u drastically 
        if (health <= 15 && phase == 2)    // Difficult to dodge them 
        {
            phase = 3;
            attackDuration = 40;
            phaseTimer = 0;
        }
    }

    void handleAttack()
    {
        if (attackTimer >= attackDuration)
        {
            attackTimer = 0;
            // Launching Rockets logic at phase 1 Rockets 

            if (phase == 1)
            {
                fireRocket();
            }


            if (phase == 2)
            {                                            // Two rockets are lauched at stage 2 
                fireRocket();
                fireRocket();
            }


            if (phase == 3)
            {
                fireRocket();                            // Three rockets are luanched at phase 3
                fireRocket();
                fireRocket();
            }
        }
    }

    void fireRocket()
    {
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] == nullptr)
            {
                rockets[i] = new Rocket(x, y, playerX, playerY, 600.f, 600.f);
                break;
            }
        }
    }

    void updateRockets()
    {
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] != nullptr)
            {
                rockets[i]->updateTrajactory();

                if (!rockets[i]->isActive())
                {
                    delete rockets[i];
                    rockets[i] = nullptr;
                }
            }
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (isDefeated)
        {
            return;
        }

        if (textureLoaded)
        {
            // ensure frame is updated
            int left = currentFrame * (frameWidth + frameGap);
            enemySprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
            enemySprite.setPosition(x, y);
            window.draw(enemySprite);
        }
        else
        {
            // Gamakichi - large purple rectangle (bigger than Mogera)
            sf::RectangleShape gamakichiShape(sf::Vector2f(100.f, 120.f));
            gamakichiShape.setFillColor(sf::Color(100, 0, 150));    // Dark purple
            gamakichiShape.setPosition(x, y);
            window.draw(gamakichiShape);
        }

        // Health bar background (red)
        sf::RectangleShape healthBarBackground(sf::Vector2f(100.f, 12.f));
        healthBarBackground.setFillColor(sf::Color::Red);
        healthBarBackground.setPosition(x, y - 20.f);
        window.draw(healthBarBackground);

        // Health bar foreground (shrinks as health decreases)
        float healthPercent = (float)health / 35.0f;
        sf::RectangleShape healthBarForeground(sf::Vector2f(100.f * healthPercent, 12.f));
        healthBarForeground.setFillColor(sf::Color::Green);
        healthBarForeground.setPosition(x, y - 20.f);
        window.draw(healthBarForeground);

        // Phase indicator - small colored box shows current phase
        sf::RectangleShape phaseIndicator(sf::Vector2f(20.f, 10.f));

        if (phase == 1)
        {
            phaseIndicator.setFillColor(sf::Color::Green);
        }
        if (phase == 2)
        {
            phaseIndicator.setFillColor(sf::Color::Yellow);
        }
        if (phase == 3)
        {
            phaseIndicator.setFillColor(sf::Color::Red);
        }

        phaseIndicator.setPosition(x, y - 35.f);
        window.draw(phaseIndicator);

        // Draw all active rockets
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] != nullptr)
            {
                rockets[i]->drawProjectiles(window);
            }
        }
    }

    void reduceHealth() override
    {
        health--;

        if (health <= 0)
        {
            health = 0;
            isDefeated = true;
            snowCovered = true;
        }
    }

    bool getIsDefeated()
    {
        return isDefeated;
    }

    // Expose death state to Level
    bool getIsDead() override
    {
        return isDefeated;
    }

    void setOnGround(bool value)
    {
        onGround = value;
    }

    void snapToGround(float groundY)
    {
        y = groundY;
        velocityY = 0;
        onGround = true;
    }

    int getPhase()
    {
        return phase;
    }





    bool isRocketHittingPlayer(float playerX, float playerY)
    {
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] == nullptr || !rockets[i]->isActive())
            {
                continue;
            }

            sf::FloatRect rocketBounds(
                rockets[i]->getX(),
                rockets[i]->getY(),
                15.f, 8.f
            );

            if (rocketBounds.intersects(playerBounds))
            {
                rockets[i]->deactivate();   // Rocket disappears on hit
                std::cout << "[Gamakichi] rocket hit player at (" << playerX << "," << playerY << ")\n";
                return true;
            }
        }

        return false;
    }

    bool isProjectileHittingPlayer(float playerX, float playerY) override
    {
        return isRocketHittingPlayer(playerX, playerY);
    }



    ~Gamakichi()
    {
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] != nullptr)
            {
                delete rockets[i];
                rockets[i] = nullptr;
            }
        }
    }
};