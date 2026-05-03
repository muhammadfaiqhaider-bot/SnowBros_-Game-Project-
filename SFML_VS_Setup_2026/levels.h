#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "SnowBall.h"
#include "Tornado.h"
#include "PLayer.h"
#include "Mogera.h"

#include "PowerUps.h"
#include "SpeedBoost.h"
#include "SnowballPower.h"
#include "DistanceIncreasePowerUp.h"
#include "Star.h"

class Level
{
protected:
    //  ENEMIES 
    static const int MAX_ENEMIES = 20;
    Enemy* enemies[MAX_ENEMIES];
    int enemyCount;
    bool prevAlive[MAX_ENEMIES];

    //  PLATFORMS 
    static const int MAX_PLATFORMS = 10;
    sf::RectangleShape platforms[MAX_PLATFORMS];
    int platformCount;

    // BACKGROUND 
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

    // POWERUPS
    static const int MAX_POWERUPS = 6;
    PowerUps* activePowerUps[MAX_POWERUPS];
    int powerupCount;

    sf::Texture platformTexture;
    bool platformTextureLoaded;
    sf::Color enemyTint;
    // Per-level music
    sf::Music levelMusic;
    bool levelMusicLoaded;


public:
    Level()
    {
        enemyCount = 0;
        platformCount = 0;
        backgroundLoaded = false;

        powerupCount = 0;

        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            enemies[i] = nullptr;
            prevAlive[i] = false;
        }
        for (int i = 0; i < MAX_POWERUPS; i++)
            activePowerUps[i] = nullptr;
        platformTextureLoaded = false;
        enemyTint = sf::Color::White;
        levelMusicLoaded = false;

        srand((unsigned)time(NULL));
    }

    // Spawn a Star power-up on a random platform in this level
    void spawnStarOnRandomPlatform()
    {
        if (powerupCount >= MAX_POWERUPS) return;
        if (platformCount <= 0) return;

        int pidx = rand() % platformCount;
        sf::Vector2f pos = platforms[pidx].getPosition();
        sf::Vector2f size = platforms[pidx].getSize();
        float sx = pos.x + (size.x / 2.f) - 7.f; // center the star
        float sy = pos.y - 20.f; // just above platform

        Star* s = new Star(sx, sy);
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (activePowerUps[i] == nullptr) {
                activePowerUps[i] = s;
                powerupCount++;
                break;
            }
        }
    }

    // Load a music file for this level. Call from the Level subclass constructor.
    void loadLevelMusic(const std::string& path)
    {
        levelMusicLoaded = levelMusic.openFromFile(path);
        if (levelMusicLoaded)
        {
            levelMusic.setLoop(true);
            levelMusic.setVolume(20.f);
        }
    }

    void playLevelMusic()
    {
        if (levelMusicLoaded && levelMusic.getStatus() != sf::Music::Playing)
            levelMusic.play();
    }

    void stopLevelMusic()
    {
        if (levelMusicLoaded && levelMusic.getStatus() == sf::Music::Playing)
            levelMusic.stop();
    }

    void loadPlatformTexture(const std::string& path)
    {
        platformTextureLoaded = platformTexture.loadFromFile(path);
        if (platformTextureLoaded)
            platformTexture.setRepeated(true);  // enables tiling
    }


    void addEnemy(Enemy* e) // association here 
    {
        if (enemyCount < MAX_ENEMIES)
        {
            enemies[enemyCount] = e;
            prevAlive[enemyCount] = true; 
            enemyCount++;
        }
    }

    void setEnemyTint(sf::Color color)
    {
        enemyTint = color;
    }

  
    void addPlatform(float x, float y, float w, float h,
        sf::Color color = sf::Color::White)
    {
        if (platformCount < MAX_PLATFORMS)
        {
            platforms[platformCount].setSize(sf::Vector2f(w, h));
            platforms[platformCount].setFillColor(color);
            platforms[platformCount].setPosition(x, y);
            platformCount++;
        }
        if (platformTextureLoaded)
        {

            int idx = platformCount - 1;
            if (idx >= 0 && idx < MAX_PLATFORMS) {
                platforms[idx].setFillColor(sf::Color::White);
                platforms[idx].setTexture(&platformTexture);

                platforms[idx].setTextureRect(sf::IntRect(0, 0, (int)w, (int)h));
            }
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


    void update(float playerX, float playerY, Player* player = nullptr)
    {
        int deathsThisFrame = 0;
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;


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


            enemies[i]->setPlayerPosition(playerX, playerY);

            // Special handling: if this enemy is a Mogera, snap its children to platforms
            Mogera* mog = nullptr;
            mog = dynamic_cast<Mogera*>(enemies[i]);
            if (mog != nullptr)
            {
                for (int ci = 0; ci < mog->getNumberOfChilds(); ci++)
                {
                    float cx = mog->getChildX(ci);
                    float cy = mog->getChildY(ci);

                    if (cx < 0 || cy < 0) continue; // invalid / inactive

                    // Clear onGround; will be set true if a platform is beneath this frame
                    mog->setChildOnGround(ci, false);

                    for (int j = 0; j < platformCount; j++)
                    {
                        sf::FloatRect childBounds(
                            cx,
                            cy,
                            20.f, 20.f
                        );
                        sf::FloatRect platBounds = platforms[j].getGlobalBounds();

                        if (childBounds.intersects(platBounds))
                        {
                            float platTop = platforms[j].getPosition().y;
                            float childBot = cy + 20.f;

                            // Only snap when child is falling onto the platform
                            // Use child's vertical velocity to ensure downward movement
                            // We need a safe getter; MogeraChild::getVelocityY() provides it
                            // If child's bottom reached platform top and it was above it and moving downwards
                            if (childBot >= platTop && cy < platTop)
                            {
                                mog->snapChildToGround(ci, platTop - 19.99f);
                                mog->setChildOnGround(ci, true);
                            }
                        }
                    }
                }
            }

            enemies[i]->movementsUpdate();
        }

        // PLAYER KICKS ENCASED ENEMY 
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

        // ROLLING ENEMY CHAIN KILS ----
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


        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr)
            {
                continue;
            }

            bool nowDead = enemies[i]->getIsDead();

            // Use prevAlive array to determine if this enemy transitioned to dead this frame
            bool wasAlive = prevAlive[i];

            if (wasAlive && nowDead)
            {
                // Award points based on enemy type
                std::string n = enemies[i]->getName();

                int baseScore = 0;
                int gems = 0;

                if (n.find("Botom") != std::string::npos)
                {
                    baseScore = 100 + (rand() % 401); // 100-500
                    gems = 5;
                }
                else if (n.find("FlyingFooga") != std::string::npos || n.find("FlyingFoogo") != std::string::npos)
                {
                    baseScore = 200 + (rand() % 601); // 200-800
                    gems = 8;
                }
                else if (n.find("Tornado") != std::string::npos)
                {
                    baseScore = 300 + (rand() % 901); // 300-1200
                    gems = 12;
                }
                else if (n.find("Mogera") != std::string::npos)
                {
                    baseScore = 5000;
                    gems = 200;
                }
                else if (n.find("Gamakichi") != std::string::npos)
                {
                    baseScore = 10000;
                    gems = 500;
                }

                deathsThisFrame++;
                float multiplier = 1.0f + 0.1f * (deathsThisFrame - 1); // +10% per chain
                int finalScore = (int)(baseScore * multiplier);

                if (player != nullptr)
                {
                    if (finalScore > 0)
                        player->addScore(finalScore);
                    if (gems > 0)
                        player->addGems((int)(gems * multiplier));
                }

                // 30% chance to spawn a power-up at this enemy's position
                if (rand() % 100 < 30)
                {
                    spawnPowerUp(enemies[i]->getPositionX(), enemies[i]->getPositionY());
                }
            }

            prevAlive[i] = !nowDead;
        }

        // Update active power-ups (physics + platform snap)
        for (int pi = 0; pi < MAX_POWERUPS; pi++)
        {
            PowerUps* p = activePowerUps[pi];
            if (p == nullptr) continue;
            if (!p->getActiveStatus()) continue;

            // Apply gravity / custom update
            p->updatePowerUp();

            // Snap to platforms or ground
            sf::FloatRect pb = p->getHitBox();
            // ground check
            if (p->getY() + 20.f >= 560.f)
            {
                p->setPosition(p->getX(), 560.f - 20.f);
                p->setVelocityY(0.f);
                continue;
            }

            for (int j = 0; j < platformCount; j++)
            {
                sf::FloatRect platBounds = platforms[j].getGlobalBounds();
                if (pb.intersects(platBounds))
                {
                    // place on top of platform
                    p->setPosition(p->getX(), platforms[j].getPosition().y - 20.f);
                    p->setVelocityY(0.f);
                    break;
                }
            }
        }

        // If player is present, check for powerup collection
        if (player != nullptr)
        {
            for (int pi = 0; pi < MAX_POWERUPS; pi++)
            {
                PowerUps* p = activePowerUps[pi];
                if (p == nullptr) continue;
                if (!p->getActiveStatus()) continue;

                sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);
                if (playerBounds.intersects(p->getHitBox()))
                {
                    // Show a message to player indicating which power-up was picked
                    if (player != nullptr)
                    {
                        std::string msg = std::string("Picked: ") + p->getName();
                        player->showPowerupMessage(msg, 120);
                    }

                    p->applyEffect(*player);
                    p->collect();
                    delete p;
                    activePowerUps[pi] = nullptr;
                    powerupCount--;
                }
            }
        }


        // ENEMY PROJECTILE HITS ( Tornado knives)

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            if (player != nullptr)
            {
                if (enemies[i]->isProjectileHittingPlayer(playerX, playerY) || enemies[i]->isKnifeHittingPlayer(playerX, playerY))
                {
                    player->loseLife();
                }
            }
        }

        // Also check Mogera child collisions and notify player
        if (player != nullptr)
        {
            for (int i = 0; i < enemyCount; i++)
            {
                Mogera* mog = dynamic_cast<Mogera*>(enemies[i]);
                if (mog == nullptr) continue;

                for (int ci = 0; ci < mog->getNumberOfChilds(); ci++)
                {
                    if (!mog->isChildActive(ci)) continue;
                    // If child hits player, damage and deactivate child
                    if (mog->isChildActive(ci))
                    {
                        // Use bounds intersection same as earlier
                        sf::FloatRect childBounds(mog->getChildX(ci), mog->getChildY(ci), 20.f, 20.f);
                        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);
                        if (childBounds.intersects(playerBounds))
                        {
                            mog->deactivateChild(ci);
                            player->loseLife();
                        }
                    }
                }
            }
        }
    }


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

        // Special if this enemy is a Mogera, check its children too
        for (int i = 0; i < enemyCount; i++)
        {
            Mogera* mog = dynamic_cast<Mogera*>(enemies[i]);
            if (mog != nullptr)
            {
                for (int ci = 0; ci < mog->getNumberOfChilds(); ci++)
                {
                    if (!mog->isChildActive(ci)) continue;
                    sf::FloatRect childBounds(mog->getChildX(ci), mog->getChildY(ci), 20.f, 20.f);
                    if (snowballBounds.intersects(childBounds))
                    {
                        // Kill child immediately and deactivate snowball
                        mog->killChild(ci);
                        snowball->deactivate();
                        return true;
                    }
                }
            }
        }
        return false;
    }


    bool isPlayerHit(float playerX, float playerY)
    {
        sf::FloatRect playerBounds(playerX, playerY, 40.f, 40.f);

        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr || enemies[i]->getIsDead())
                continue;

            Tornado* tornado = dynamic_cast<Tornado*>(enemies[i]);
            if (tornado != nullptr && tornado->isKnifeHittingPlayer(playerX, playerY))
                return true;

            if (enemies[i]->getIsEncased())
                continue;

            sf::FloatRect enemyBounds(
                enemies[i]->getPositionX(),
                enemies[i]->getPositionY(),
                40.f, 40.f
            );

            if (playerBounds.intersects(enemyBounds))
                return true;

            // Also check Mogera children collisions
            Mogera* mog = dynamic_cast<Mogera*>(enemies[i]);
            if (mog != nullptr)
            {
                for (int ci = 0; ci < mog->getNumberOfChilds(); ci++)
                {
                    if (!mog->isChildActive(ci)) continue;
                    sf::FloatRect childBounds(mog->getChildX(ci), mog->getChildY(ci), 20.f, 20.f);
                    if (playerBounds.intersects(childBounds))
                        return true;
                }
            }
        }
        return false;
    }



// Level complete
    bool isLevelComplete()
    {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i] == nullptr) continue;
            if (!enemies[i]->getIsDead()) return false;
        }
        return true;
    }


    void draw(sf::RenderWindow& window)
    {
        if (backgroundLoaded)
        {
            window.draw(backgroundSprite);
            sf::RectangleShape overlay(sf::Vector2f(600.f, 600.f));
            overlay.setFillColor(sf::Color(0, 0, 0, 70));  // change 100 to taste
            window.draw(overlay);
        }
        for (int i = 0; i < platformCount; i++)
            window.draw(platforms[i]);

        for (int i = 0; i < enemyCount; i++)
        {
            {
                if (enemyTint != sf::Color::White)
                {
                    enemies[i]->setTint(enemyTint);
                }
                enemies[i]->DisplayEnemy(window);

            }
        }

        // Draw active power-ups
        for (int pi = 0; pi < MAX_POWERUPS; pi++)
        {
            if (activePowerUps[pi] != nullptr && activePowerUps[pi]->getActiveStatus())
            {
                activePowerUps[pi]->DrawPowerUp(window);
            }
        }
    }

    void spawnPowerUp(float px, float py)
    {
        if (powerupCount >= MAX_POWERUPS) return;

        PowerUps* p = nullptr;
        int r = rand() % 3;
        if (r == 0) p = new SpeedBoost(px, py);
        else if (r == 1) p = new SnowballPower(px, py);
        else p = new DistanceIncrease(px, py);

        for (int i = 0; i < MAX_POWERUPS; i++)
        {
            if (activePowerUps[i] == nullptr)
            {
                activePowerUps[i] = p;
                powerupCount++;
                break;
            }
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
        for (int i = 0; i < MAX_POWERUPS; i++)
        {
            if (activePowerUps[i] != nullptr)
            {
                delete activePowerUps[i];
                activePowerUps[i] = nullptr;
            }
        }
    }
};