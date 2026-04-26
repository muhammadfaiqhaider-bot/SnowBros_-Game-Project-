#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "SnowBall.h"


class Level2
{
private:
    static const int BOTOM_COUNT = 5;
    static const int FLYING_COUNT = 3;

    Botom* botoms[BOTOM_COUNT];
    FlyingFoogaFoog* flyingEnemies[FLYING_COUNT];

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

public:
    Level2()
    {
        // ---- Spawn 5 Botoms ----
        botoms[0] = new Botom(100.f, 520.f);
        botoms[1] = new Botom(250.f, 520.f);
        botoms[2] = new Botom(400.f, 520.f);
        botoms[3] = new Botom(150.f, 380.f);
        botoms[4] = new Botom(450.f, 380.f);

        // ---- Spawn 3 FlyingFoogo ----
        flyingEnemies[0] = new FlyingFoogaFoog(200.f, 240.f);
        flyingEnemies[1] = new FlyingFoogaFoog(400.f, 100.f);
        flyingEnemies[2] = new FlyingFoogaFoog(100.f, 100.f);

        // ---- Load background ----
        backgroundLoaded = backgroundTexture.loadFromFile("assets/bg_level2.png");
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
        // ---- Update Botoms ----
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] == nullptr || botoms[i]->getIsDead())
            {
                continue;
            }

            // Platform collision
            botoms[i]->setOnGround(false);

            for (int j = 0; j < platformCount; j++)
            {
                sf::FloatRect botomBounds(
                    botoms[i]->getPositionX(),
                    botoms[i]->getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect platformBounds = platforms[j].getGlobalBounds();

                if (botomBounds.intersects(platformBounds))
                {
                    float platformTop = platforms[j].getPosition().y;
                    float botomBottom = botoms[i]->getPositionY() + 40.f;

                    if (botomBottom >= platformTop &&
                        botoms[i]->getPositionY() < platformTop &&
                        botoms[i]->getVelocityY() >= 0)
                    {
                        botoms[i]->setOnGround(true);
                        botoms[i]->snapToGround(platformTop - 39.99f);
                    }
                }
            }

            // Wall check
            if (botoms[i]->getPositionX() <= 0 ||
                botoms[i]->getPositionX() >= 560)
            {
                botoms[i]->setHitWall(true);
            }

            botoms[i]->movementsUpdate();
        }

        // ---- Update FlyingFoogo ----
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] == nullptr || flyingEnemies[i]->getIsDead())
            {
                continue;
            }

            // Platform collision - same as botom
            flyingEnemies[i]->setOnGround(false);

            for (int j = 0; j < platformCount; j++)
            {
                sf::FloatRect flyingBounds(
                    flyingEnemies[i]->getPositionX(),
                    flyingEnemies[i]->getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect platformBounds = platforms[j].getGlobalBounds();

                if (flyingBounds.intersects(platformBounds))
                {
                    float platformTop = platforms[j].getPosition().y;

                    if (flyingEnemies[i]->getPositionY() + 40.f <= platformTop + 10.f)
                    {
                        flyingEnemies[i]->setOnGround(true);
                        flyingEnemies[i]->snapToGround(platformTop - 40.f);
                    }
                }
            }

            flyingEnemies[i]->movementsUpdate();
        }

        // ---- Rolling Botom kills other enemies ----
        checkChainKill();
    }

    // ==========================================
    // SNOWBALL COLLISION
    // ==========================================

    void checkSnowballCollision(Snowball* snowball)
    {
        if (snowball == nullptr)
        {
            return;
        }

        sf::FloatRect snowballBounds = snowball->getHitBox();

        // Check against Botoms
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] == nullptr || botoms[i]->getIsDead())
            {
                continue;
            }

            sf::FloatRect enemyBounds(
                botoms[i]->getPositionX(),
                botoms[i]->getPositionY(),
                40.f, 40.f
            );

            if (snowballBounds.intersects(enemyBounds))
            {
                if (botoms[i]->getIsEncased() && !botoms[i]->getIsRolling())
                {
                    // Launch rolling
                    botoms[i]->kickRoll(snowball->getDirection());
                    snowball->deactivate();
                    return;
                }

                if (!botoms[i]->getIsEncased())
                {
                    botoms[i]->reduceHealth();
                    snowball->deactivate();
                    return;
                }
            }
        }

        // Check against FlyingFoogo
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] == nullptr || flyingEnemies[i]->getIsDead())
            {
                continue;
            }

            sf::FloatRect enemyBounds(
                flyingEnemies[i]->getPositionX(),
                flyingEnemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (snowballBounds.intersects(enemyBounds))
            {
                if (flyingEnemies[i]->getIsEncased() && !flyingEnemies[i]->getIsRolling())
                {
                    // Launch rolling
                    flyingEnemies[i]->kickRoll(snowball->getDirection());
                    snowball->deactivate();
                    return;
                }

                if (!flyingEnemies[i]->getIsEncased())
                {
                    flyingEnemies[i]->reduceHealth();
                    snowball->deactivate();
                    return;
                }
            }
        }
    }

    // ==========================================
    // PLAYER HIT CHECK
    // ==========================================

    bool isPlayerHit(float playerX, float playerY)
    {
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

        // Check Botoms
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] == nullptr || botoms[i]->getIsDead())
            {
                continue;
            }

            // Encased stationary - safe to touch
            if (botoms[i]->getIsEncased() && !botoms[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect enemyBounds(
                botoms[i]->getPositionX(),
                botoms[i]->getPositionY(),
                40.f, 40.f
            );

            if (playerBounds.intersects(enemyBounds))
            {
                return true;
            }
        }

        // Check FlyingFoogo
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] == nullptr || flyingEnemies[i]->getIsDead())
            {
                continue;
            }

            // Encased stationary - safe to touch
            if (flyingEnemies[i]->getIsEncased() && !flyingEnemies[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect enemyBounds(
                flyingEnemies[i]->getPositionX(),
                flyingEnemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (playerBounds.intersects(enemyBounds))
            {
                return true;
            }
        }

        return false;
    }

    // ==========================================
    // LEVEL COMPLETE
    // ==========================================

    bool isLevelComplete()
    {
        // Check all Botoms dead
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] == nullptr)
            {
                continue;
            }

            if (!botoms[i]->getIsDead())
            {
                return false;
            }
        }

        // Check all FlyingFoogo dead
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] == nullptr)
            {
                continue;
            }

            if (!flyingEnemies[i]->getIsDead())
            {
                return false;
            }
        }

        return true;    // All dead!
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

        // Draw Botoms
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] != nullptr && !botoms[i]->getIsDead())
            {
                botoms[i]->DisplayEnemy(window);
            }
        }

        // Draw FlyingFoogo
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] != nullptr && !flyingEnemies[i]->getIsDead())
            {
                flyingEnemies[i]->DisplayEnemy(window);
            }
        }
    }

    // ==========================================
    // DESTRUCTOR
    // ==========================================

    ~Level2()
    {
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] != nullptr)
            {
                delete botoms[i];
                botoms[i] = nullptr;
            }
        }

        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] != nullptr)
            {
                delete flyingEnemies[i];
                flyingEnemies[i] = nullptr;
            }
        }
    }

private:

    // ==========================================
    // CHAIN KILL
    // ==========================================

    void checkChainKill()
    {
        // Rolling Botom kills other Botoms
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] == nullptr || !botoms[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect rollingBounds(
                botoms[i]->getPositionX(),
                botoms[i]->getPositionY(),
                44.f, 44.f
            );

            // Check against other Botoms
            for (int j = 0; j < BOTOM_COUNT; j++)
            {
                if (i == j || botoms[j] == nullptr ||
                    botoms[j]->getIsDead() || botoms[j]->getIsRolling())
                {
                    continue;
                }

                sf::FloatRect otherBounds(
                    botoms[j]->getPositionX(),
                    botoms[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    botoms[j]->instantKill();
                }
            }

            // Check against FlyingFoogo
            for (int j = 0; j < FLYING_COUNT; j++)
            {
                if (flyingEnemies[j] == nullptr ||
                    flyingEnemies[j]->getIsDead() ||
                    flyingEnemies[j]->getIsRolling())
                {
                    continue;
                }

                sf::FloatRect otherBounds(
                    flyingEnemies[j]->getPositionX(),
                    flyingEnemies[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    flyingEnemies[j]->instantKill();
                }
            }
        }

        // Rolling FlyingFoogo kills others too
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] == nullptr || !flyingEnemies[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect rollingBounds(
                flyingEnemies[i]->getPositionX(),
                flyingEnemies[i]->getPositionY(),
                44.f, 44.f
            );

            // Check against Botoms
            for (int j = 0; j < BOTOM_COUNT; j++)
            {
                if (botoms[j] == nullptr ||
                    botoms[j]->getIsDead() ||
                    botoms[j]->getIsRolling())
                {
                    continue;
                }

                sf::FloatRect otherBounds(
                    botoms[j]->getPositionX(),
                    botoms[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    botoms[j]->instantKill();
                }
            }

            // Check against other FlyingFoogo
            for (int j = 0; j < FLYING_COUNT; j++)
            {
                if (i == j || flyingEnemies[j] == nullptr ||
                    flyingEnemies[j]->getIsDead() ||
                    flyingEnemies[j]->getIsRolling())
                {
                    continue;
                }

                sf::FloatRect otherBounds(
                    flyingEnemies[j]->getPositionX(),
                    flyingEnemies[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    flyingEnemies[j]->instantKill();
                }
            }
        }
    }
};