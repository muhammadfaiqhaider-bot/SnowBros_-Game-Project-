#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Shop.h"
#include "Nick.h"
#include <iostream>
#include "CheemaTheToughGuy.h"
#include "FaiqTheDangerMan.h"
#include "HUD.h"
#include "levels.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"
#include "Level8.h"
#include "Level9.h"
#include "Level10.h"


class GamePlay
{
private:
    sf::RectangleShape platforms[6];
    Nick nick;
    HUD hud;
    Shop shop;
    int currentLevelNumber;
    Level* levels[10];
    int totalLevels;

    std::string saveMessage;
    int saveMessageTimer;

    sf::Font font;

    // ---- TWO PLAYER ----
    Player* player2;        // nullptr in 1P mode
    bool twoPlayerMode;
    bool p2Alive;           // tracks if P2 is still in the game

    // Mirrors one shop slot's effect onto any player without touching gems
    void applyShopItemToPlayer(int index, Player& p)
    {
        switch (index)
        {
        case 0: p.applySpeedBoost();       break;
        case 1: p.applySnowballPower();    break;
        case 2: p.applyDistanceIncrease(); break;
        case 3: p.applyBalloonMode();      break;
        case 4: p.addLife();               break;
        }
    }

public:
    GamePlay() : nick(100.f, 520.f), player2(nullptr), twoPlayerMode(false), p2Alive(false)
    {
        font.loadFromFile("assets/text.ttf");

        saveMessage = "";
        saveMessageTimer = 0;
        currentLevelNumber = 1;

        setupPlatforms();
        totalLevels = 10;
        for (int i = 0; i < 10; i++)
            levels[i] = nullptr;
        levels[0] = new Level1();
    }

    ~GamePlay()
    {
        for (int i = 0; i < 10; i++)
        {
            if (levels[i] != nullptr)
            {
                delete levels[i];
                levels[i] = nullptr;
            }
        }
        if (player2 != nullptr)
        {
            delete player2;
            player2 = nullptr;
        }
    }

    // ==========================================
    // Call this from main.cpp right after gameplay.reset()
    // when transitioning from CharacterSelect -> Gameplay
    // ==========================================
    void setTwoPlayerMode(bool enabled, int p1Char, int p2Char)
    {
        twoPlayerMode = enabled;
        p2Alive = enabled;  // P2 starts alive only in 2P mode

        // Always clean up any old P2
        if (player2 != nullptr)
        {
            delete player2;
            player2 = nullptr;
        }

        if (!enabled) return;

        // Spawn P2 offset from P1 so they don't overlap
        switch (p2Char)
        {
        case 0: player2 = new Nick(400.f, 520.f);   break;
        case 1: player2 = new Faiq(400.f, 520.f);   break;
        case 2: player2 = new Cheema(400.f, 520.f); break;
        default: player2 = new Nick(400.f, 520.f);  break;
        }

        if (player2 != nullptr)
            player2->setAsPlayer2();    // switches controls to arrow keys
    }

    int getPlayerMode() const { return twoPlayerMode ? 2 : 1; }

    void loadCurrentLevel()
    {
        int idx = currentLevelNumber - 1;
        if (idx < 0 || idx >= 10) return;
        if (levels[idx] != nullptr) return;

        switch (currentLevelNumber)
        {
        case 1:  levels[idx] = new Level1();  break;
        case 2:  levels[idx] = new Level2();  break;
        case 3:  levels[idx] = new Level3();  break;
        case 4:  levels[idx] = new Level4();  break;
        case 5:  levels[idx] = new Level5();  break;
        case 6:  levels[idx] = new Level6();  break;
        case 7:  levels[idx] = new Level7();  break;
        case 8:  levels[idx] = new Level8();  break;
        case 9:  levels[idx] = new Level9();  break;
        case 10: levels[idx] = new Level10(); break;
        default: break;
        }
    }

    int handleEvents(sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
                nick.throwSnowball();

            // P2 throw — only if P2 is still alive
            if (twoPlayerMode && player2 != nullptr && p2Alive)
            {
                if (event.key.code == sf::Keyboard::Numpad0)
                    player2->throwSnowball();
            }

            if (event.key.code == sf::Keyboard::Escape)
                return 4;
        }
        return 3;
    }

    int handleShopEvents(sf::Event& event)
    {
        // Let the shop run normally against P1 (deducts gems, applies effect)
        int result = shop.handleEvents(event, nick);

        // If a purchase just happened, mirror the effect onto P2
        if (twoPlayerMode && player2 != nullptr && p2Alive)
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                float mx = event.mouseButton.x;
                float my = event.mouseButton.y;

                // Same hit-test as Shop::handleEvents item buttons
                // Items are at y = 160 + i*70, height 55, x 150-450
                for (int i = 0; i < 5; i++)
                {
                    float btnY = 160.f + i * 70.f;
                    if (mx >= 150 && mx <= 450 && my >= btnY && my <= btnY + 55.f)
                    {
                        // Apply the matching effect to P2 — no gem deduction
                        applyShopItemToPlayer(i, *player2);
                        break;
                    }
                }
            }
        }

        return result;
    }


    void drawShop(sf::RenderWindow& window)
    {
        shop.draw(window);
    }

    // ==========================================
    // UPDATE
    // ==========================================
    int update()
    {
        loadCurrentLevel();
        Level* current = levels[currentLevelNumber - 1];

        // ---- P1 update ----
        nick.movementsUpdate();
        handlePlayerPlatformCollision(&nick);

        if (current != nullptr)
        {
            current->update(nick.getPositionX(), nick.getPositionY(), &nick);
            shop.syncGems(nick.getGemCount());
            nick.updateSnowball();

            if (nick.getSnowball() != nullptr)
                current->checkSnowballCollision(nick.getSnowball());

            if (current->isPlayerHit(nick.getPositionX(), nick.getPositionY()))
                nick.loseLife();
        }

        // ---- P2 update (only when alive) ----
        if (twoPlayerMode && player2 != nullptr && p2Alive)
        {
            player2->movementsUpdate();
            handlePlayerPlatformCollision(player2);

            if (current != nullptr)
            {
                // P2 shares the same level; pass P2 position for enemy/powerup logic
                // Note: calling update again would double-update enemies, so we only
                // check hits and snowball collision for P2 separately.
                player2->updateSnowball();

                if (player2->getSnowball() != nullptr)
                    current->checkSnowballCollision(player2->getSnowball());

                if (current->isPlayerHit(player2->getPositionX(), player2->getPositionY()))
                    player2->loseLife();
            }

            // Check if P2 just died this frame
            if (!player2->getIsAlive())
                p2Alive = false;
        }

        // ---- Level complete ----
        if (current != nullptr && current->isLevelComplete())
        {
            if (currentLevelNumber >= totalLevels)
                return 7;
            return 6;
        }

        // ---- Game over: P1 dead AND (P2 dead or not in use) ----
        bool p1Dead = !nick.getIsAlive();
        bool p2Dead = !twoPlayerMode || !p2Alive;

        if (p1Dead && p2Dead)
            return 5;

        // HUD: show whoever is still alive (P1 stats; P2 shown separately in draw)
        hud.update(nick.getScore(), nick.getLives(), nick.getGemCount(), currentLevelNumber);

        if (saveMessageTimer > 0)
            saveMessageTimer--;

        return 3;
    }

    // ==========================================
    // DRAW
    // ==========================================
    void draw(sf::RenderWindow& window)
    {
        loadCurrentLevel();
        window.clear(sf::Color(200, 150, 150));

        Level* current = levels[currentLevelNumber - 1];
        if (current != nullptr)
            current->draw(window);

        if (current == nullptr || current->getPlatformCount() == 0)
        {
            for (int i = 0; i < 6; i++)
                window.draw(platforms[i]);
        }

        // Draw P1 snowball and player
        nick.drawSnowball(window);
        if (nick.getIsAlive())
            nick.displayPlayer(window);

        // Draw P2 snowball and player (only while alive)
        if (twoPlayerMode && player2 != nullptr && p2Alive)
        {
            player2->drawSnowball(window);
            player2->displayPlayer(window);
        }

        // Main HUD (P1)
        hud.draw(window);

        // P2 HUD shown at the right side of the HUD bar when in 2P mode
        if (twoPlayerMode && player2 != nullptr)
            drawP2HUD(window);

        // Debug level info
        sf::Text lvlText;
        lvlText.setFont(font);
        int platformCount = 0;
        if (current != nullptr) platformCount = current->getPlatformCount();
        lvlText.setString(std::string("Level: ") + std::to_string(currentLevelNumber) +
            " P: " + std::to_string(platformCount));
        lvlText.setCharacterSize(14);
        lvlText.setFillColor(sf::Color::Yellow);
        lvlText.setPosition(10.f, 560.f);
        window.draw(lvlText);

        if (saveMessageTimer > 0)
        {
            sf::Text saveText;
            saveText.setFont(font);
            saveText.setString(saveMessage);
            saveText.setCharacterSize(16);
            saveText.setFillColor(sf::Color::Green);
            saveText.setPosition(220.f, 560.f);
            window.draw(saveText);
        }

        if (nick.getPowerupMessageTimer() > 0 && !nick.getPowerupMessage().empty())
        {
            sf::Text pText;
            pText.setFont(font);
            pText.setString(nick.getPowerupMessage());
            pText.setCharacterSize(16);
            pText.setFillColor(sf::Color::Yellow);
            pText.setPosition(200.f, 540.f);
            window.draw(pText);
        }
    }

    void drawOnly(sf::RenderWindow& window)
    {
        window.clear(sf::Color(200, 150, 150));
        Level* current = levels[currentLevelNumber - 1];
        if (current != nullptr)
            current->draw(window);

        nick.displayPlayer(window);

        if (twoPlayerMode && player2 != nullptr && p2Alive)
            player2->displayPlayer(window);

        hud.draw(window);
        if (twoPlayerMode && player2 != nullptr)
            drawP2HUD(window);
    }

    void goNextLevel()
    {
        int idx = currentLevelNumber - 1;
        if (levels[idx] != nullptr)
        {
            delete levels[idx];
            levels[idx] = nullptr;
        }
        currentLevelNumber++;
        nick.snapToGround(520.f);
        if (twoPlayerMode && player2 != nullptr && p2Alive)
            player2->snapToGround(520.f);
    }

    bool isGameComplete()
    {
        if (currentLevelNumber > totalLevels) return true;
        if (currentLevelNumber == totalLevels)
        {
            Level* current = levels[currentLevelNumber - 1];
            if (current != nullptr && current->isLevelComplete())
                return true;
        }
        return false;
    }

    void showSaveMessage()
    {
        saveMessage = "Game Saved!";
        saveMessageTimer = 180;
    }

    // Getters
    int getScore() { return nick.getScore(); }
    int getLives() { return nick.getLives(); }
    int getGems() { return nick.getGemCount(); }
    int getLevel() { return currentLevelNumber; }

    void playLevelMusic()
    {
        int idx = currentLevelNumber - 1;
        if (idx < 0 || idx >= 10) return;
        if (levels[idx] == nullptr) loadCurrentLevel();
        if (levels[idx] != nullptr) levels[idx]->playLevelMusic();
    }

    void stopLevelMusic()
    {
        int idx = currentLevelNumber - 1;
        if (idx < 0 || idx >= 10) return;
        if (levels[idx] != nullptr) levels[idx]->stopLevelMusic();
    }

    void reset()
    {
        currentLevelNumber = 1;
        nick.reset(100.f, 520.f);
        saveMessage = "";
        saveMessageTimer = 0;

        // Clean up P2 — will be re-created by setTwoPlayerMode if needed
        if (player2 != nullptr)
        {
            delete player2;
            player2 = nullptr;
        }
        twoPlayerMode = false;
        p2Alive = false;

        for (int i = 0; i < 10; i++)
        {
            if (levels[i] != nullptr)
            {
                delete levels[i];
                levels[i] = nullptr;
            }
        }
        levels[0] = new Level1();
        totalLevels = 10;
    }

private:

    // ==========================================
    // P2 mini-HUD — lives shown on the right of the HUD bar
    // ==========================================
    void drawP2HUD(sf::RenderWindow& window)
    {
        sf::Font& f = font;

        // "P2" label
        sf::Text p2Label;
        p2Label.setFont(f);
        p2Label.setString("P2:");
        p2Label.setCharacterSize(12);
        p2Label.setFillColor(p2Alive ? sf::Color(255, 140, 0) : sf::Color(100, 100, 100));
        p2Label.setPosition(360.f, 45.f);
        window.draw(p2Label);

        int p2Lives = (player2 != nullptr) ? player2->getLives() : 0;

        for (int i = 0; i < 5; i++)
        {
            sf::CircleShape heart(8.f);
            if (i < p2Lives && p2Alive)
                heart.setFillColor(sf::Color(255, 100, 0));   // orange hearts for P2
            else
                heart.setFillColor(sf::Color(60, 60, 60));
            heart.setPosition(390.f + i * 22.f, 43.f);
            window.draw(heart);
        }

        // "DEAD" label if P2 is gone
        if (!p2Alive)
        {
            sf::Text deadText;
            deadText.setFont(f);
            deadText.setString("DEAD");
            deadText.setCharacterSize(11);
            deadText.setFillColor(sf::Color(180, 60, 60));
            deadText.setPosition(390.f, 45.f);
            window.draw(deadText);
        }
    }

    // ==========================================
    // Platform collision — works for any Player*
    // ==========================================
    void handlePlayerPlatformCollision(Player* p)
    {
        if (levels[currentLevelNumber - 1] == nullptr) return;
        sf::RectangleShape* plats = levels[currentLevelNumber - 1]->getPlatforms();
        int count = levels[currentLevelNumber - 1]->getPlatformCount();

        p->setOnGround(false);

        for (int i = 0; i < count; i++)
        {
            sf::FloatRect playerBounds(p->getPositionX(), p->getPositionY(), 40.f, 50.f);
            sf::FloatRect platBounds = plats[i].getGlobalBounds();

            if (playerBounds.intersects(platBounds))
            {
                float platTop = plats[i].getPosition().y;
                float playerBot = p->getPositionY() + 50.f;

                if (playerBot >= platTop && p->getPositionY() < platTop && p->getVelocityY() >= 0.f)
                {
                    p->setOnGround(true);
                    p->setJump(0);
                    p->snapToGround(platTop - 49.9f);
                }
            }
        }
    }

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
};