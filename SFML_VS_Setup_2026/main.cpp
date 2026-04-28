#include <SFML/Graphics.hpp>
#include "Nick.h"
#include "TestLevel.h"
#include "HUD.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Snow Bros - Physics Test");
    window.setFramerateLimit(60);

    // ---- Player ----
    Nick nick(100.f, 520.f);

    // ---- Test Level ----
    TestLevel testLevel;

    // ---- HUD ----
    HUD hud;

    // ---- Game Loop ----
    while (window.isOpen())
    {
        // 1. EVENTS
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    nick.throwSnowball();
                }
            }
        }

        // 2. UPDATE
        testLevel.update(nick);
        nick.updateSnowball();
        nick.movementsUpdate();

        hud.update(
            nick.getScore(),
            nick.getLives(),
            nick.getGemCount(),
            1
        );

        // 3. DRAW
        window.clear(sf::Color(200, 150, 150));

        testLevel.draw(window);

        nick.drawSnowball(window);
        nick.displayPlayer(window);

        hud.draw(window);

        window.display();
    }

    return 0;
}