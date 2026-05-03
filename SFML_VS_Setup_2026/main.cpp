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
#include "LevelNavigator.h"

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


    std::string currentUsername = "";
    // Code Runs sb se  pehle start with the Login screen 
    // login screen = 0
    // main menu = 1
    // CharacterSelection screen =2
    // Gameplay=3             main screen for camplay all levels scroing system and HUD linked with it
    // Pause screen=4  U can pause game to go back or buy stuff 
    // game over screen =5
    // Level Complete = 6
    // game complete = 7
    // leaderboard =8
        //shop =9

    int currentScreen = 0;



    // SOund implementation logic here 
    sf::Music menuMusic;
    bool menuMusicLoaded = false;
    const float menuMusicInitialVolume = 20.f;   // Volume indicator setted to low ...
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

    


    // This while loop is so important  all game loop happens in here ..........
    // screen repeat it selp many times and we see them as moving persons........
    while (window.isOpen())
    {
        // 1. EVENTS
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (currentScreen == 0)          // LOging Screen......
            {
                int result = loginScreen.handleEvents(event, authManager);
                if (result == 1)
                {
                    currentUsername = authManager.getCurrentUsername();
                    currentScreen = 1;
                }
            }

            else if (currentScreen == 1)     // Main menu........
            {
                int result = mainMenu.handleEvents(event, window);
                if (result == 2)  currentScreen = 2;
                else if (result == 8)  currentScreen = 8;
                else if (result == -1) window.close();
            }

            else if (currentScreen == 2) // Character Selection screen..........
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

            else if (currentScreen == 3) //Game play screen...........
            {

                LevelNavigator::handleEvent(event, gameplay); // This feature is added as per requirement for navigating through diffentr screens with numpad

                int result = gameplay.handleEvents(event);
                if (result == 4)
                    currentScreen = 4;
            }

 
            else if (currentScreen == 4) // Pause
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


            else if (currentScreen == 5)  // Gameover screen
            {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Return)
                    currentScreen = 1;
            }

  
            else if (currentScreen == 6) //Level Complete
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

            else if (currentScreen == 7)
            {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Return)
                    currentScreen = 1;
            }


            else if (currentScreen == 9) // Shop screen
            {
                int shopResult = gameplay.handleShopEvents(event);
                if (shopResult == 3)
                    currentScreen = 3;
            }


            else if (currentScreen == 8)             // LEADERBOARD 
            {
                int lbResult = leaderboardScreen.handleEvents(event);
                if (lbResult == 1)
                    currentScreen = 1;
            }
        }


        // Menu music fade when entering gameplay
        if (menuMusicLoaded &&  menuMusic.getStatus() == sf::Music::Playing && currentScreen == 3)
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

        // Per level music
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

        //  GAMEPLAY UPDATE + SCREEN TRANSITIONS 
        if (currentScreen == 3)
        {
            int result = gameplay.update();

            if (result == 5)
            {
                // Game over  save score 
                if (!currentUsername.empty())
                    leaderboardScreen.saveScore(currentUsername,
                        gameplay.getScore(),
                        gameplay.getLevel());
                currentScreen = 5;
            }
            else if (result == 6)
            {
                // Level complete  also save so progress isn't lost on quit
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