#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "Tornado.h"
#include "SnowBall.h"

class Level3
{
private:
    static const int BOTOM_COUNT = 1;
    static const int FLYING_COUNT = 1;
    static const int TORNADO_COUNT = 3;

    Botom* botoms[BOTOM_COUNT];
    FlyingFoogaFoog* flyingEnemies[FLYING_COUNT];
    Tornado* tornados[TORNADO_COUNT];

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

public:
    Level3()
    {
        // ---- Spawn Botoms ----
        botoms[0] = new Botom(100.f, 520.f);
        botoms[1] = new Botom(300.f, 520.f);
        botoms[2] = new Botom(500.f, 380.f);

        // ---- Spawn FlyingFoogo ----
        flyingEnemies[0] = new FlyingFoogaFoog(200.f, 240.f);
        flyingEnemies[1] = new FlyingFoogaFoog(450.f, 100.f);

        // ---- Spawn Tornados ----
        tornados[0] = new Tornado(150.f, 100.f);
        tornados[1] = new Tornado(400.f, 240.f);
        tornados[2] = new Tornado(300.f, 380.f);

        // ---- Load background ----
        backgroundLoaded = backgroundTexture.loadFromFile("assets/bg_level3.png");
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

            flyingEnemies[i]->setOnGround(false);

            if (flyingEnemies[i]->getIsFlying())
            {
                // Only ground platform during flight
                sf::FloatRect flyingBounds(
                    flyingEnemies[i]->getPositionX(),
                    flyingEnemies[i]->getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect groundBounds = platforms[0].getGlobalBounds();

                if (flyingBounds.intersects(groundBounds))
                {
                    float groundTop = platforms[0].getPosition().y;

                    if (flyingEnemies[i]->getPositionY() + 40.f <= groundTop + 10.f)
                    {
                        flyingEnemies[i]->setOnGround(true);
                        flyingEnemies[i]->snapToGround(groundTop - 40.f);
                    }
                }
            }
            else
            {
                // All platforms when walking
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
            }

            flyingEnemies[i]->movementsUpdate();
        }

        // ---- Update Tornados ----
        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] == nullptr || tornados[i]->getIsDead())
            {
                continue;
            }

            tornados[i]->setOnGround(false);

            if (tornados[i]->getIsFlying())
            {
                // Only ground platform during flight/teleport
                sf::FloatRect tornadoBounds(
                    tornados[i]->getPositionX(),
                    tornados[i]->getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect groundBounds = platforms[0].getGlobalBounds();

                if (tornadoBounds.intersects(groundBounds))
                {
                    float groundTop = platforms[0].getPosition().y;

                    if (tornados[i]->getPositionY() + 40.f <= groundTop + 10.f)
                    {
                        tornados[i]->setOnGround(true);
                        tornados[i]->snapToGround(groundTop - 40.f);
                    }
                }
            }
            else
            {
                // All platforms when walking
                for (int j = 0; j < platformCount; j++)
                {
                    sf::FloatRect tornadoBounds(
                        tornados[i]->getPositionX(),
                        tornados[i]->getPositionY(),
                        40.f, 40.f
                    );
                    sf::FloatRect platformBounds = platforms[j].getGlobalBounds();

                    if (tornadoBounds.intersects(platformBounds))
                    {
                        float platformTop = platforms[j].getPosition().y;
                        float tornadoBottom = tornados[i]->getPositionY() + 40.f;

                        if (tornadoBottom >= platformTop &&
                            tornados[i]->getPositionY() < platformTop &&
                            tornados[i]->getVelocityY() >= 0)
                        {
                            tornados[i]->setOnGround(true);
                            tornados[i]->snapToGround(platformTop - 39.99f);
                        }
                    }
                }
            }

            // Update player position for knife throwing
            tornados[i]->setPlayerPosition(playerX, playerY);
            tornados[i]->movementsUpdate();
        }

        // ---- Chain kill check ----
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

        // Check Botoms
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

        // Check FlyingFoogo
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

        // Check Tornados
        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] == nullptr || tornados[i]->getIsDead())
            {
                continue;
            }

            sf::FloatRect enemyBounds(
                tornados[i]->getPositionX(),
                tornados[i]->getPositionY(),
                40.f, 40.f
            );

            if (snowballBounds.intersects(enemyBounds))
            {
                if (tornados[i]->getIsEncased() && !tornados[i]->getIsRolling())
                {
                    tornados[i]->kickRoll(snowball->getDirection());
                    snowball->deactivate();
                    return;
                }

                if (!tornados[i]->getIsEncased())
                {
                    tornados[i]->reduceHealth();
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

        // Check Tornados
        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] == nullptr || tornados[i]->getIsDead())
            {
                continue;
            }

            if (tornados[i]->getIsEncased() && !tornados[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect enemyBounds(
                tornados[i]->getPositionX(),
                tornados[i]->getPositionY(),
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
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] != nullptr && !botoms[i]->getIsDead())
            {
                return false;
            }
        }

        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] != nullptr && !flyingEnemies[i]->getIsDead())
            {
                return false;
            }
        }

        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] != nullptr && !tornados[i]->getIsDead())
            {
                return false;
            }
        }

        return true;
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

        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] != nullptr && !botoms[i]->getIsDead())
            {
                botoms[i]->DisplayEnemy(window);
            }
        }

        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] != nullptr && !flyingEnemies[i]->getIsDead())
            {
                flyingEnemies[i]->DisplayEnemy(window);
            }
        }

        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] != nullptr && !tornados[i]->getIsDead())
            {
                tornados[i]->DisplayEnemy(window);
            }
        }
    }

    // ==========================================
    // DESTRUCTOR
    // ==========================================

    ~Level3()
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

        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] != nullptr)
            {
                delete tornados[i];
                tornados[i] = nullptr;
            }
        }
    }

private:

    // ==========================================
    // CHAIN KILL
    // ==========================================

    void checkChainKill()
    {
        // All rolling enemies - check against all others
        Botom* allEnemies[BOTOM_COUNT + FLYING_COUNT + TORNADO_COUNT];
        int totalCount = 0;

        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            allEnemies[totalCount++] = botoms[i];
        }
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            allEnemies[totalCount++] = flyingEnemies[i];
        }
        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            allEnemies[totalCount++] = tornados[i];
        }

        // Check rolling vs all others
        for (int i = 0; i < totalCount; i++)
        {
            if (allEnemies[i] == nullptr || !allEnemies[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect rollingBounds(
                allEnemies[i]->getPositionX(),
                allEnemies[i]->getPositionY(),
                44.f, 44.f
            );

            for (int j = 0; j < totalCount; j++)
            {
                if (i == j || allEnemies[j] == nullptr ||
                    allEnemies[j]->getIsDead() ||
                    allEnemies[j]->getIsRolling())
                {
                    continue;
                }

                sf::FloatRect otherBounds(
                    allEnemies[j]->getPositionX(),
                    allEnemies[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    allEnemies[j]->instantKill();
                }
            }
        }
    }
};