#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "LoginScreen.h"
#include <iostream>
#include "MainMenu.h"
#include "CharacterSelecte.h"
#include "PauseMenu.h"
#include "GamePlay.h"
#include "GameEnd.h"
#include "AuthenticationSystem.h"
#include "DataBase.h"
#include "LeaderboardScreen.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Snow Bros");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("assets/Title.ttf");

    DatabaseManager dbManager;
    AuthManager authManager(&dbManager);

    LoginScreen loginScreen;
    MainMenu mainMenu;
    LeaderboardScreen leaderboardScreen;
    CharacterSelect charSelect;
    PauseMenu pauseMenu;
    GamePlay gameplay;
    GameEnd gameEnd;

    // Cache the logged-in username here so it's never lost mid-session
    std::string currentUsername = "";

    // 0=Login  1=MainMenu  2=CharSelect  3=Gameplay
    // 4=Pause  5=GameOver  6=LevelComplete  7=GameComplete
    // 8=Leaderboard  9=Shop
    int currentScreen = 0;

    sf::Music menuMusic;
    bool menuMusicLoaded = false;
    const float menuMusicInitialVolume = 20.f;
    int menuFadeFramesRemaining = 0;

    if (menuMusic.openFromFile("assets/menu.ogg"))
    {
        menuMusic.setLoop(true);
        menuMusic.setVolume(menuMusicInitialVolume);
        menuMusic.play();
        menuMusicLoaded = true;
    }

    sf::Music levelCompleteMusic;
    bool levelCompleteLoaded = false;
    if (levelCompleteMusic.openFromFile("assets/level_complete.wav"))
    {
        levelCompleteMusic.setLoop(true);
        levelCompleteMusic.setVolume(100.f);
        levelCompleteLoaded = true;
    }

    sf::Texture levelCompleteBgTexture;
    sf::Sprite levelCompleteBgSprite;
    bool levelCompleteBgLoaded = false;
    if (levelCompleteBgTexture.loadFromFile("assets/level_complete_bg.png"))
    {
        levelCompleteBgLoaded = true;
        levelCompleteBgSprite.setTexture(levelCompleteBgTexture);
        float sx = 600.f / levelCompleteBgTexture.getSize().x;
        float sy = 600.f / levelCompleteBgTexture.getSize().y;
        levelCompleteBgSprite.setScale(sx, sy);
    }

    int lastScreen = -1;
    int lastPlayedLevel = -1;

    while (window.isOpen())
    {
        // ==========================================
        // 1. EVENTS
        // ==========================================
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // ---- LOGIN ----
            if (currentScreen == 0)
            {
                int result = loginScreen.handleEvents(event, authManager);
                if (result == 1)
                {
                    // Cache username immediately on successful login
                    currentUsername = authManager.getCurrentUsername();
                    currentScreen = 1;
                }
            }

            // ---- MAIN MENU ----
            else if (currentScreen == 1)
            {
                int result = mainMenu.handleEvents(event, window);
                if (result == 2)  currentScreen = 2;
                else if (result == 8)  currentScreen = 8;
                else if (result == -1) window.close();
            }

            // ---- CHARACTER SELECT ----
            else if (currentScreen == 2)
            {
                int result = charSelect.handleEvents(event);
                if (result == 3)
                {
                    gameplay.reset();

                    bool is2P = (charSelect.getPlayerMode() == 2);
                    int p1Char = charSelect.getSelectedCharacter();
                    int p2Char = charSelect.getP2Character();

                    if (is2P)
                        gameplay.setTwoPlayerMode(true, p1Char, p2Char);

                    currentScreen = 3;
                }
                else if (result == 1)
                    currentScreen = 1;
            }

            // ---- GAMEPLAY ----
            else if (currentScreen == 3)
            {
                int result = gameplay.handleEvents(event);
                if (result == 4)
                    currentScreen = 4;
            }

            // ---- PAUSE ----
            else if (currentScreen == 4)
            {
                int result = pauseMenu.handleEvents(event);
                if (result == 3)
                    currentScreen = 3;
                else if (result == 9)
                    currentScreen = 9;
                else if (result == 10)
                {
                    dbManager.saveProgress(
                        authManager.getCurrentUserId(),
                        gameplay.getLevel(),
                        gameplay.getLives(),
                        gameplay.getGems(),
                        gameplay.getScore()
                    );
                    gameplay.showSaveMessage();
                    currentScreen = 3;
                }
                else if (result == 1)
                    currentScreen = 1;
            }

            // ---- GAME OVER ----
            else if (currentScreen == 5)
            {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Return)
                    currentScreen = 1;
            }

            // ---- LEVEL COMPLETE ----
            else if (currentScreen == 6)
            {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Return)
                {
                    if (levelCompleteLoaded &&
                        levelCompleteMusic.getStatus() == sf::Music::Playing)
                        levelCompleteMusic.stop();

                    gameplay.goNextLevel();
                    currentScreen = 3;
                }
            }

            // ---- GAME COMPLETE ----
            else if (currentScreen == 7)
            {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Return)
                    currentScreen = 1;
            }

            // ---- SHOP ----
            else if (currentScreen == 9)
            {
                int shopResult = gameplay.handleShopEvents(event);
                if (shopResult == 3)
                    currentScreen = 3;
            }

            // ---- LEADERBOARD ----
            else if (currentScreen == 8)
            {
                int lbResult = leaderboardScreen.handleEvents(event);
                if (lbResult == 1)
                    currentScreen = 1;
            }
        }

        // ==========================================
        // 2. UPDATE
        // ==========================================

        // Menu music fade when entering gameplay
        if (menuMusicLoaded &&
            menuMusic.getStatus() == sf::Music::Playing &&
            currentScreen == 3)
        {
            if (menuFadeFramesRemaining == 0)
                menuFadeFramesRemaining = 30;
        }
        if (menuFadeFramesRemaining > 0 && menuMusicLoaded)
        {
            menuFadeFramesRemaining--;
            float vol = menuMusicInitialVolume * (float)menuFadeFramesRemaining / 30.f;
            menuMusic.setVolume(vol < 0.f ? 0.f : vol);
            if (menuFadeFramesRemaining == 0)
                menuMusic.stop();
        }

        // Per-level music
        if (currentScreen == 3)
        {
            int lvl = gameplay.getLevel();
            if (lastScreen != 3 || lastPlayedLevel != lvl)
            {
                if (menuMusicLoaded && menuMusic.getStatus() == sf::Music::Playing)
                    menuMusic.stop();
                gameplay.playLevelMusic();
                lastPlayedLevel = lvl;
            }
        }
        else if (lastScreen == 3)
        {
            gameplay.stopLevelMusic();
            lastPlayedLevel = -1;
        }

        // ---- GAMEPLAY UPDATE + SCREEN TRANSITIONS ----
        if (currentScreen == 3)
        {
            int result = gameplay.update();

            if (result == 5)
            {
                // Game over — save score using cached username
                if (!currentUsername.empty())
                    leaderboardScreen.saveScore(currentUsername,
                        gameplay.getScore(),
                        gameplay.getLevel());
                currentScreen = 5;
            }
            else if (result == 6)
            {
                // Level complete — also save so progress isn't lost on quit
                if (!currentUsername.empty())
                    leaderboardScreen.saveScore(currentUsername,
                        gameplay.getScore(),
                        gameplay.getLevel());
                currentScreen = 6;
            }
            else if (result == 7)
            {
                // Game complete
                if (!currentUsername.empty())
                    leaderboardScreen.saveScore(currentUsername,
                        gameplay.getScore(),
                        gameplay.getLevel());
                currentScreen = 7;
            }
        }

        lastScreen = currentScreen;

        // ==========================================
        // 3. DRAW
        // ==========================================
        window.clear();

        if (currentScreen == 0)
        {
            loginScreen.update();
            loginScreen.draw(window);
        }
        else if (currentScreen == 1)
        {
            mainMenu.update();
            mainMenu.draw(window, currentUsername);
        }
        else if (currentScreen == 8)
        {
            leaderboardScreen.draw(window);
        }
        else if (currentScreen == 2)
        {
            charSelect.draw(window);
        }
        else if (currentScreen == 3)
        {
            gameplay.draw(window);
        }
        else if (currentScreen == 4)
        {
            gameplay.drawOnly(window);
            pauseMenu.draw(window);
        }
        else if (currentScreen == 5)
        {
            sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
            background.setFillColor(sf::Color(50, 0, 0));
            window.draw(background);

            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER");
            gameOverText.setCharacterSize(55);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(100.f, 150.f);
            window.draw(gameOverText);

            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setString("Score: " + std::to_string(gameplay.getScore()));
            scoreText.setCharacterSize(25);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(210.f, 260.f);
            window.draw(scoreText);

            sf::Text levelText;
            levelText.setFont(font);
            levelText.setString("Level Reached: " + std::to_string(gameplay.getLevel()));
            levelText.setCharacterSize(25);
            levelText.setFillColor(sf::Color::White);
            levelText.setPosition(165.f, 310.f);
            window.draw(levelText);

            sf::Text continueText;
            continueText.setFont(font);
            continueText.setString("Press ENTER for Main Menu");
            continueText.setCharacterSize(18);
            continueText.setFillColor(sf::Color(180, 180, 180));
            continueText.setPosition(120.f, 420.f);
            window.draw(continueText);
        }
        else if (currentScreen == 6)
        {
            if (levelCompleteBgLoaded)
                window.draw(levelCompleteBgSprite);
            else
            {
                sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
                background.setFillColor(sf::Color(0, 50, 0));
                window.draw(background);
            }

            sf::Text levelCompleteText;
            levelCompleteText.setFont(font);
            levelCompleteText.setString("LEVEL COMPLETE!");
            levelCompleteText.setCharacterSize(40);
            levelCompleteText.setFillColor(sf::Color::Green);
            levelCompleteText.setPosition(120.f, 150.f);
            window.draw(levelCompleteText);

            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setString("Score: " + std::to_string(gameplay.getScore()));
            scoreText.setCharacterSize(22);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(230.f, 250.f);
            window.draw(scoreText);

            sf::Text gemsText;
            gemsText.setFont(font);
            gemsText.setString("Gems: " + std::to_string(gameplay.getGems()));
            gemsText.setCharacterSize(22);
            gemsText.setFillColor(sf::Color::Yellow);
            gemsText.setPosition(235.f, 300.f);
            window.draw(gemsText);

            sf::Text nextLevelText;
            nextLevelText.setFont(font);
            nextLevelText.setString("Next Level: " + std::to_string(gameplay.getLevel() + 1));
            nextLevelText.setCharacterSize(22);
            nextLevelText.setFillColor(sf::Color::Cyan);
            nextLevelText.setPosition(197.f, 350.f);
            window.draw(nextLevelText);

            sf::Text pressEnterText;
            pressEnterText.setFont(font);
            pressEnterText.setString("Press ENTER to continue");
            pressEnterText.setCharacterSize(18);
            pressEnterText.setFillColor(sf::Color(180, 180, 180));
            pressEnterText.setPosition(160.f, 430.f);
            window.draw(pressEnterText);

            if (levelCompleteLoaded &&
                levelCompleteMusic.getStatus() != sf::Music::Playing)
                levelCompleteMusic.play();
        }
        else if (currentScreen == 7)
        {
            gameEnd.draw(window, gameplay.getScore(), gameplay.getGems());
        }
        else if (currentScreen == 9)
        {
            gameplay.drawShop(window);
        }

        window.display();
    }

    return 0;
}