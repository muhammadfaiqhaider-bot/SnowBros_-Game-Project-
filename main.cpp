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

    // Resolution is set to 600x600 bcz this is old game soo bursted piexlated asserts
    // Frames are sett to 60 easy for calculation and ideal for this typa game....
    sf::RenderWindow window(sf::VideoMode(600, 600), "Snow Bros");
    window.setFramerateLimit(60);

    // Font We Use in Our Game Downloaded from text downloader
    sf::Font font;
    font.loadFromFile("assets/Title.ttf");

    // These classes are essential for my LOGGIN PHASE 
    // DataBase Manager Handles all the DATA saving and retrinving things.... Creating user 
    // verifying user saving game progress leader board (inshort most of the file handling stuff)
    DatabaseManager dbManager; // Object is created of DataBase class.....


    // Using the data and autorized login during sign up phase........
    AuthManager authManager(&dbManager);



    //SCREENS.....

    //-00
    LoginScreen loginScreen;
    MainMenu mainMenu;
    LeaderboardScreen leaderboardScreen;
    CharacterSelect charSelect;
    PauseMenu pauseMenu;
    GamePlay gameplay;
    GameEnd gameEnd;

    // ---- CURRENT SCREEN ----
    // 0 = Login
    // 1 = Main Menu
    // 2 = Character Select
    // 3 = Gameplay
    // 4 = Pause
    // 5 = Game Over
    // 6 = Level Complete
    // 7 = Game Complete
    // 9 = Shop (later)
    int currentScreen = 0;

    // ---- AUDIO: menu music ----
    sf::Music menuMusic;
    bool menuMusicLoaded = false;
    const float menuMusicInitialVolume = 80.f;
    int menuFadeFramesRemaining = 0; // fade-out duration in frames

    if (menuMusic.openFromFile("assets/menu.ogg"))
    {
        menuMusic.setLoop(true);
        menuMusic.setVolume(menuMusicInitialVolume);
        menuMusic.play();
        menuMusicLoaded = true;
    }
    // ---- AUDIO: level complete music (plays while level-complete screen shown) ----
    sf::Music levelCompleteMusic;
    bool levelCompleteLoaded = false;
    if (levelCompleteMusic.openFromFile("assets/level_complete.ogg"))
    {
        levelCompleteMusic.setLoop(true); // keep playing until player presses ENTER
        levelCompleteMusic.setVolume(90.f);
        levelCompleteLoaded = true;
    }
    // Optional background image for level-complete screen
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

    // ---- GAME LOOP ----
    int lastScreen = -1;
    int lastPlayedLevel = -1;
    while (window.isOpen())
    {
        // ==========================================
        // 1. HANDLE EVENTS
        // ==========================================
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // ---- LOGIN ----
            if (currentScreen == 0)
            {
                int result = loginScreen.handleEvents(event, authManager);
                if (result == 1)
                {
                    currentScreen = 1;      // Go to main menu
                }
            }

            // ---- MAIN MENU ----
            else if (currentScreen == 1)
            {
                int result = mainMenu.handleEvents(event, window);

                if (result == 2)
                {
                    currentScreen = 2;      // Go to character select
                }

                else if (result == 8)
                {
                    currentScreen = 8;      // Go to leaderboard
                }

                else if (result == -1)
                {
                    window.close();         // Exit game
                }
            }

            // ---- CHARACTER SELECT ----
            else if (currentScreen == 2)
            {
                int result = charSelect.handleEvents(event);

                if (result == 3)
                {
                    gameplay.reset();
                    gameplay.reset();
                    currentScreen = 3;      // Go to gameplay
                }
                else if (result == 1)
                {
                    currentScreen = 1;      // Back to main menu
                }
            }

            // ---- GAMEPLAY ----
            else if (currentScreen == 3)
            {
                int result = gameplay.handleEvents(event);

                if (result == 4)
                {
                    currentScreen = 4;      // Go to pause
                }
            }

            // ---- PAUSE ----
            else if (currentScreen == 4)
            {
                int result = pauseMenu.handleEvents(event);

                if (result == 3)
                {
                    currentScreen = 3;      // Resume gameplay
                }
                else if (result == 9)
                {
                    std::cout << "[Main] PauseMenu requested SHOP" << std::endl;
                    currentScreen = 9;      // Go to shop
                }
                else if (result == 10)
                {
                    // Save game progress
                    dbManager.saveProgress(
                        authManager.getCurrentUserId(),
                        gameplay.getLevel(),
                        gameplay.getLives(),
                        gameplay.getGems(),
                        gameplay.getScore()
                    );
                    gameplay.showSaveMessage();
                    currentScreen = 3;      // Resume after save
                }
                else if (result == 1)
                {
                    currentScreen = 1;      // Exit to main menu
                }
            }

            // ---- GAME OVER ----
            else if (currentScreen == 5)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        currentScreen = 1;  // Back to main menu
                    }
                }
            }

            // ---- LEVEL COMPLETE ----
            else if (currentScreen == 6)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        // Stop level-complete music when proceeding
                        if (levelCompleteLoaded && levelCompleteMusic.getStatus() == sf::Music::Playing)
                            levelCompleteMusic.stop();

                        gameplay.goNextLevel();
                        currentScreen = 3;  // Back to gameplay next level
                    }
                }
            }

            // ---- GAME COMPLETE ----
            else if (currentScreen == 7)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        currentScreen = 1;  // Back to main menu
                    }
                }
            }



            // ---- SHOP ----
            else if (currentScreen == 9)
            {
                int shopResult = gameplay.handleShopEvents(event);
                if (shopResult == 3)
                {
                    // Resume gameplay
                    currentScreen = 3;
                }
            }
            // ---- LEADERBOARD ----
            else if (currentScreen == 8)
            {
                int lbResult = leaderboardScreen.handleEvents(event);
                if (lbResult == 1)
                    currentScreen = 1; // Back to main menu
            }
        }

        // ==========================================
        // 2. UPDATE - Only during gameplay
        // ==========================================

        // handle menu music fade when entering gameplay
        if (menuMusicLoaded && menuMusic.getStatus() == sf::Music::Playing)
        {
            if (currentScreen == 3)
            {
                // start fade if not already
                if (menuFadeFramesRemaining == 0)
                    menuFadeFramesRemaining = 30; // half-second at 60 fps
            }
        }

        if (menuFadeFramesRemaining > 0 && menuMusicLoaded)
        {
            menuFadeFramesRemaining--;
            float vol = menuMusicInitialVolume * (float)menuFadeFramesRemaining / 30.f;
            if (vol < 0.f) vol = 0.f;
            menuMusic.setVolume(vol);
            if (menuFadeFramesRemaining == 0)
                menuMusic.stop();
        }

        // Manage per-level music playback via GamePlay/Level helpers
        if (currentScreen == 3)
        {
            int lvl = gameplay.getLevel();
            // if we just entered gameplay or level changed, (re)start music for this level
            if (lastScreen != 3 || lastPlayedLevel != lvl)
            {
                if (menuMusicLoaded && menuMusic.getStatus() == sf::Music::Playing)
                    menuMusic.stop();
                gameplay.playLevelMusic();
                lastPlayedLevel = lvl;
            }
        }
        else
        {
            // if we just left gameplay, stop level music
            if (lastScreen == 3)
            {
                gameplay.stopLevelMusic();
                lastPlayedLevel = -1;
            }
        }

        if (currentScreen == 3)
        {
            int result = gameplay.update();

            if (result == 5)
            {
                // Save score to leaderboard before showing Game Over
                leaderboardScreen.saveScore(authManager.getCurrentUsername(), gameplay.getScore(), gameplay.getLevel());
                currentScreen = 5;          // Game over
            }
            else if (result == 6)
            {
                currentScreen = 6;      // Level complete
            }
            else if (result == 7)
            {
                // Save final score to leaderboard
                leaderboardScreen.saveScore(authManager.getCurrentUsername(), gameplay.getScore(), gameplay.getLevel());
                currentScreen = 7;      // Game complete!
            }
        }

        // ==========================================
        // 3. DRAW
        // ==========================================

        window.clear();

        // ---- LOGIN ----
        if (currentScreen == 0)
        {
            loginScreen.update();
            loginScreen.draw(window);
        }

        // ---- MAIN MENU ----
        else if (currentScreen == 1)
        {
            mainMenu.draw(window, authManager.getCurrentUsername());
        }

        // ---- LEADERBOARD ----
        else if (currentScreen == 8)
        {
            leaderboardScreen.draw(window);
        }

        // ---- CHARACTER SELECT ----
        else if (currentScreen == 2)
        {
            charSelect.draw(window);
        }

        // ---- GAMEPLAY ----
        else if (currentScreen == 3)
        {
            gameplay.draw(window);
        }

        // ---- PAUSE ----
        else if (currentScreen == 4)
        {
            gameplay.drawOnly(window);      // Frozen game behind pause
            pauseMenu.draw(window);
        }

        // ---- GAME OVER ----
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

        // ---- LEVEL COMPLETE ----
        else if (currentScreen == 6)
        {
            if (levelCompleteBgLoaded)
            {
                window.draw(levelCompleteBgSprite);
            }
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

            // Play level-complete music while this screen is active
            if (levelCompleteLoaded && levelCompleteMusic.getStatus() != sf::Music::Playing)
            {
                levelCompleteMusic.play();
            }
        }

        // ---- GAME COMPLETE ----
        else if (currentScreen == 7)
        {
            gameEnd.draw(window, gameplay.getScore(), gameplay.getGems());
        }

        // ---- SHOP ----
        else if (currentScreen == 9)
        {
            gameplay.drawShop(window);
        }

        window.display();
    }

    return 0;
}