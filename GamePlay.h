#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Shop.h"
#include <iostream>
#include "Nick.h"
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

public:
    GamePlay() : nick(100.f, 520.f)
    {
        font.loadFromFile("assets/text.ttf");

        saveMessage = "";
        saveMessageTimer = 0;
        currentLevelNumber = 1;

        setupPlatforms();
        totalLevels = 10;
        for (int i = 0; i < 10; i++)
            levels[i] = nullptr;
        // create first level lazily
        levels[0] = new Level1();
    }

    void loadCurrentLevel()
    {
        int idx = currentLevelNumber - 1;
        if (idx < 0 || idx >= 10)
        {
            std::cout << "[GamePlay] loadCurrentLevel: invalid level index " << idx << std::endl;
            return;
        }

        if (levels[idx] != nullptr)
            return;  // already loaded
        std::cout << "[GamePlay] Loading level " << currentLevelNumber << std::endl;
        switch (currentLevelNumber)
        {
        case 1: levels[idx] = new Level1(); break;
        case 2: levels[idx] = new Level2(); break;
        case 3: levels[idx] = new Level3(); break;
        case 4: levels[idx] = new Level4(); break;
        case 5: levels[idx] = new Level5(); break;
        case 6: levels[idx] = new Level6(); break;
        case 7: levels[idx] = new Level7(); break;
        case 8: levels[idx] = new Level8(); break;
        case 9: levels[idx] = new Level9(); break;
        case 10: levels[idx] = new Level10(); break;
        default: break;
        }
    }
    int handleEvents(sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                nick.throwSnowball();
            }

            if (event.key.code == sf::Keyboard::Escape)
            {
                return 4;
            }
        }

        return 3;
    }

    // Forward shop events so purchases apply to the active player
    int handleShopEvents(sf::Event& event)
    {
        int r = shop.handleEvents(event, nick);
        return r;
    }

    void drawShop(sf::RenderWindow& window)
    {
        shop.draw(window);
    }

    int update()
    {
        loadCurrentLevel();
        handlePlayerPlatformCollision();

    

        Level* current = levels[currentLevelNumber - 1];

        if (current != nullptr)
        {
            // Pass player pointer so Level::update can award score and gems
            current->update(nick.getPositionX(), nick.getPositionY(), &nick);

            // Sync shop coins with player's gems every frame
            shop.syncGems(nick.getGemCount());

            // Ensure snowball is moved before running collision checks so hits are detected
            nick.updateSnowball();

            if (nick.getSnowball() != nullptr)
            {
                if (current->checkSnowballCollision(nick.getSnowball()))
                {

                }
            }

            if (current->isPlayerHit(nick.getPositionX(), nick.getPositionY()))
            {
                nick.loseLife();
            }

            if (current->isLevelComplete())
            {
                std::cout << "[GamePlay] Level " << currentLevelNumber << " is complete" << std::endl;
                // If this was the last level, indicate game complete (7)
                if (currentLevelNumber >= totalLevels)
                    return 7;
                return 6;
            }
        }

        if (!nick.getIsAlive())
            return 5;

        // snowball already updated before collision checks
        hud.update(nick.getScore(), nick.getLives(), nick.getGemCount(), currentLevelNumber);

        if (saveMessageTimer > 0)
            saveMessageTimer--;

        nick.movementsUpdate();

        return 3;
    }
    void draw(sf::RenderWindow& window)
    {
        loadCurrentLevel();
        window.clear(sf::Color(200, 150, 150));

        Level* current = levels[currentLevelNumber - 1];
        if (current != nullptr)
            current->draw(window);

        // Debug / fallback: if level has no platforms (blank) draw gameplay's platforms
        if (current == nullptr || current->getPlatformCount() == 0)
        {
            for (int i = 0; i < 6; i++)
                window.draw(platforms[i]);
        }

        nick.drawSnowball(window);
        nick.displayPlayer(window);
        hud.draw(window);

        // Draw current level number and platform count for debugging
        sf::Text lvlText;
        lvlText.setFont(font);
        int platformCount = 0;
        if (current != nullptr) platformCount = current->getPlatformCount();
        lvlText.setString(std::string("Level: ") + std::to_string(currentLevelNumber) + " P: " + std::to_string(platformCount));
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
    }

    void drawOnly(sf::RenderWindow& window)
    {
        window.clear(sf::Color(200, 150, 150));

        Level* current = levels[currentLevelNumber - 1];
        if (current != nullptr)
            current->draw(window);

        nick.displayPlayer(window);
        hud.draw(window);
    }

    void goNextLevel()
    {
        std::cout << "[GamePlay] goNextLevel from " << currentLevelNumber << std::endl;
        int idx = currentLevelNumber - 1;
        if (levels[idx] != nullptr)
        {
            delete levels[idx];
            levels[idx] = nullptr;
        }

        currentLevelNumber++;
        nick.snapToGround(520.f);   // Reset player position
    }

    bool isGameComplete()
    {
        // Consider game complete when we've progressed past the last level
        // or when the current (last) level is already marked complete.
        if (currentLevelNumber > totalLevels)
            return true;

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

    // Play/stop per-level music (wrappers that call into current Level)
    void playLevelMusic()
    {
        int idx = currentLevelNumber - 1;
        if (idx < 0 || idx >= 10) return;
        if (levels[idx] == nullptr)
            loadCurrentLevel();
        if (levels[idx] != nullptr)
            levels[idx]->playLevelMusic();
    }

    void stopLevelMusic()
    {
        int idx = currentLevelNumber - 1;
        if (idx < 0 || idx >= 10) return;
        if (levels[idx] != nullptr)
            levels[idx]->stopLevelMusic();
    }

    void reset()
    {
        currentLevelNumber = 1;
        nick = Nick(100.f, 520.f);
        saveMessage = "";
        saveMessageTimer = 0;
        // levelTimer = 0;

        for (int i = 0; i < 10; i++)
        {
            if (levels[i] != nullptr)
            {
                delete levels[i];
                levels[i] = nullptr;
            }
        }
        // only recreate first level; others will be created lazily
        levels[0] = new Level1();
        totalLevels = 10;
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
    }

private:


    void setupPlatforms()
    {
        // Platform 1 - Ground
        platforms[0].setSize(sf::Vector2f(600.f, 20.f));
        platforms[0].setFillColor(sf::Color(128, 0, 128));
        platforms[0].setPosition(0.f, 560.f);

        // Platform 2 - Middle Left
        platforms[1].setSize(sf::Vector2f(250.f, 20.f));
        platforms[1].setFillColor(sf::Color(128, 0, 128));
        platforms[1].setPosition(0.f, 420.f);

        // Platform 3 - Middle Right
        platforms[2].setSize(sf::Vector2f(250.f, 20.f));
        platforms[2].setFillColor(sf::Color(128, 0, 128));
        platforms[2].setPosition(350.f, 420.f);

        // Platform 4 - Upper Middle
        platforms[3].setSize(sf::Vector2f(300.f, 20.f));
        platforms[3].setFillColor(sf::Color(128, 0, 128));
        platforms[3].setPosition(150.f, 280.f);

        // Platform 5 - Top Left
        platforms[4].setSize(sf::Vector2f(200.f, 20.f));
        platforms[4].setFillColor(sf::Color(128, 0, 128));
        platforms[4].setPosition(0.f, 140.f);

        // Platform 6 - Top Right
        platforms[5].setSize(sf::Vector2f(200.f, 20.f));
        platforms[5].setFillColor(sf::Color(128, 0, 128));
        platforms[5].setPosition(400.f, 140.f);
    }


    void handlePlayerPlatformCollision()
    {
        if (levels[currentLevelNumber - 1] == nullptr) return;

        sf::RectangleShape* platforms = levels[currentLevelNumber - 1]->getPlatforms();
        int count = levels[currentLevelNumber - 1]->getPlatformCount();

        nick.setOnGround(false);

        for (int i = 0; i < count; i++)
        {
            sf::FloatRect nickBounds(nick.getPositionX(), nick.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (nickBounds.intersects(platformBounds))
            {
                float platformTop = platforms[i].getPosition().y;

                if (nick.getPositionY() + 40.f <= platformTop + 10.f)
                {
                    nick.setOnGround(true);
                    nick.setJump(0);
                    nick.snapToGround(platformTop - 40.f);
                }
            }
        }
    }

};