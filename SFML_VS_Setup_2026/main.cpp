#include <SFML/Graphics.hpp>
#include "LoginScreen.h"
#include "MainMenu.h"
#include "CharacterSelecte.h"
#include "HUD.h"
#include "PauseMenu.h"
#include "Nick.h"
#include "Botom.h"
#include "AuthenticationSystem.h"
#include "DataBase.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Snow Bros");
    window.setFramerateLimit(60);

    // ---- SYSTEMS ----
    DatabaseManager dbManager;
    AuthManager authManager(&dbManager);

    // ---- SCREENS ----
    LoginScreen loginScreen;
    MainMenu mainMenu;
    CharacterSelect charSelect;
    HUD hud;
    PauseMenu pauseMenu;

    // ---- FONT for Game Over and Pause ----
    sf::Font font;
    font.loadFromFile("Orbitron-VariableFont_wght.ttf");

    // ---- PLATFORMS ----
    sf::RectangleShape platforms[6];

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

    // ---- PLAYER ----
    Nick nick(100.f, 520.f);

    // ---- ENEMIES ----
    Botom botom1(300.f, 520.f);
    Botom botom2(450.f, 520.f);
    Botom botom3(200.f, 380.f);

    // ---- CURRENT SCREEN ----
    // 0 = Login
    // 1 = Main Menu
    // 2 = Character Select
    // 3 = Gameplay
    // 4 = Pause
    // 5 = Game Over
    // 9 = Shop (later)
    int currentScreen = 0;

    // ---- SAVE MESSAGE ----
    std::string saveMessage = "";
    int saveMessageTimer = 0;

    // ---- GAME LOOP ----
    while (window.isOpen())
    {
        // 1. HANDLE EVENTS
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
                    currentScreen = 1;
                }
            }

            // ---- MAIN MENU ----
            else if (currentScreen == 1)
            {
                int result = mainMenu.handleEvents(event, window);
                if (result == 2)
                {
                    currentScreen = 2;
                }
                else if (result == -1)
                {
                    window.close();
                }
            }

            // ---- CHARACTER SELECT ----
            else if (currentScreen == 2)
            {
                int result = charSelect.handleEvents(event);
                if (result == 3)
                {
                    currentScreen = 3;
                }
                else if (result == 1)
                {
                    currentScreen = 1;
                }
            }

            // ---- GAMEPLAY ----
            else if (currentScreen == 3)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    // Space - throw snowball
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        nick.throwSnowball();
                    }

                    // ESC - pause
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        currentScreen = 4;
                    }
                }
            }

            // ---- PAUSE MENU ----
            else if (currentScreen == 4)
            {
                int result = pauseMenu.handleEvents(event);

                if (result == 3)
                {
                    currentScreen = 3;      // Resume gameplay
                }
                else if (result == 9)
                {
                    currentScreen = 9;      // Go to shop (later)
                }
                else if (result == 10)
                {
                    // Save game
                    dbManager.saveProgress(
                        authManager.getCurrentUserId(),
                        1,                      // current level
                        nick.getLives(),
                        nick.getGemCount(),
                        nick.getScore()
                    );

                    saveMessage = "Game Saved!";
                    saveMessageTimer = 180;     // Show for 3 seconds
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
                        currentScreen = 1;      // Back to main menu
                    }
                }
            }
        }

        // 2. UPDATE - only during gameplay
        if (currentScreen == 3)
        {
            // --- Player platform collision ---
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

            // --- Botom array ---
            Botom* botoms[3] = { &botom1, &botom2, &botom3 };

            // --- Botom platform collision ---
            for (int b = 0; b < 3; b++)
            {
                botoms[b]->setOnGround(false);

                for (int i = 0; i < 6; i++)
                {
                    sf::FloatRect botomBounds(botoms[b]->getPositionX(), botoms[b]->getPositionY(), 40.f, 40.f);
                    sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

                    if (botomBounds.intersects(platformBounds))
                    {
                        float platformTop = platforms[i].getPosition().y;
                        float botomBottom = botoms[b]->getPositionY() + 40.f;

                        if (botomBottom >= platformTop && botoms[b]->getPositionY() < platformTop && botoms[b]->getVelocityY() >= 0)
                        {
                            botoms[b]->setOnGround(true);
                            botoms[b]->snapToGround(platformTop - 39.99f);
                        }
                    }
                }

                // Wall check
                if (botoms[b]->getPositionX() <= 0 || botoms[b]->getPositionX() >= 560)
                {
                    botoms[b]->setHitWall(true);
                }
            }

            // --- Snowball collision ---
            if (nick.getSnowball() != nullptr)
            {
                sf::FloatRect snowballBounds = nick.getSnowball()->getHitBox();

                for (int b = 0; b < 3; b++)
                {
                    sf::FloatRect botomBounds(botoms[b]->getPositionX(), botoms[b]->getPositionY(), 40.f, 40.f);

                    if (snowballBounds.intersects(botomBounds))
                    {
                        nick.getSnowball()->onHitEnemy(botoms[b]);
                    }
                }
            }

            // --- Player enemy collision ---
            sf::FloatRect nickBounds(nick.getPositionX(), nick.getPositionY(), 40.f, 40.f);

            for (int b = 0; b < 3; b++)
            {
                sf::FloatRect botomBounds(botoms[b]->getPositionX(), botoms[b]->getPositionY(), 40.f, 40.f);

                if (nickBounds.intersects(botomBounds))
                {
                    nick.loseLife();
                }
            }

            // --- Check game over ---
            if (!nick.getIsAlive())
            {
                currentScreen = 5;
            }

            // --- Update snowball ---
            nick.updateSnowball();

            // --- Update HUD ---
            hud.update(nick.getScore(), nick.getLives(), nick.getGemCount(), 1);

            // --- Update save message timer ---
            if (saveMessageTimer > 0)
            {
                saveMessageTimer--;
            }

            // --- Update all entities ---
            nick.movementsUpdate();
            botom1.movementsUpdate();
            botom2.movementsUpdate();
            botom3.movementsUpdate();
        }

        // 3. DRAW
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
            window.clear(sf::Color(200, 150, 150));

            // Draw platforms
            for (int i = 0; i < 6; i++)
            {
                window.draw(platforms[i]);
            }

            // Draw enemies
            botom1.DisplayEnemy(window);
            botom2.DisplayEnemy(window);
            botom3.DisplayEnemy(window);

            // Draw snowball
            nick.drawSnowball(window);

            // Draw player
            nick.displayPlayer(window);

            // Draw HUD on top
            hud.draw(window);

            // Draw save message if active
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

        // ---- PAUSE ----
        else if (currentScreen == 4)
        {
            // Draw gameplay behind pause
            window.clear(sf::Color(200, 150, 150));

            for (int i = 0; i < 6; i++)
            {
                window.draw(platforms[i]);
            }

            botom1.DisplayEnemy(window);
            botom2.DisplayEnemy(window);
            botom3.DisplayEnemy(window);
            nick.displayPlayer(window);
            hud.draw(window);

            // Draw pause menu on top
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
            scoreText.setString("Score: " + std::to_string(nick.getScore()));
            scoreText.setCharacterSize(25);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(210.f, 280.f);
            window.draw(scoreText);

            sf::Text continueText;
            continueText.setFont(font);
            continueText.setString("Press ENTER for Main Menu");
            continueText.setCharacterSize(18);
            continueText.setFillColor(sf::Color(180, 180, 180));
            continueText.setPosition(120.f, 400.f);
            window.draw(continueText);
        }

        // ---- SHOP PLACEHOLDER ----
        else if (currentScreen == 9)
        {
            // Shop coming soon placeholder
            window.clear(sf::Color(10, 10, 40));

            sf::Text shopText;
            shopText.setFont(font);
            shopText.setString("SHOP - Coming Soon!");
            shopText.setCharacterSize(30);
            shopText.setFillColor(sf::Color::Yellow);
            shopText.setPosition(130.f, 250.f);
            window.draw(shopText);

            sf::Text backText;
            backText.setFont(font);
            backText.setString("Press ESC to go back");
            backText.setCharacterSize(18);
            backText.setFillColor(sf::Color::White);
            backText.setPosition(175.f, 330.f);
            window.draw(backText);

            // ESC from shop goes back to pause
            sf::Event shopEvent;
            while (window.pollEvent(shopEvent))
            {
                if (shopEvent.type == sf::Event::KeyPressed)
                {
                    if (shopEvent.key.code == sf::Keyboard::Escape)
                    {
                        currentScreen = 4;
                    }
                }
            }
        }

        window.display();
    }

    return 0;
}