#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "Tornado.h"
#include "ColorVarients.h"
#include "Mogera.h"
#include "Gamakichi.h"

// ==========================================
// ENEMY SPAWN DATA
// ==========================================

struct EnemySpawn
{
    std::string type;       // "Botom", "FlyingFoogo", "Tornado" etc
    float x;
    float y;
};

// ==========================================
// LEVEL CONFIG
// ==========================================

struct LevelConfig
{
    int levelNumber;
    std::string backgroundFile;     // Background image path - u add images later
    int enemyCount;
    EnemySpawn enemySpawns[10];     // Max 10 enemies per level
    bool isBossLevel;
    bool isBonusLevel;
    std::string bossType;           // "Mogera" or "Gamakichi"
};

// ==========================================
// LEVEL MANAGER CLASS
// ==========================================

class LevelManager
{
private:
    LevelConfig levels[10];         // All 10 levels
    int currentLevel;               // 0-9 index
    int bonusLevel1;                // Random bonus level index
    int bonusLevel2;                // Random bonus level index

    // ---- Active enemies ----
    // Max 10 enemies per level - no vectors!
    Botom* botomEnemies[10];
    FlyingFoogaFoog* flyingEnemies[10];
    Tornado* tornadoEnemies[10];
    Mogera* mogera;
    Gamakichi* gamakichi;

    int botomCount;
    int flyingCount;
    int tornadoCount;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    LevelManager()
    {
        currentLevel = 0;
        mogera = nullptr;
        gamakichi = nullptr;

        botomCount = 0;
        flyingCount = 0;
        tornadoCount = 0;

        // Initialize enemy arrays
        for (int i = 0; i < 10; i++)
        {
            botomEnemies[i] = nullptr;
            flyingEnemies[i] = nullptr;
            tornadoEnemies[i] = nullptr;
        }

        // Pick 2 random bonus levels (non-boss levels only)
        srand(time(0));
        bonusLevel1 = rand() % 4;           // Level 1-4 (index 0-3)
        bonusLevel2 = rand() % 4;
        while (bonusLevel2 == bonusLevel1)
        {
            bonusLevel2 = rand() % 4;       // Make sure they are different
        }

        setupAllLevels();
    }

    // ==========================================
    // SETUP ALL 10 LEVELS
    // ==========================================

    void setupAllLevels()
    {
        // ---- LEVEL 1 ---- Intro level slow enemies simple layout
        levels[0].levelNumber = 1;
        levels[0].backgroundFile = "assets/bg_level1.png";     // u add this image
        levels[0].isBossLevel = false;
        levels[0].isBonusLevel = (bonusLevel1 == 0 || bonusLevel2 == 0);
        levels[0].bossType = "";
        levels[0].enemyCount = 3;
        levels[0].enemySpawns[0] = { "Botom", 150.f, 520.f };
        levels[0].enemySpawns[1] = { "Botom", 350.f, 520.f };
        levels[0].enemySpawns[2] = { "Botom", 500.f, 520.f };

        // ---- LEVEL 2 ---- More enemies slightly faster
        levels[1].levelNumber = 2;
        levels[1].backgroundFile = "assets/bg_level2.png";
        levels[1].isBossLevel = false;
        levels[1].isBonusLevel = (bonusLevel1 == 1 || bonusLevel2 == 1);
        levels[1].bossType = "";
        levels[1].enemyCount = 4;
        levels[1].enemySpawns[0] = { "Botom",      150.f, 520.f };
        levels[1].enemySpawns[1] = { "Botom",      400.f, 520.f };
        levels[1].enemySpawns[2] = { "FlyingFoogo", 250.f, 240.f };
        levels[1].enemySpawns[3] = { "FlyingFoogo", 450.f, 100.f };

        // ---- LEVEL 3 ---- Mixed enemy types
        levels[2].levelNumber = 3;
        levels[2].backgroundFile = "assets/bg_level3.png";
        levels[2].isBossLevel = false;
        levels[2].isBonusLevel = (bonusLevel1 == 2 || bonusLevel2 == 2);
        levels[2].bossType = "";
        levels[2].enemyCount = 5;
        levels[2].enemySpawns[0] = { "Botom",      100.f, 520.f };
        levels[2].enemySpawns[1] = { "Botom",      450.f, 380.f };
        levels[2].enemySpawns[2] = { "FlyingFoogo", 300.f, 240.f };
        levels[2].enemySpawns[3] = { "Tornado",     200.f, 100.f };
        levels[2].enemySpawns[4] = { "Botom",      500.f, 520.f };

        // ---- LEVEL 4 ---- Faster enemies complex layout
        levels[3].levelNumber = 4;
        levels[3].backgroundFile = "assets/bg_level4.png";
        levels[3].isBossLevel = false;
        levels[3].isBonusLevel = (bonusLevel1 == 3 || bonusLevel2 == 3);
        levels[3].bossType = "";
        levels[3].enemyCount = 6;
        levels[3].enemySpawns[0] = { "Botom",      100.f, 520.f };
        levels[3].enemySpawns[1] = { "Botom",      300.f, 520.f };
        levels[3].enemySpawns[2] = { "FlyingFoogo", 200.f, 380.f };
        levels[3].enemySpawns[3] = { "Tornado",     400.f, 240.f };
        levels[3].enemySpawns[4] = { "Tornado",     100.f, 100.f };
        levels[3].enemySpawns[5] = { "FlyingFoogo", 500.f, 380.f };

        // ---- LEVEL 5 ---- BOSS LEVEL - Mogera
        levels[4].levelNumber = 5;
        levels[4].backgroundFile = "assets/bg_boss1.png";
        levels[4].isBossLevel = true;
        levels[4].isBonusLevel = false;
        levels[4].bossType = "Mogera";
        levels[4].enemyCount = 2;
        levels[4].enemySpawns[0] = { "Botom", 100.f, 520.f };    // Some regular enemies too
        levels[4].enemySpawns[1] = { "Botom", 200.f, 380.f };

        // ---- LEVEL 6 ---- Green variants appear
        levels[5].levelNumber = 6;
        levels[5].backgroundFile = "assets/bg_level6.png";
        levels[5].isBossLevel = false;
        levels[5].isBonusLevel = false;
        levels[5].bossType = "";
        levels[5].enemyCount = 5;
        levels[5].enemySpawns[0] = { "GreenBotom",      150.f, 520.f };
        levels[5].enemySpawns[1] = { "GreenBotom",      400.f, 520.f };
        levels[5].enemySpawns[2] = { "GreenFlyingFoogo", 300.f, 380.f };
        levels[5].enemySpawns[3] = { "GreenTornado",     200.f, 240.f };
        levels[5].enemySpawns[4] = { "GreenBotom",      500.f, 380.f };

        // ---- LEVEL 7 ---- Green and Blue variants
        levels[6].levelNumber = 7;
        levels[6].backgroundFile = "assets/bg_level7.png";
        levels[6].isBossLevel = false;
        levels[6].isBonusLevel = false;
        levels[6].bossType = "";
        levels[6].enemyCount = 6;
        levels[6].enemySpawns[0] = { "GreenBotom",     100.f, 520.f };
        levels[6].enemySpawns[1] = { "BlueBotom",      350.f, 520.f };
        levels[6].enemySpawns[2] = { "GreenFlyingFoogo", 200.f, 380.f };
        levels[6].enemySpawns[3] = { "BlueFlyingFoogo",  450.f, 240.f };
        levels[6].enemySpawns[4] = { "GreenTornado",    150.f, 100.f };
        levels[6].enemySpawns[5] = { "BlueTornado",     500.f, 380.f };

        // ---- LEVEL 8 ---- Blue variants mostly
        levels[7].levelNumber = 8;
        levels[7].backgroundFile = "assets/bg_level8.png";
        levels[7].isBossLevel = false;
        levels[7].isBonusLevel = false;
        levels[7].bossType = "";
        levels[7].enemyCount = 7;
        levels[7].enemySpawns[0] = { "BlueBotom",      100.f, 520.f };
        levels[7].enemySpawns[1] = { "BlueBotom",      300.f, 520.f };
        levels[7].enemySpawns[2] = { "BlueFlyingFoogo", 200.f, 380.f };
        levels[7].enemySpawns[3] = { "BlueTornado",     400.f, 240.f };
        levels[7].enemySpawns[4] = { "PurpleBotom",    500.f, 520.f };
        levels[7].enemySpawns[5] = { "BlueFlyingFoogo", 100.f, 100.f };
        levels[7].enemySpawns[6] = { "BlueTornado",     350.f, 100.f };

        // ---- LEVEL 9 ---- Purple variants high speed
        levels[8].levelNumber = 9;
        levels[8].backgroundFile = "assets/bg_level9.png";
        levels[8].isBossLevel = false;
        levels[8].isBonusLevel = false;
        levels[8].bossType = "";
        levels[8].enemyCount = 8;
        levels[8].enemySpawns[0] = { "PurpleBotom",      100.f, 520.f };
        levels[8].enemySpawns[1] = { "PurpleBotom",      300.f, 520.f };
        levels[8].enemySpawns[2] = { "PurpleFlyingFoogo", 200.f, 380.f };
        levels[8].enemySpawns[3] = { "PurpleTornado",     400.f, 240.f };
        levels[8].enemySpawns[4] = { "PurpleBotom",      500.f, 380.f };
        levels[8].enemySpawns[5] = { "PurpleFlyingFoogo", 150.f, 100.f };
        levels[8].enemySpawns[6] = { "PurpleTornado",     450.f, 100.f };
        levels[8].enemySpawns[7] = { "PurpleBotom",      350.f, 520.f };

        // ---- LEVEL 10 ---- FINAL BOSS - Gamakichi
        levels[9].levelNumber = 10;
        levels[9].backgroundFile = "assets/bg_boss2.png";
        levels[9].isBossLevel = true;
        levels[9].isBonusLevel = false;
        levels[9].bossType = "Gamakichi";
        levels[9].enemyCount = 3;
        levels[9].enemySpawns[0] = { "PurpleBotom",  100.f, 520.f };
        levels[9].enemySpawns[1] = { "PurpleTornado", 300.f, 380.f };
        levels[9].enemySpawns[2] = { "PurpleBotom",  500.f, 520.f };
    }

    // ==========================================
    // LOAD LEVEL - spawn enemies
    // ==========================================

    void loadLevel(int levelIndex)
    {
        currentLevel = levelIndex;

        // Clear old enemies first
        clearEnemies();

        botomCount = 0;
        flyingCount = 0;
        tornadoCount = 0;

        LevelConfig& config = levels[levelIndex];

        // Spawn enemies based on config
        for (int i = 0; i < config.enemyCount; i++)
        {
            std::string type = config.enemySpawns[i].type;
            float x = config.enemySpawns[i].x;
            float y = config.enemySpawns[i].y;

            spawnEnemy(type, x, y);
        }

        // Spawn boss if boss level
        if (config.isBossLevel)
        {
            if (config.bossType == "Mogera")
            {
                mogera = new Mogera(390.f, 340.f);
            }
            else if (config.bossType == "Gamakichi")
            {
                gamakichi = new Gamakichi(400.f, 20.f);
            }
        }

        // Load background image
        if (backgroundTexture.loadFromFile(config.backgroundFile))
        {
            backgroundSprite.setTexture(backgroundTexture);
        }
    }

    // ==========================================
    // UPDATE ALL ENEMIES
    // ==========================================

    void updateEnemies(float playerX, float playerY,
        sf::RectangleShape platforms[], int platformCount)
    {
        // Update botoms
        for (int i = 0; i < botomCount; i++)
        {
            if (botomEnemies[i] != nullptr)
            {
                updateBotomPhysics(*botomEnemies[i], platforms, platformCount);
                botomEnemies[i]->movementsUpdate();
            }
        }

        // Update flying enemies
        for (int i = 0; i < flyingCount; i++)
        {
            if (flyingEnemies[i] != nullptr)
            {
                updateFlyingPhysics(*flyingEnemies[i], platforms, platformCount);
                flyingEnemies[i]->movementsUpdate();
            }
        }

        // Update tornados
        for (int i = 0; i < tornadoCount; i++)
        {
            if (tornadoEnemies[i] != nullptr)
            {
                tornadoEnemies[i]->setPlayerPosition(playerX, playerY);
                updateFlyingPhysics(*tornadoEnemies[i], platforms, platformCount);
                tornadoEnemies[i]->movementsUpdate();
            }
        }

        // Update boss
        if (mogera != nullptr)
        {
            mogera->setPlayerPosition(playerX, playerY);
            mogera->movementsUpdate();
        }

        if (gamakichi != nullptr)
        {
            gamakichi->setPlayerPosition(playerX, playerY);
            gamakichi->movementsUpdate();
        }
    }

    // ==========================================
    // DRAW ALL ENEMIES
    // ==========================================

    void draw(sf::RenderWindow& window)
    {
        // Draw background
        window.draw(backgroundSprite);

        // Draw botoms
        for (int i = 0; i < botomCount; i++)
        {
            if (botomEnemies[i] != nullptr)
            {
                botomEnemies[i]->DisplayEnemy(window);
            }
        }

        // Draw flying enemies
        for (int i = 0; i < flyingCount; i++)
        {
            if (flyingEnemies[i] != nullptr)
            {
                flyingEnemies[i]->DisplayEnemy(window);
            }
        }

        // Draw tornados
        for (int i = 0; i < tornadoCount; i++)
        {
            if (tornadoEnemies[i] != nullptr)
            {
                tornadoEnemies[i]->DisplayEnemy(window);
            }
        }

        // Draw boss
        if (mogera != nullptr)
        {
            mogera->DisplayEnemy(window);
        }

        if (gamakichi != nullptr)
        {
            gamakichi->DisplayEnemy(window);
        }
    }

    // ==========================================
    // CHECK LEVEL COMPLETE
    // ==========================================

    bool isLevelComplete()
    {
        // Check all botoms defeated
        for (int i = 0; i < botomCount; i++)
        {
            if (botomEnemies[i] != nullptr && !botomEnemies[i]->getEncaedStatusBot())
            {
                return false;
            }
        }

        // Check all flying enemies defeated
        for (int i = 0; i < flyingCount; i++)
        {
            if (flyingEnemies[i] != nullptr && !flyingEnemies[i]->getEncaedStatusFlying())
            {
                return false;
            }
        }

        // Check all tornados defeated
        for (int i = 0; i < tornadoCount; i++)
        {
            if (tornadoEnemies[i] != nullptr && !tornadoEnemies[i]->getEncaedStatusTor())
            {
                return false;
            }
        }

        // Check boss defeated
        if (mogera != nullptr && !mogera->getIsDefeated())
        {
            return false;
        }

        if (gamakichi != nullptr && !gamakichi->getIsDefeated())
        {
            return false;
        }

        return true;    // All enemies defeated!
    }

    // ==========================================
    // NEXT LEVEL
    // ==========================================

    bool goNextLevel()
    {
        if (currentLevel < 9)
        {
            currentLevel++;
            loadLevel(currentLevel);
            return true;        // More levels available
        }

        return false;           // Game complete!
    }

    // Getters
    int getCurrentLevelNumber()
    {
        return levels[currentLevel].levelNumber;
    }

    bool currentIsBossLevel()
    {
        return levels[currentLevel].isBossLevel;
    }

    bool currentIsBonusLevel()
    {
        return levels[currentLevel].isBonusLevel;
    }

    std::string getCurrentBackground()
    {
        return levels[currentLevel].backgroundFile;
    }

    // ==========================================
    // DESTRUCTOR
    // ==========================================

    ~LevelManager()
    {
        clearEnemies();
    }

private:

    // ==========================================
    // SPAWN ENEMY BY TYPE
    // ==========================================

    void spawnEnemy(std::string type, float x, float y)
    {
        if (type == "Botom" || type == "GreenBotom" ||
            type == "BlueBotom" || type == "PurpleBotom")
        {
            if (botomCount < 10)
            {
                if (type == "Botom")
                    botomEnemies[botomCount] = new Botom(x, y);
                else if (type == "GreenBotom")
                    botomEnemies[botomCount] = new GreenBotom(x, y);
                else if (type == "BlueBotom")
                    botomEnemies[botomCount] = new BlueBotom(x, y);
                else if (type == "PurpleBotom")
                    botomEnemies[botomCount] = new PurpleBotom(x, y);

                botomCount++;
            }
        }
        else if (type == "FlyingFoogo" || type == "GreenFlyingFoogo" ||
            type == "BlueFlyingFoogo" || type == "PurpleFlyingFoogo")
        {
            if (flyingCount < 10)
            {
                if (type == "FlyingFoogo")
                    flyingEnemies[flyingCount] = new FlyingFoogaFoog(x, y);
                else if (type == "GreenFlyingFoogo")
                    flyingEnemies[flyingCount] = new GreenFlyingFoogo(x, y);
                else if (type == "BlueFlyingFoogo")
                    flyingEnemies[flyingCount] = new BlueFlyingFoogo(x, y);
                else if (type == "PurpleFlyingFoogo")
                    flyingEnemies[flyingCount] = new PurpleFlyingFoogo(x, y);

                flyingCount++;
            }
        }
        else if (type == "Tornado" || type == "GreenTornado" ||
            type == "BlueTornado" || type == "PurpleTornado")
        {
            if (tornadoCount < 10)
            {
                if (type == "Tornado")
                    tornadoEnemies[tornadoCount] = new Tornado(x, y);
                else if (type == "GreenTornado")
                    tornadoEnemies[tornadoCount] = new GreenTornado(x, y);
                else if (type == "BlueTornado")
                    tornadoEnemies[tornadoCount] = new BlueTornado(x, y);
                else if (type == "PurpleTornado")
                    tornadoEnemies[tornadoCount] = new PurpleTornado(x, y);

                tornadoCount++;
            }
        }
    }

    // ==========================================
    // PHYSICS HELPERS
    // ==========================================

    void updateBotomPhysics(Botom& botom,
        sf::RectangleShape platforms[],
        int platformCount)
    {
        botom.setOnGround(false);

        for (int i = 0; i < platformCount; i++)
        {
            sf::FloatRect botomBounds(botom.getPositionX(), botom.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (botomBounds.intersects(platformBounds))
            {
                float platformTop = platforms[i].getPosition().y;
                float botomBottom = botom.getPositionY() + 40.f;

                if (botomBottom >= platformTop && botom.getPositionY() < platformTop && botom.getVelocityY() >= 0)
                {
                    botom.setOnGround(true);
                    botom.snapToGround(platformTop - 39.99f);
                }
            }
        }

        if (botom.getPositionX() <= 0 || botom.getPositionX() >= 560)
        {
            botom.setHitWall(true);
        }
    }

    void updateFlyingPhysics(FlyingFoogaFoog& enemy,
        sf::RectangleShape platforms[],
        int platformCount)
    {
        enemy.setOnGround(false);

        for (int i = 0; i < platformCount; i++)
        {
            sf::FloatRect enemyBounds(enemy.getPositionX(), enemy.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (enemyBounds.intersects(platformBounds))
            {
                float platformTop = platforms[i].getPosition().y;

                if (enemy.getPositionY() + 40.f <= platformTop + 10.f)
                {
                    enemy.setOnGround(true);
                    enemy.snapToGround(platformTop - 40.f);
                }
            }
        }
    }

    // ==========================================
    // CLEAR ALL ENEMIES
    // ==========================================

    void clearEnemies()
    {
        for (int i = 0; i < 10; i++)
        {
            if (botomEnemies[i] != nullptr)
            {
                delete botomEnemies[i];
                botomEnemies[i] = nullptr;
            }

            if (flyingEnemies[i] != nullptr)
            {
                delete flyingEnemies[i];
                flyingEnemies[i] = nullptr;
            }

            if (tornadoEnemies[i] != nullptr)
            {
                delete tornadoEnemies[i];
                tornadoEnemies[i] = nullptr;
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

        botomCount = 0;
        flyingCount = 0;
        tornadoCount = 0;
    }
};