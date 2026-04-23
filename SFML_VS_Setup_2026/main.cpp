#include <SFML/Graphics.hpp>
#include "Nick.h"
#include "Botom.h"

int main()
{
    // Window 600x600
    sf::RenderWindow window(sf::VideoMode(600, 600), "Snow Bros - Snowball Test");
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

    // ---- PLAYER ----
    Nick nick(100.f, 520.f);

    // ---- ENEMIES ----
    Botom botom1(300.f, 520.f);
    Botom botom2(450.f, 520.f);
    Botom botom3(200.f, 380.f);

    // ---- LIVES DISPLAY ----
    sf::RectangleShape life1(sf::Vector2f(20.f, 20.f));
    life1.setFillColor(sf::Color::Blue);
    life1.setPosition(10.f, 5.f);

    sf::RectangleShape life2(sf::Vector2f(20.f, 20.f));
    life2.setFillColor(sf::Color::Blue);
    life2.setPosition(35.f, 5.f);

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
                // Space - throw snowball
                if (event.key.code == sf::Keyboard::Space)
                {
                    nick.throwSnowball();
                }

                // L - test lose life
                if (event.key.code == sf::Keyboard::L)
                {
                    nick.loseLife();
                }
            }
        }

        // 2. UPDATE

        // --- Player platform collision FIRST ---
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
                    nick.setJump(0); // added this so that jump resets when landing - cheema
                    nick.snapToGround(platformTop - 40.f);

                }
            }
        }

        // --- Botom1 platform collision ---
        botom1.setOnGround(false);
        for (int i = 0; i < 6; i++)
        {
            sf::FloatRect botomBounds(botom1.getPositionX(), botom1.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (botomBounds.intersects(platformBounds))
            {
                float platformTop = platforms[i].getPosition().y; //Had to change this whole condiiton -Cheema
                float botomBottom = botom1.getPositionY() + 40.f;

                if (botomBottom >= platformTop && botom1.getPositionY() < platformTop && botom1.getVelocityY() >= 0)
                {
                    botom1.setOnGround(true);
                    botom1.snapToGround(platformTop - 39.99);   //Same logic as nick but idk why i had to set ts to 39.9 otherwise it glitched -Cheema

                }   //wese faiqay, imo we should put this kind of code in class and call it here , i think it would be more efficient. -Cheema
            }
        }


        if (botom1.getPositionX() <= 0 || botom1.getPositionX() >= 560)
        {
            botom1.setHitWall(true); //I have changed setHitWall function in botom.h -Cheema
            

        }


        // --- Botom2 platform collision ---
        botom2.setOnGround(false);
        for (int i = 0; i < 6; i++)
        {
            sf::FloatRect botomBounds(botom2.getPositionX(), botom2.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (botomBounds.intersects(platformBounds))
            {
                float platformTop = platforms[i].getPosition().y;
                float botomBottom = botom2.getPositionY() + 40.f;

                if (botomBottom >= platformTop && botom2.getPositionY() < platformTop && botom2.getVelocityY() >= 0)
                {
                    botom2.setOnGround(true);
                    botom2.snapToGround(platformTop - 39.99);

                }
            }
        }
        if (botom2.getPositionX() <= 0 || botom2.getPositionX() >= 560)
        {
            botom2.setHitWall(true);


        }



        // --- Botom3 platform collision ---
        botom3.setOnGround(false);
        for (int i = 0; i < 6; i++)
        {
            sf::FloatRect botomBounds(botom3.getPositionX(), botom3.getPositionY(), 40.f, 40.f);
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (botomBounds.intersects(platformBounds))
            {
                float platformTop = platforms[i].getPosition().y;
                float botomBottom = botom3.getPositionY() + 40.f;

                if (botomBottom >= platformTop && botom3.getPositionY() < platformTop && botom3.getVelocityY() >= 0)
                {
                    botom3.setOnGround(true);
                    botom3.snapToGround(platformTop - 39.99);

                }
            }
        }

        if (botom3.getPositionX() <= 0 || botom3.getPositionX() >= 560)
        {
            botom3.setHitWall(true);
 

        }


        // --- Snowball collision with all 3 botoms ---
        if (nick.getSnowball() != nullptr)
        {
            sf::FloatRect snowballBounds = nick.getSnowball()->getHitBox();

            // Check botom1
            sf::FloatRect botom1Bounds(botom1.getPositionX(), botom1.getPositionY(), 40.f, 40.f);
            if (snowballBounds.intersects(botom1Bounds))
            {
                nick.getSnowball()->onHitEnemy(&botom1);
            }

            // Check botom2
            sf::FloatRect botom2Bounds(botom2.getPositionX(), botom2.getPositionY(), 40.f, 40.f);
            if (snowballBounds.intersects(botom2Bounds))
            {
                nick.getSnowball()->onHitEnemy(&botom2);
            }

            // Check botom3
            sf::FloatRect botom3Bounds(botom3.getPositionX(), botom3.getPositionY(), 40.f, 40.f);
            if (snowballBounds.intersects(botom3Bounds))
            {
                nick.getSnowball()->onHitEnemy(&botom3);
            }
        }

        // --- Player touches Botom - lose life ---
        sf::FloatRect nickBounds(nick.getPositionX(), nick.getPositionY(), 40.f, 40.f);

        sf::FloatRect botom1Bounds(botom1.getPositionX(), botom1.getPositionY(), 40.f, 40.f);
        sf::FloatRect botom2Bounds(botom2.getPositionX(), botom2.getPositionY(), 40.f, 40.f);
        sf::FloatRect botom3Bounds(botom3.getPositionX(), botom3.getPositionY(), 40.f, 40.f);

        if (nickBounds.intersects(botom1Bounds))
        {
            nick.loseLife();
        }
        if (nickBounds.intersects(botom2Bounds))
        {
            nick.loseLife();
        }
        if (nickBounds.intersects(botom3Bounds))
        {
            nick.loseLife();
        }

        // --- Update snowball ---
        nick.updateSnowball();

        // --- Update all ---
        nick.movementsUpdate();
        botom1.movementsUpdate();
        botom2.movementsUpdate();
        botom3.movementsUpdate();

        // --- Update lives display ---
        if (nick.getLives() >= 1)
        {
            life1.setFillColor(sf::Color::Blue);
        }
        else
        {
            life1.setFillColor(sf::Color(50, 50, 50));
        }

        if (nick.getLives() >= 2)
        {
            life2.setFillColor(sf::Color::Blue);
        }
        else
        {
            life2.setFillColor(sf::Color(50, 50, 50));
        }

        // 3. DRAW
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

        // Draw lives
        window.draw(life1);
        window.draw(life2);

        window.display();
    }

    return 0;
}