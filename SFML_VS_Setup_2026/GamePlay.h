#pragma once
#include <SFML/Graphics.hpp>
#include "Nick.h"
#include "HUD.h"
#include "Level1.h"
#include "Level2.h"

class GamePlay
{
private:
    sf::RectangleShape platforms[6];
    Nick nick;
    HUD hud;
    int currentLevelNumber;
    Level1* level1;
    Level2* level2;


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

        // Start with level 1
        level1 = new Level1();
        level2 = new Level2();

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
        // Player platform collision - physics stay same as before
        handlePlayerPlatformCollision();

        // Update current level enemies
        if (currentLevelNumber == 1 && level1 != nullptr)
        {
            level1->update(
                nick.getPositionX(),
                nick.getPositionY(),
                platforms,
                6
            );

            // Check snowball vs enemies
            if (nick.getSnowball() != nullptr)
            {
                level1->checkSnowballCollision(nick.getSnowball());
            }

            // Check player vs enemies
            if (level1->isPlayerHit(nick.getPositionX(), nick.getPositionY()))
            {

                    nick.loseLife();
      
            }

            // Check level complete
            if (level1->isLevelComplete())
            {
                return 6;       // Level complete
            }
        }

        // Check game over
        if (!nick.getIsAlive())
        {
            return 5;
        }

        // Update snowball
        nick.updateSnowball();

        // Update HUD
        hud.update(
            nick.getScore(),
            nick.getLives(),
            nick.getGemCount(),
            currentLevelNumber
        );

        // Update save message timer
        if (saveMessageTimer > 0)
        {
            saveMessageTimer--;
        }

        // Update player
        nick.movementsUpdate();


        if (currentLevelNumber == 2 && level2 != nullptr)
        {
            level2->update(nick.getPositionX(), nick.getPositionY(), platforms, 6);

            if (nick.getSnowball() != nullptr)
            {
                level2->checkSnowballCollision(nick.getSnowball());
            }

            if (level2->isPlayerHit(nick.getPositionX(), nick.getPositionY()))
            {
                if (nick.getSnowball() == nullptr)  // Only take damage if snowball isn't active
                {
                    nick.loseLife();
                }
            }

            if (level2->isLevelComplete())
            {
                return 6;
            }
        }


        return 3;
    }

    void draw(sf::RenderWindow& window)
    {
        window.clear(sf::Color(200, 150, 150));

        // Draw platforms
        for (int i = 0; i < 6; i++)
        {
            window.draw(platforms[i]);
        }

        // Draw current level enemies
        if (currentLevelNumber == 1 && level1 != nullptr)
        {
            level1->draw(window);
        }
        if (currentLevelNumber == 2 && level2 != nullptr)
        {
            level2->draw(window);
        }
        // Draw snowball
        nick.drawSnowball(window);

        // Draw player
        nick.displayPlayer(window);

        // Draw HUD
        hud.draw(window);

        // Draw save message
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

        for (int i = 0; i < 6; i++)
        {
            window.draw(platforms[i]);
        }

        if (currentLevelNumber == 1 && level1 != nullptr)
        {
            level1->draw(window);
        }

        if (currentLevelNumber == 2 && level2 != nullptr)
        {
            level2->draw(window);
        }

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
        nick = Nick(100.f, 520.f);   // Re-construct nick at start position (resets lives/score/gems)
        saveMessage = "";
        saveMessageTimer = 0;

        // Reset levels too
        delete level1;
        delete level2;
        level1 = new Level1();
        level2 = new Level2();
    }

    ~GamePlay()
    {
        if (level1 != nullptr)
        {
            delete level1;
            level1 = nullptr;
        }
        if (level2 != nullptr)
        {
            delete level2;
            level2 = nullptr;
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
        nick.setOnGround(false);

        for (int i = 0; i < 6; i++)
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