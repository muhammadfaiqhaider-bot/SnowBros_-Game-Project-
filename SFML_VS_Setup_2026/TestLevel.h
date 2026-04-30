#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "Tornado.h"
#include "CollisionManager.h"
#include "Nick.h"
#include "SnowBall.h"
#include "Mogera.h"
#include "Gamakichi.h"

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
    Mogera* mogera;
    Gamakichi* gamakichi;

public:
    TestLevel()
    {
        setupPlatforms();

        // Spawn enemies
        botoms[0] = new Botom(150.f, 520.f);
        botoms[1] = new Botom(400.f, 380.f);

        flyingEnemies[0] = new FlyingFoogaFoog(300.f, 240.f);

        tornados[0] = new Tornado(450.f, 100.f);

        mogera = new Mogera(230.f, 200.f);

        gamakichi = new Gamakichi(400.f, 20.f);

    }


    // ==========================================
    // UPDATE
    // ==========================================

    void update(Nick& player)
    {
        // Player collision
        CollisionManager::handlePlayer(player, platforms, 6);



        // MogeraChild platform collision
        for (int i = 0; i < mogera->getNumberOfChilds(); i++)
        {
            if (mogera->getChildX(i) < 0)
            {
                continue;
            }

            for (int j = 0; j < 6; j++)
            {
                sf::FloatRect childBounds(
                    mogera->getChildX(i),
                    mogera->getChildY(i),
                    20.f, 20.f
                );
                sf::FloatRect platformBounds = platforms[j].getGlobalBounds();

                if (childBounds.intersects(platformBounds))
                {
                    float platformTop = platforms[j].getPosition().y;
                    float childBottom = mogera->getChildY(i) + 20.f;

                    if (childBottom >= platformTop &&
                        mogera->getChildY(i) < platformTop)
                    {
                        mogera->setChildOnGround(i, true);
                        mogera->snapChildToGround(i, platformTop - 20.f);
                    }
                }
            }
        }

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


        // Mogera update
        if (mogera != nullptr && !mogera->getIsDefeated())
        {
            // Platform collision for Mogera
            mogera->setOnGround(false);

            for (int i = 0; i < 6; i++)
            {
                sf::FloatRect mogeraBounds(
                    mogera->getPositionX(),
                    mogera->getPositionY(),
                    80.f, 80.f
                );
                sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

                if (mogeraBounds.intersects(platformBounds))
                {
                    float platformTop = platforms[i].getPosition().y;
                    float mogeraBottom = mogera->getPositionY() + 80.f;

                    if (mogeraBottom >= platformTop &&
                        mogera->getPositionY() < platformTop)
                    {
                        mogera->setOnGround(true);
                        mogera->snapToGround(platformTop - 80.f);
                    }
                }
            }

            mogera->setPlayerPosition(
                player.getPositionX(),
                player.getPositionY()
            );

            // Reset child onGround before platform check
            for (int i = 0; i < mogera->getNumberOfChilds(); i++)
            {
                if (mogera->getChildX(i) < 0)
                {
                    continue;
                }
                mogera->setChildOnGround(i, false);  // Reset first!
            }

            mogera->movementsUpdate();
        }

        // Gamakichi update
        if (gamakichi != nullptr && !gamakichi->getIsDefeated())
        {
            // Platform collision for Gamakichi
            gamakichi->setOnGround(false);

            for (int i = 0; i < 6; i++)
            {
                sf::FloatRect gamakichiBounds(
                    gamakichi->getPositionX(),
                    gamakichi->getPositionY(),
                    100.f, 120.f        // Gamakichi size
                );
                sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

                if (gamakichiBounds.intersects(platformBounds))
                {
                    float platformTop = platforms[i].getPosition().y;
                    float gamakichiBottom = gamakichi->getPositionY() + 120.f;

                    if (gamakichiBottom >= platformTop &&
                        gamakichi->getPositionY() < platformTop)
                    {
                        gamakichi->setOnGround(true);
                        gamakichi->snapToGround(platformTop - 120.f);
                    }
                }
            }

            gamakichi->setPlayerPosition(
                player.getPositionX(),
                player.getPositionY()
            );
            gamakichi->movementsUpdate();
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

        if (mogera != nullptr && !mogera->getIsDefeated())
        {
            mogera->DisplayEnemy(window);
        }

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

        if (gamakichi != nullptr && !gamakichi->getIsDefeated())
        {
            gamakichi->DisplayEnemy(window);
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

        if (mogera != nullptr)
        {
            delete mogera;
            mogera = nullptr;
        }

        if (gamakichi != nullptr)
        {
            delete gamakichi;
            gamakichi = nullptr;
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

        // Check Mogera
        if (mogera != nullptr && !mogera->getIsDefeated())
        {
            sf::FloatRect mogeraBounds(
                mogera->getPositionX(),
                mogera->getPositionY(),
                80.f, 80.f      // Mogera is bigger
            );

            if (snowballBounds.intersects(mogeraBounds))
            {
                mogera->reduceHealth();
                snowball->deactivate();
                return;
            }
        }
        // Check MogeraChild
        if (mogera != nullptr && !mogera->getIsDefeated())
        {
            for (int i = 0; i < mogera->getNumberOfChilds(); i++)
            {
                if (mogera->getChildX(i) < 0)
                {
                    continue;
                }

                sf::FloatRect childBounds(
                    mogera->getChildX(i),
                    mogera->getChildY(i),
                    20.f, 20.f
                );

                if (snowballBounds.intersects(childBounds))
                {
                    // Deactivate child on snowball hit
                    mogera->deactivateChild(i);
                    snowball->deactivate();
                    return;
                }
            }
        }

        // Check Gamakichi
        if (gamakichi != nullptr && !gamakichi->getIsDefeated())
        {
            sf::FloatRect gamakichiBounds(
                gamakichi->getPositionX(),
                gamakichi->getPositionY(),
                100.f, 120.f
            );

            if (snowballBounds.intersects(gamakichiBounds))
            {
                gamakichi->reduceHealth();
                snowball->deactivate();
                return;
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


        // Mogera children can hit player
        if (mogera != nullptr && !mogera->getIsDefeated())
        {
            // Check Mogera children
            for (int i = 0; i < mogera->getNumberOfChilds(); i++)
            {
                if (mogera->getChildX(i) < 0)
                {
                    continue;
                }

                sf::FloatRect childBounds(
                    mogera->getChildX(i),
                    mogera->getChildY(i),
                    20.f, 20.f
                );

                if (playerBounds.intersects(childBounds))
                {
                    return true;
                }
            }
        }

        // ✅ FIX: Use playerX and playerY parameters (not player object)
        // Gamakichi rockets hit player
        if (gamakichi != nullptr && !gamakichi->getIsDefeated())
        {
            if (gamakichi->isRocketHittingPlayer(playerX, playerY))
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

            // ✅ FlyingFooga rolling kills Tornado
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