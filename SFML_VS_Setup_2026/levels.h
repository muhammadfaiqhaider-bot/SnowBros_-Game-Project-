<<<<<<< HEAD
#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "SnowBall.h"

class Level
{
protected:
    // ---- ENEMIES ----
    static const int MAX_ENEMIES = 20;
    Enemy* enemies[MAX_ENEMIES];
    int enemyCount;

    // ---- PLATFORMS ----
    static const int MAX_PLATFORMS = 10;
    sf::RectangleShape platforms[MAX_PLATFORMS];
    int platformCount;

    // ---- BACKGROUND ----
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

public:
    Level()
    {
        enemyCount = 0;
        platformCount = 0;
        backgroundLoaded = false;

        for (int i = 0; i < MAX_ENEMIES; i++)
            enemies[i] = nullptr;
    }

    // ---- CALLED BY SUBCLASS TO REGISTER ENEMIES ----
    void addEnemy(Enemy* e)
    {
        if (enemyCount < MAX_ENEMIES)
        {
            enemies[enemyCount] = e;
            enemyCount++;
        }
    }

    // ---- CALLED BY SUBCLASS TO REGISTER PLATFORMS ----
    void addPlatform(float x, float y, float w, float h,
        sf::Color color = sf::Color(128, 0, 128))
    {
        if (platformCount < MAX_PLATFORMS)
        {
            platforms[platformCount].setSize(sf::Vector2f(w, h));
            platforms[platformCount].setFillColor(color);
            platforms[platformCount].setPosition(x, y);
            platformCount++;
        }
    }

    void loadBackground(const std::string& path)
    {
        backgroundLoaded = backgroundTexture.loadFromFile(path);
        if (backgroundLoaded)
        {
            backgroundSprite.setTexture(backgroundTexture);
            float scaleX = 600.f / backgroundTexture.getSize().x;
            float scaleY = 600.f / backgroundTexture.getSize().y;
            backgroundSprite.setScale(scaleX, scaleY);
        }
    }

    // ==========================================
    // UPDATE â€” handles ALL enemy logic
    // ==========================================
    void update(float playerX, float playerY)
    {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            // Platform collision
            enemies[i]->setOnGround(false);

            for (int j = 0; j < platformCount; j++)
            {
                sf::FloatRect enemyBounds(
                    enemies[i]->getPositionX(),
                    enemies[i]->getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect platBounds = platforms[j].getGlobalBounds();

                if (enemyBounds.intersects(platBounds))
                {
                    float platTop = platforms[j].getPosition().y;
                    float enemyBot = enemies[i]->getPositionY() + 40.f;

                    if (enemyBot >= platTop &&
                        enemies[i]->getPositionY() < platTop &&
                        enemies[i]->getVelocityY() >= 0)
                    {
                        enemies[i]->setOnGround(true);
                        enemies[i]->snapToGround(platTop - 39.99f);
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

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            if (enemies[i]->getIsEncased() && !enemies[i]->getIsRolling())
            {
                sf::FloatRect enemyBounds(
                    enemies[i]->getPositionX(),
                    enemies[i]->getPositionY(),
                    40.f, 40.f
                );

                if (playerBounds.intersects(enemyBounds))
                {
                    int kickDir = (playerX > enemies[i]->getPositionX()) ? -1 : 1;
                    enemies[i]->kickRoll(kickDir);
                }
            }
        }

        // ---- ROLLING ENEMY CHAIN KILLS ----
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || !enemies[i]->getIsRolling())
                continue;

            sf::FloatRect rollingBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                44.f, 44.f
            );

            for (int j = 0; j < enemyCount; j++)
            {
                if (i == j || enemies[j] == nullptr ||
                    enemies[j]->getIsDead() || enemies[j]->getIsRolling())
                    continue;

                sf::FloatRect otherBounds(
                    enemies[j]->getPositionX(),
                    enemies[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    enemies[j]->instantKill();
                    enemies[j]->kickRoll(
                        enemies[i]->getRollVelocityX() > 0 ? 1 : -1
                    );
                }
            }
        }
    }

    // ==========================================
    // SNOWBALL COLLISION
    // ==========================================
    bool checkSnowballCollision(Snowball* snowball)
    {
        if (snowball == nullptr) return false;

        sf::FloatRect snowballBounds = snowball->getHitBox();

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            sf::FloatRect enemyBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (snowballBounds.intersects(enemyBounds))
            {
                if (enemies[i]->getIsEncased() && !enemies[i]->getIsRolling())
                {
                    enemies[i]->kickRoll(snowball->getDirection());
                    snowball->deactivate();
                    return true;
                }

                if (!enemies[i]->getIsEncased())
                {
                    enemies[i]->reduceHealth();
                    snowball->deactivate();
                    return true;
                }
            }
        }
        return false;
    }

    // ==========================================
    // PLAYER HIT CHECK
    // ==========================================
    bool isPlayerHit(float playerX, float playerY)
    {
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            if (enemies[i]->getIsEncased())
                continue;

            sf::FloatRect enemyBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (playerBounds.intersects(enemyBounds))
                return true;
        }
        return false;
    }

    // ==========================================
    // LEVEL COMPLETE
    // ==========================================
    bool isLevelComplete()
    {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr) continue;
            if (!enemies[i]->getIsDead()) return false;
        }
        return true;
    }

    // ==========================================
    // DRAW
    // ==========================================
    void draw(sf::RenderWindow& window)
    {
        if (backgroundLoaded)
            window.draw(backgroundSprite);

        for (int i = 0; i < platformCount; i++)
            window.draw(platforms[i]);

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] != nullptr && !enemies[i]->getIsDead())
                enemies[i]->DisplayEnemy(window);
        }
    }

    // Getter so GamePlay can still access platforms for Nick collision
    sf::RectangleShape* getPlatforms() { return platforms; }
    int getPlatformCount() { return platformCount; }

    virtual ~Level()
    {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] != nullptr)
            {
                delete enemies[i];
                enemies[i] = nullptr;
            }
        }
    }
=======
#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "SnowBall.h"

class Level
{
protected:
    // ---- ENEMIES ----
    static const int MAX_ENEMIES = 20;
    Enemy* enemies[MAX_ENEMIES];
    int enemyCount;

    // ---- PLATFORMS ----
    static const int MAX_PLATFORMS = 10;
    sf::RectangleShape platforms[MAX_PLATFORMS];
    int platformCount;

    // ---- BACKGROUND ----
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

public:
    Level()
    {
        enemyCount = 0;
        platformCount = 0;
        backgroundLoaded = false;

        for (int i = 0; i < MAX_ENEMIES; i++)
            enemies[i] = nullptr;
    }

    // ---- CALLED BY SUBCLASS TO REGISTER ENEMIES ----
    void addEnemy(Enemy* e)
    {
        if (enemyCount < MAX_ENEMIES)
        {
            enemies[enemyCount] = e;
            enemyCount++;
        }
    }

    // ---- CALLED BY SUBCLASS TO REGISTER PLATFORMS ----
    void addPlatform(float x, float y, float w, float h,
        sf::Color color = sf::Color(128, 0, 128))
    {
        if (platformCount < MAX_PLATFORMS)
        {
            platforms[platformCount].setSize(sf::Vector2f(w, h));
            platforms[platformCount].setFillColor(color);
            platforms[platformCount].setPosition(x, y);
            platformCount++;
        }
    }

    void loadBackground(const std::string& path)
    {
        backgroundLoaded = backgroundTexture.loadFromFile(path);
        if (backgroundLoaded)
        {
            backgroundSprite.setTexture(backgroundTexture);
            float scaleX = 600.f / backgroundTexture.getSize().x;
            float scaleY = 600.f / backgroundTexture.getSize().y;
            backgroundSprite.setScale(scaleX, scaleY);
        }
    }

    // ==========================================
    // UPDATE — handles ALL enemy logic
    // ==========================================
    void update(float playerX, float playerY)
    {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            // Platform collision
            enemies[i]->setOnGround(false);

            for (int j = 0; j < platformCount; j++)
            {
                sf::FloatRect enemyBounds(
                    enemies[i]->getPositionX(),
                    enemies[i]->getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect platBounds = platforms[j].getGlobalBounds();

                if (enemyBounds.intersects(platBounds))
                {
                    float platTop = platforms[j].getPosition().y;
                    float enemyBot = enemies[i]->getPositionY() + 40.f;

                    if (enemyBot >= platTop &&
                        enemies[i]->getPositionY() < platTop &&
                        enemies[i]->getVelocityY() >= 0)
                    {
                        enemies[i]->setOnGround(true);
                        enemies[i]->snapToGround(platTop - 39.99f);
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

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            if (enemies[i]->getIsEncased() && !enemies[i]->getIsRolling())
            {
                sf::FloatRect enemyBounds(
                    enemies[i]->getPositionX(),
                    enemies[i]->getPositionY(),
                    40.f, 40.f
                );

                if (playerBounds.intersects(enemyBounds))
                {
                    int kickDir = (playerX > enemies[i]->getPositionX()) ? -1 : 1;
                    enemies[i]->kickRoll(kickDir);
                }
            }
        }

        // ---- ROLLING ENEMY CHAIN KILLS ----
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || !enemies[i]->getIsRolling())
                continue;

            sf::FloatRect rollingBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                44.f, 44.f
            );

            for (int j = 0; j < enemyCount; j++)
            {
                if (i == j || enemies[j] == nullptr ||
                    enemies[j]->getIsDead() || enemies[j]->getIsRolling())
                    continue;

                sf::FloatRect otherBounds(
                    enemies[j]->getPositionX(),
                    enemies[j]->getPositionY(),
                    40.f, 40.f
                );

                if (rollingBounds.intersects(otherBounds))
                {
                    enemies[j]->instantKill();
                    enemies[j]->kickRoll(
                        enemies[i]->getRollVelocityX() > 0 ? 1 : -1
                    );
                }
            }
        }
    }

    // ==========================================
    // SNOWBALL COLLISION
    // ==========================================
    bool checkSnowballCollision(Snowball* snowball)
    {
        if (snowball == nullptr) return false;

        sf::FloatRect snowballBounds = snowball->getHitBox();

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            sf::FloatRect enemyBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (snowballBounds.intersects(enemyBounds))
            {
                if (enemies[i]->getIsEncased() && !enemies[i]->getIsRolling())
                {
                    enemies[i]->kickRoll(snowball->getDirection());
                    snowball->deactivate();
                    return true;
                }

                if (!enemies[i]->getIsEncased())
                {
                    enemies[i]->reduceHealth();
                    snowball->deactivate();
                    return true;
                }
            }
        }
        return false;
    }

    // ==========================================
    // PLAYER HIT CHECK
    // ==========================================
    bool isPlayerHit(float playerX, float playerY)
    {
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            if (enemies[i]->getIsEncased())
                continue;

            sf::FloatRect enemyBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (playerBounds.intersects(enemyBounds))
                return true;
        }
        return false;
    }

    // ==========================================
    // LEVEL COMPLETE
    // ==========================================
    bool isLevelComplete()
    {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr) continue;
            if (!enemies[i]->getIsDead()) return false;
        }
        return true;
    }

    // ==========================================
    // DRAW
    // ==========================================
    void draw(sf::RenderWindow& window)
    {
        if (backgroundLoaded)
            window.draw(backgroundSprite);

        for (int i = 0; i < platformCount; i++)
            window.draw(platforms[i]);

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] != nullptr && !enemies[i]->getIsDead())
                enemies[i]->DisplayEnemy(window);
        }
    }

    // Getter so GamePlay can still access platforms for Nick collision
    sf::RectangleShape* getPlatforms() { return platforms; }
    int getPlatformCount() { return platformCount; }

    virtual ~Level()
    {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] != nullptr)
            {
                delete enemies[i];
                enemies[i] = nullptr;
            }
        }
    }
>>>>>>> ffc21b01c7fd8b9993a5a0810c34c12abab3c75c
};