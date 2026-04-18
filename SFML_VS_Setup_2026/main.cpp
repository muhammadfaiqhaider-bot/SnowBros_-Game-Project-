#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Circle Test");

    // Create a circle
    sf::CircleShape circle(80.f);   // radius = 80
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(360.f, 260.f); // center-ish

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }

    return 0;
}
