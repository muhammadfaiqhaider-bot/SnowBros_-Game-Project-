#include <SFML/Graphics.hpp>
#include "LoginScreen.h"
#include "MainMenu.h"
#include "CharacterSelecte.h"
#include "PauseMenu.h"
#include "GamePlay.h"
#include "AuthenticationSystem.h"
#include "DataBase.h"

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
    CharacterSelect charSelect;
    PauseMenu pauseMenu;
    GamePlay gameplay;

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

    // ---- GAME LOOP ----
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
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        currentScreen = 4;  // Back to pause
                    }
                }
            }
        }

        // ==========================================
        // 2. UPDATE - Only during gameplay
        // ==========================================

        if (currentScreen == 3)
        {
            int result = gameplay.update();

            if (result == 5)
            {
                currentScreen = 5;          // Game over
            }
            else if (result == 6)
            {
                if (gameplay.isGameComplete())
                {
                    currentScreen = 7;      // Game complete!
                }
                else
                {
                    currentScreen = 6;      // Level complete
                }
            }
        }

        // ==========================================
        // 3. DRAW
        // ==========================================

        window.clear();

        // ---- LOGIN ----
        if (currentScreen == 0)
        {
            loginScreen.draw(window);
        }

        // ---- MAIN MENU ----
        else if (currentScreen == 1)
        {
            mainMenu.draw(window, authManager.getCurrentUsername());
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
            sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
            background.setFillColor(sf::Color(0, 50, 0));
            window.draw(background);

            sf::Text levelCompleteText;
            levelCompleteText.setFont(font);
            levelCompleteText.setString("LEVEL COMPLETE!");
            levelCompleteText.setCharacterSize(40);
            levelCompleteText.setFillColor(sf::Color::Green);
            levelCompleteText.setPosition(90.f, 150.f);
            window.draw(levelCompleteText);

            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setString("Score: " + std::to_string(gameplay.getScore()));
            scoreText.setCharacterSize(22);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(210.f, 250.f);
            window.draw(scoreText);

            sf::Text gemsText;
            gemsText.setFont(font);
            gemsText.setString("Gems: " + std::to_string(gameplay.getGems()));
            gemsText.setCharacterSize(22);
            gemsText.setFillColor(sf::Color::Yellow);
            gemsText.setPosition(225.f, 300.f);
            window.draw(gemsText);

            sf::Text nextLevelText;
            nextLevelText.setFont(font);
            nextLevelText.setString("Next Level: " + std::to_string(gameplay.getLevel() + 1));
            nextLevelText.setCharacterSize(22);
            nextLevelText.setFillColor(sf::Color::Cyan);
            nextLevelText.setPosition(185.f, 350.f);
            window.draw(nextLevelText);

            sf::Text pressEnterText;
            pressEnterText.setFont(font);
            pressEnterText.setString("Press ENTER to continue");
            pressEnterText.setCharacterSize(18);
            pressEnterText.setFillColor(sf::Color(180, 180, 180));
            pressEnterText.setPosition(145.f, 430.f);
            window.draw(pressEnterText);
        }

        // ---- GAME COMPLETE ----
        else if (currentScreen == 7)
        {
            sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
            background.setFillColor(sf::Color(20, 0, 50));
            window.draw(background);

            sf::Text completeText;
            completeText.setFont(font);
            completeText.setString("YOU WIN!");
            completeText.setCharacterSize(60);
            completeText.setFillColor(sf::Color::Yellow);
            completeText.setPosition(150.f, 100.f);
            window.draw(completeText);

            sf::Text congratsText;
            congratsText.setFont(font);
            congratsText.setString("Congratulations!");
            congratsText.setCharacterSize(25);
            congratsText.setFillColor(sf::Color::Cyan);
            congratsText.setPosition(165.f, 200.f);
            window.draw(congratsText);

            sf::Text finalScoreText;
            finalScoreText.setFont(font);
            finalScoreText.setString("Final Score: " + std::to_string(gameplay.getScore()));
            finalScoreText.setCharacterSize(22);
            finalScoreText.setFillColor(sf::Color::White);
            finalScoreText.setPosition(180.f, 280.f);
            window.draw(finalScoreText);

            sf::Text finalGemsText;
            finalGemsText.setFont(font);
            finalGemsText.setString("Total Gems: " + std::to_string(gameplay.getGems()));
            finalGemsText.setCharacterSize(22);
            finalGemsText.setFillColor(sf::Color::Yellow);
            finalGemsText.setPosition(190.f, 330.f);
            window.draw(finalGemsText);

            sf::Text returnText;
            returnText.setFont(font);
            returnText.setString("Press ENTER for Main Menu");
            returnText.setCharacterSize(18);
            returnText.setFillColor(sf::Color(180, 180, 180));
            returnText.setPosition(120.f, 430.f);
            window.draw(returnText);
        }

        // ---- SHOP PLACEHOLDER ----
        else if (currentScreen == 9)
        {
            window.clear(sf::Color(10, 10, 40));

            sf::Text shopText;
            shopText.setFont(font);
            shopText.setString("SHOP");
            shopText.setCharacterSize(50);
            shopText.setFillColor(sf::Color::Yellow);
            shopText.setPosition(220.f, 150.f);
            window.draw(shopText);

            sf::Text comingSoonText;
            comingSoonText.setFont(font);
            comingSoonText.setString("Coming Soon!");
            comingSoonText.setCharacterSize(25);
            comingSoonText.setFillColor(sf::Color::White);
            comingSoonText.setPosition(185.f, 250.f);
            window.draw(comingSoonText);

            sf::Text backText;
            backText.setFont(font);
            backText.setString("Press ESC to go back");
            backText.setCharacterSize(18);
            backText.setFillColor(sf::Color(180, 180, 180));
            backText.setPosition(175.f, 380.f);
            window.draw(backText);
        }

        window.display();
    }

    return 0;
}