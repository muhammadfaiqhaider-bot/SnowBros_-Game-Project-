#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Nick.h"
#include "HUD.h"
#include "levels.h"
#include "Level1.h"
#include "Level2.h"
<<<<<<< HEAD
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"
#include "Level8.h"
#include "Level9.h"
#include "Level10.h"

=======
>>>>>>> ffc21b01c7fd8b9993a5a0810c34c12abab3c75c

class GamePlay
{
private:
    sf::RectangleShape platforms[6];
    Nick nick;
    HUD hud;
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

<<<<<<< HEAD
        setupPlatforms();
        levels[0] = new Level1();
        levels[1] = new Level2();
        levels[2] = new Level3();
        levels[3] = new Level4();
        levels[4] = new Level5();
        levels[5] = new Level6();
        levels[6] = new Level7();
        levels[7] = new Level8();
        levels[8] = new Level9();
        levels[9] = new Level10();
        totalLevels = 10;
=======
        
        levels[0] = new Level1();
        levels[1] = new Level2();
        totalLevels = 2;
>>>>>>> ffc21b01c7fd8b9993a5a0810c34c12abab3c75c
        for (int i = totalLevels; i < 10; i++)
            levels[i] = nullptr;
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

    int update()
    {
        handlePlayerPlatformCollision();

        Level* current = levels[currentLevelNumber - 1];

        if (current != nullptr)
        {
            current->update(nick.getPositionX(), nick.getPositionY());

            if (nick.getSnowball() != nullptr)
            {
                if (current->checkSnowballCollision(nick.getSnowball()))
                {
<<<<<<< HEAD

=======
                    
>>>>>>> ffc21b01c7fd8b9993a5a0810c34c12abab3c75c
                }
            }

            if (current->isPlayerHit(nick.getPositionX(), nick.getPositionY()))
            {
                nick.loseLife();
            }

            if (current->isLevelComplete())
            {
                return 6;
            }
        }

        if (!nick.getIsAlive())
            return 5;

        nick.updateSnowball();
        hud.update(nick.getScore(), nick.getLives(), nick.getGemCount(), currentLevelNumber);

        if (saveMessageTimer > 0)
            saveMessageTimer--;

        nick.movementsUpdate();

        return 3;
    }
    void draw(sf::RenderWindow& window)
    {
        window.clear(sf::Color(200, 150, 150));

        Level* current = levels[currentLevelNumber - 1];
        if (current != nullptr)
            current->draw(window);
<<<<<<< HEAD

        // Debug / fallback: if level has no platforms (blank) draw gameplay's platforms
        if (current == nullptr || current->getPlatformCount() == 0)
        {
            for (int i = 0; i < 6; i++)
                window.draw(platforms[i]);
        }
=======
>>>>>>> ffc21b01c7fd8b9993a5a0810c34c12abab3c75c

        nick.drawSnowball(window);
        nick.displayPlayer(window);
        hud.draw(window);

<<<<<<< HEAD
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

=======
>>>>>>> ffc21b01c7fd8b9993a5a0810c34c12abab3c75c
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
        currentLevelNumber++;
        nick.snapToGround(520.f);   // Reset player position
    }

    bool isGameComplete()
    {
        return currentLevelNumber > 10;
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

    void reset()
    {
        currentLevelNumber = 1;
        nick = Nick(100.f, 520.f);
        saveMessage = "";
        saveMessageTimer = 0;
<<<<<<< HEAD
        // levelTimer = 0;
=======
       // levelTimer = 0;
>>>>>>> ffc21b01c7fd8b9993a5a0810c34c12abab3c75c

        for (int i = 0; i < 10; i++)
        {
            if (levels[i] != nullptr)
            {
                delete levels[i];
                levels[i] = nullptr;
            }
        }

        levels[0] = new Level1();
        levels[1] = new Level2();
<<<<<<< HEAD
        levels[2] = new Level3();
        levels[3] = new Level4();
        levels[4] = new Level5();
        levels[5] = new Level6();
        levels[6] = new Level7();
        levels[7] = new Level8();
        levels[8] = new Level9();
        levels[9] = new Level10();

        totalLevels = 10;
=======
        totalLevels = 2;
>>>>>>> ffc21b01c7fd8b9993a5a0810c34c12abab3c75c
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