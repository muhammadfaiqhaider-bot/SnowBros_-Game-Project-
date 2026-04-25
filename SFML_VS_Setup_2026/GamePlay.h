#pragma once
#include <SFML/Graphics.hpp>
#include "Nick.h"
#include "HUD.h"
#include "LevelManager.h"

class GamePlay
{
private:
    // ---- Platforms ----
    sf::RectangleShape platforms[6];

    // ---- Player ----
    Nick nick;

    // ---- Level Manager ----
    LevelManager levelManager;

    // ---- HUD ----
    HUD hud;

    // ---- Save message ----
    std::string saveMessage;
    int saveMessageTimer;

    // ---- Font ----
    sf::Font font;

public:
    GamePlay() : nick(100.f, 520.f)
    {
        font.loadFromFile("Orbitron-VariableFont_wght.ttf");

        saveMessage = "";
        saveMessageTimer = 0;

        setupPlatforms();

        // Load first level
        levelManager.loadLevel(0);
    }

    // ==========================================
    // HANDLE EVENTS
    // ==========================================

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
                return 4;       // Pause
            }
        }

        return 3;
    }

    // ==========================================
    // UPDATE
    // ==========================================

    int update()
    {
        // Player platform collision
        handlePlayerPlatformCollision();

        // Update all enemies via level manager
        levelManager.updateEnemies(
            nick.getPositionX(),
            nick.getPositionY(),
            platforms,
            6
        );

        // Snowball collision
        handleSnowballCollision();

        // Player enemy collision
        handlePlayerEnemyCollision();

        // Check game over
        if (!nick.getIsAlive())
        {
            return 5;
        }

        // Check level complete
        if (levelManager.isLevelComplete())
        {
            return 6;       // Level complete screen
        }

        // Update snowball
        nick.updateSnowball();

        // Update HUD
        hud.update(
            nick.getScore(),
            nick.getLives(),
            nick.getGemCount(),
            levelManager.getCurrentLevelNumber()
        );

        // Update save message timer
        if (saveMessageTimer > 0)
        {
            saveMessageTimer--;
        }

        // Update player
        nick.movementsUpdate();

        return 3;
    }

    // ==========================================
    // DRAW
    // ==========================================

    void draw(sf::RenderWindow& window)
    {
        window.clear(sf::Color(200, 150, 150));

        // Draw platforms
        for (int i = 0; i < 6; i++)
        {
            window.draw(platforms[i]);
        }

        // Draw all enemies via level manager
        levelManager.draw(window);

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

        levelManager.draw(window);
        nick.displayPlayer(window);
        hud.draw(window);
    }

    // ==========================================
    // LEVEL NAVIGATION
    // ==========================================

    void goNextLevel()
    {
        levelManager.goNextLevel();

        // Reset player position
        nick.snapToGround(520.f);
    }

    bool isGameComplete()
    {
        return levelManager.getCurrentLevelNumber() == 10 &&
            levelManager.isLevelComplete();
    }

    // ==========================================
    // SAVE
    // ==========================================

    void showSaveMessage()
    {
        saveMessage = "Game Saved!";
        saveMessageTimer = 180;
    }

    // Getters
    int getScore() { return nick.getScore(); }
    int getLives() { return nick.getLives(); }
    int getGems() { return nick.getGemCount(); }
    int getLevel() { return levelManager.getCurrentLevelNumber(); }

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

    void handleSnowballCollision()
    {
        // Will be handled through level manager later
        // For now placeholder
    }

    void handlePlayerEnemyCollision()
    {
        // Will be handled through level manager later
        // For now placeholder
    }
};