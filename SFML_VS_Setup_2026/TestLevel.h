#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "Tornado.h"
#include "CollisionManager.h"
#include "Nick.h"
#include "SnowBall.h"

class TestLevel
{
private:
    // ---- Platforms ----
    sf::RectangleShape platforms[6];

    // ---- Enemies ----
    static const int BOTOM_COUNT = 2;
    static const int FLYING_COUNT = 1;
    static const int TORNADO_COUNT = 1;

    Botom* botoms[BOTOM_COUNT];
    FlyingFoogaFoog* flyingEnemies[FLYING_COUNT];
    Tornado* tornados[TORNADO_COUNT];

public:
    TestLevel()
    {
        setupPlatforms();

        // Spawn enemies
        botoms[0] = new Botom(150.f, 520.f);
        botoms[1] = new Botom(400.f, 380.f);

        flyingEnemies[0] = new FlyingFoogaFoog(300.f, 240.f);

        tornados[0] = new Tornado(450.f, 100.f);
    }

    // ==========================================
    // UPDATE
    // ==========================================

    void update(Nick& player)
    {
        // Player collision
        CollisionManager::handlePlayer(player, platforms, 6);

        // Botom collisions
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] == nullptr || botoms[i]->getIsDead())
            {
                continue;
            }

            CollisionManager::handleBotom(*botoms[i], platforms, 6);
            botoms[i]->movementsUpdate();
        }

        // FlyingFoogo collisions
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] == nullptr || flyingEnemies[i]->getIsDead())
            {
                continue;
            }

            CollisionManager::handleFlyingFoogo(*flyingEnemies[i], platforms, 6);
            flyingEnemies[i]->movementsUpdate();
        }

        // Tornado collisions
        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] == nullptr || tornados[i]->getIsDead())
            {
                continue;
            }

            CollisionManager::handleTornado(*tornados[i], platforms, 6);
            tornados[i]->setPlayerPosition(player.getPositionX(), player.getPositionY());
            tornados[i]->movementsUpdate();
        }

        // After tornado update loop add this:
        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] == nullptr || tornados[i]->getIsDead())
            {
                continue;
            }

            // Check if tornado's knife hit player
            if (tornados[i]->isKnifeHittingPlayer(
                player.getPositionX(),
                player.getPositionY()))
            {
                player.loseLife();
            }
        }

        // Snowball collision
        if (player.getSnowball() != nullptr)
        {
            checkSnowballCollision(player.getSnowball());
        }

        // Player hit check
        if (isPlayerHit(player.getPositionX(), player.getPositionY()))
        {
            player.loseLife();
        }

        // Chain kill
        checkChainKill();
    }

    // ==========================================
    // DRAW
    // ==========================================

    void draw(sf::RenderWindow& window)
    {
        // Draw platforms
        for (int i = 0; i < 6; i++)
        {
            window.draw(platforms[i]);
        }

        // Draw enemies
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
    // PLATFORM GETTER (partner needs this)
    // ==========================================

    sf::RectangleShape* getPlatforms()
    {
        return platforms;
    }

    int getPlatformCount()
    {
        return 6;
    }

    // ==========================================
    // DESTRUCTOR
    // ==========================================

    ~TestLevel()
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

    void setupPlatforms()
    {
        platforms[0].setSize(sf::Vector2f(600.f, 20.f));
        platforms[0].setFillColor(sf::Color(128, 0, 128));
        platforms[0].setPosition(0.f, 560.f);

        platforms[1].setSize(sf::Vector2f(250.f, 20.f));
        platforms[1].setFillColor(sf::Color(128, 0, 128));
        platforms[1].setPosition(0.f, 420.f);

        platforms[2].setSize(sf::Vector2f(250.f, 20.f));
        platforms[2].setFillColor(sf::Color(128, 0, 128));
        platforms[2].setPosition(350.f, 420.f);

        platforms[3].setSize(sf::Vector2f(300.f, 20.f));
        platforms[3].setFillColor(sf::Color(128, 0, 128));
        platforms[3].setPosition(150.f, 280.f);

        platforms[4].setSize(sf::Vector2f(200.f, 20.f));
        platforms[4].setFillColor(sf::Color(128, 0, 128));
        platforms[4].setPosition(0.f, 140.f);

        platforms[5].setSize(sf::Vector2f(200.f, 20.f));
        platforms[5].setFillColor(sf::Color(128, 0, 128));
        platforms[5].setPosition(400.f, 140.f);
    }

    void checkSnowballCollision(Snowball* snowball)
    {
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

    bool isPlayerHit(float playerX, float playerY)
    {
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

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

    void checkChainKill()
    {
        // Rolling Botom hits others
        for (int i = 0; i < BOTOM_COUNT; i++)
        {
            if (botoms[i] == nullptr ||
                botoms[i]->getIsDead() ||
                !botoms[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect rollingBounds(
                botoms[i]->getPositionX(),
                botoms[i]->getPositionY(),
                44.f, 44.f
            );

            for (int j = 0; j < BOTOM_COUNT; j++)
            {
                if (i == j || botoms[j] == nullptr ||
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

            for (int j = 0; j < TORNADO_COUNT; j++)
            {
                if (tornados[j] == nullptr ||
                    tornados[j]->getIsDead() ||
                    tornados[j]->getIsRolling())
                {
                    continue;
                }

                sf::FloatRect otherBounds(
                    tornados[j]->getPositionX(),
                    tornados[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    tornados[j]->instantKill();
                }
            }
        }

        // Rolling FlyingFoogo hits others
        for (int i = 0; i < FLYING_COUNT; i++)
        {
            if (flyingEnemies[i] == nullptr ||
                flyingEnemies[i]->getIsDead() ||
                !flyingEnemies[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect rollingBounds(
                flyingEnemies[i]->getPositionX(),
                flyingEnemies[i]->getPositionY(),
                44.f, 44.f
            );

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
        }

        // Rolling Tornado hits others
        for (int i = 0; i < TORNADO_COUNT; i++)
        {
            if (tornados[i] == nullptr ||
                tornados[i]->getIsDead() ||
                !tornados[i]->getIsRolling())
            {
                continue;
            }

            sf::FloatRect rollingBounds(
                tornados[i]->getPositionX(),
                tornados[i]->getPositionY(),
                44.f, 44.f
            );

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
    }
};