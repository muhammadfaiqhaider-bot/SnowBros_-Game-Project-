#include <SFML/Graphics.hpp>
#include "Gamakichi.h"
#include "Mogera.h"
#include "Tornado.h"
#include "FlyingFoogo.h"
#include "Botom.h"

int main()
{
    // Window 600x600
    sf::RenderWindow window(sf::VideoMode(600, 600), "Snow Bros - Enemy Test");
    window.setFramerateLimit(60);

    // ---- PLATFORMS ----
    sf::RectangleShape platforms[6];

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

    // ---- ENEMIES ----
    Botom botom(100.f, 520.f);
    FlyingFoogaFoog foogaFoog(300.f, 240.f);
    Tornado tornado(50.f, 100.f);

    // Bosses spawn center screen separately
    Mogera mogera(390.f, 340.f);
    Gamakichi gamakichi(220.f, 380.f);

    // ---- DUMMY PLAYER ----
    float playerX = 100.f;
    float playerY = 520.f;

    sf::RectangleShape playerShape(sf::Vector2f(40.f, 40.f));
    playerShape.setFillColor(sf::Color::White);
    playerShape.setPosition(playerX, playerY);

    // ---- TOGGLE FLAG ----
    // true  = testing Mogera
    // false = testing Gamakichi
    bool testMogera = true;

    // ---- INSTRUCTIONS PANEL ----
    sf::RectangleShape instructionPanel(sf::Vector2f(600.f, 25.f));
    instructionPanel.setFillColor(sf::Color(0, 0, 0, 180));
    instructionPanel.setPosition(0.f, 0.f);

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

            if (event.type == sf::Event::KeyPressed)
            {
                // Move dummy player with arrow keys
                if (event.key.code == sf::Keyboard::Left)
                {
                    playerX -= 10.f;
                }
                if (event.key.code == sf::Keyboard::Right)
                {
                    playerX += 10.f;
                }
                if (event.key.code == sf::Keyboard::Up)
                {
                    playerY -= 10.f;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    playerY += 10.f;
                }

                // Keep player inside screen
                if (playerX < 0)
                {
                    playerX = 0;
                }
                if (playerX > 560)
                {
                    playerX = 560;
                }
                if (playerY < 0)
                {
                    playerY = 0;
                }
                if (playerY > 560)
                {
                    playerY = 560;
                }

                // TAB - switch between bosses
                if (event.key.code == sf::Keyboard::Tab)
                {
                    testMogera = !testMogera;
                }

                // H - hit Mogera
                if (event.key.code == sf::Keyboard::H)
                {
                    if (testMogera)
                    {
                        mogera.reduceHealth();
                    }
                }

                // G - hit Gamakichi
                if (event.key.code == sf::Keyboard::G)
                {
                    if (!testMogera)
                    {
                        gamakichi.reduceHealth();
                    }
                }

                playerShape.setPosition(playerX, playerY);
            }
        }

        // 2. UPDATE

        // --- Always update standard enemies ---

        // Botom collision check
        botom.setOnGround(false);
        for (int i = 0; i < 6; i++)
        {
            sf::FloatRect botomBounds(botom.getPositionX(), botom.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (botomBounds.intersects(platformBounds))
            {
                botom.setOnGround(true);
            }
        }

        if (botom.getPositionX() <= 0 || botom.getPositionX() >= 560)
        {
            botom.setHitWall(true);
        }

        // FlyingFoogo collision check
        foogaFoog.setOnGround(false);
        for (int i = 0; i < 6; i++)
        {
            sf::FloatRect foogBounds(foogaFoog.getPositionX(), foogaFoog.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (foogBounds.intersects(platformBounds))
            {
                foogaFoog.setOnGround(true);
            }
        }

        // Tornado collision check
        tornado.setOnGround(false);
        for (int i = 0; i < 6; i++)
        {
            sf::FloatRect tornadoBounds(tornado.getPositionX(), tornado.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (tornadoBounds.intersects(platformBounds))
            {
                tornado.setOnGround(true);
            }
        }

        if (tornado.getPositionX() <= 0 || tornado.getPositionX() >= 560)
        {
            tornado.setHitWall(true);
        }

        // Update standard enemies
        botom.movementsUpdate();
        foogaFoog.movementsUpdate();
        tornado.setPlayerPosition(playerX, playerY);
        tornado.movementsUpdate();

        // --- Boss toggle update ---
        if (testMogera)
        {

            // MogeraChild collision check
            for (int i = 0; i < mogera.getNumberOfChilds(); i++)
            {
                if (mogera.getChildX(i) >= 0)
                {
                    mogera.setChildOnGround(i, false); //Added reset here
                    for (int j = 0; j < 6; j++)
                    {
                        sf::FloatRect childBounds(mogera.getChildX(i), mogera.getChildY(i), 20.f, 20.f);
                        sf::FloatRect platformBounds = platforms[j].getGlobalBounds();

                        if (childBounds.intersects(platformBounds))
                        {
                            mogera.setChildOnGround(i, true);
                        }
                    }
                }
            }

            mogera.setPlayerPosition(playerX, playerY);
            mogera.movementsUpdate();
        }
        else
        {
            gamakichi.setPlayerPosition(playerX, playerY);
            gamakichi.movementsUpdate();
        }

        // 3. DRAW
        window.clear(sf::Color(200, 150, 150));

        // Draw platforms
        for (int i = 0; i < 6; i++)
        {
            window.draw(platforms[i]);
        }

        // Draw player
        window.draw(playerShape);

        // Draw standard enemies
        botom.DisplayEnemy(window);
        foogaFoog.DisplayEnemy(window);
        tornado.DisplayEnemy(window);

        // Draw active boss only
        if (testMogera)
        {
            mogera.DisplayEnemy(window);
        }
        else
        {
            gamakichi.DisplayEnemy(window);
        }

        // Draw instruction panel on top
        window.draw(instructionPanel);

        window.display();
    }

    return 0;
}