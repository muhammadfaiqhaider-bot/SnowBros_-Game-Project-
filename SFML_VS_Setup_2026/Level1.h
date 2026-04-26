#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "SnowBall.h"

class Level1
{
private:
    static const int ENEMY_COUNT = 3;
    Botom* enemies[ENEMY_COUNT];

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

public:
    Level1()
    {
        enemies[0] = new Botom(150.f, 520.f);
        enemies[1] = new Botom(350.f, 520.f);
        enemies[2] = new Botom(500.f, 380.f);

        backgroundLoaded = backgroundTexture.loadFromFile("assets/bg_level1.png");
        if (backgroundLoaded)
        {
            backgroundSprite.setTexture(backgroundTexture);
            float scaleX = 600.f / backgroundTexture.getSize().x;
            float scaleY = 600.f / backgroundTexture.getSize().y;
            backgroundSprite.setScale(scaleX, scaleY);
        }
    }

    // ==========================================
    // UPDATE
    // ==========================================

    void update(float playerX, float playerY,
        sf::RectangleShape platforms[], int platformCount)
    {
        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
            {
                continue;
            }

            // Platform collision
            enemies[i]->setOnGround(false);

            for (int j = 0; j < platformCount; j++)
            {
                sf::FloatRect enemyBounds(
                    enemies[i]->getPositionX(),
                    enemies[i]->getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect platformBounds = platforms[j].getGlobalBounds();

                if (enemyBounds.intersects(platformBounds))
                {
                    float platformTop = platforms[j].getPosition().y;
                    float enemyBottom = enemies[i]->getPositionY() + 40.f;

                    if (enemyBottom >= platformTop &&
                        enemies[i]->getPositionY() < platformTop &&
                        enemies[i]->getVelocityY() >= 0)
                    {
                        enemies[i]->setOnGround(true);
                        enemies[i]->snapToGround(platformTop - 39.99f);
                    }
                }
            }

            // Wall check
            if (enemies[i]->getPositionX() <= 0 ||
                enemies[i]->getPositionX() >= 560)
            {
                enemies[i]->setHitWall(true);
            }

            enemies[i]->movementsUpdate();
        }

        // ---- PLAYER KICKS ENCASED ENEMY ----
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
            {
                continue;
            }

            if (enemies[i]->getIsEncased() && !enemies[i]->getIsRolling())
            {
                sf::FloatRect enemyBounds(
                    enemies[i]->getPositionX(),
                    enemies[i]->getPositionY(),
                    40.f, 40.f
                );

                if (playerBounds.intersects(enemyBounds))
                {
                    // Kick direction based on player position
                    int kickDir = 1;
                    if (playerX > enemies[i]->getPositionX())
                    {
                        kickDir = -1;   // Player on right - kick left
                    }
                    enemies[i]->kickRoll(kickDir);
                }
            }
        }

        // ---- ROLLING ENEMY KILLS OTHER ENEMIES ----
        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
            {
                continue;
            }

            if (!enemies[i]->getIsRolling())
            {
                continue;
            }

            // Check if rolling enemy hits other enemies
            sf::FloatRect rollingBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                44.f, 44.f
            );

            for (int j = 0; j < ENEMY_COUNT; j++)
            {
                if (i == j)
                {
                    continue;   // Don't check against itself
                }

                if (enemies[j] == nullptr || enemies[j]->getIsDead())
                {
                    continue;
                }

                if (enemies[j]->getIsRolling())
                {
                    continue;   // Don't check rolling vs rolling
                }

                sf::FloatRect otherBounds(
                    enemies[j]->getPositionX(),
                    enemies[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    // Remove these 3 reduceHealth calls
                    // enemies[j]->reduceHealth();
                    // enemies[j]->reduceHealth();
                    // enemies[j]->reduceHealth();

                    // Instead directly set isDead
                    // But isDead is private so add this setter in Botom.h:
                    enemies[j]->instantKill();

                    // Keep the chain roll
                    enemies[j]->kickRoll(
                        enemies[i]->getRollVelocityX() > 0 ? 1 : -1
                    );
                }
            }
        }
    }



    void checkSnowballCollision(Snowball* snowball)
    {
        if (snowball == nullptr)
        {
            return;
        }

        sf::FloatRect snowballBounds = snowball->getHitBox();

        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
            {
                continue;
            }

            sf::FloatRect enemyBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (snowballBounds.intersects(enemyBounds))
            {
                // If already encased → launch it rolling
                if (enemies[i]->getIsEncased() && !enemies[i]->getIsRolling())
                {
                    // Direction based on snowball direction
                    int kickDir = snowball->getDirection();
                    enemies[i]->kickRoll(kickDir);
                    snowball->deactivate();
                    break;
                }

                // If alive → reduce health normally
                if (!enemies[i]->getIsEncased())
                {
                    enemies[i]->reduceHealth();
                    snowball->deactivate();
                    break;
                }
            }
        }
    }


    bool isPlayerHit(float playerX, float playerY)
    {
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
            {
                continue;
            }

            // Encased stationary enemy doesn't hurt player
            if (enemies[i]->getIsEncased() && !enemies[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect enemyBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (playerBounds.intersects(enemyBounds))
            {
                return true;    // Player hit!
            }
        }

        return false;
    }

 

    bool isLevelComplete()
    {
        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if (enemies[i] == nullptr)
            {
                continue;
            }

            // Level not complete if any enemy still alive and not dead
            if (!enemies[i]->getIsDead())
            {
                return false;
            }
        }

        return true;    // All enemies dead!
    }

    // ==========================================
    // DRAW
    // ==========================================

    void draw(sf::RenderWindow& window)
    {
        if (backgroundLoaded)
        {
            window.draw(backgroundSprite);
        }

        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if (enemies[i] != nullptr && !enemies[i]->getIsDead())
            {
                enemies[i]->DisplayEnemy(window);
            }
        }
    }

 
    ~Level1()
    {
        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if (enemies[i] != nullptr)
            {
                delete enemies[i];
                enemies[i] = nullptr;
            }
        }
    }
};