#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class MainMenu
{
private:
    sf::Font font;
    sf::Font texts;

    // Button hover effect
    bool playHovered;
    bool leaderboardHovered;
    bool exitHovered;

public:
    MainMenu()
    {
        font.loadFromFile("assets/Title.ttf");

        texts.loadFromFile("assets/Text.ttf");

        playHovered = false;
        leaderboardHovered = false;
        exitHovered = false;
    }

    // ==========================================
    // HANDLE EVENTS
    // Returns:
    // 1  = stay on main menu
    // 2  = go to character select (Play clicked)
    // 8  = go to leaderboard
    // -1 = exit game
    // ==========================================

    int handleEvents(sf::Event& event, sf::RenderWindow& window)
    {
        // Mouse movement - hover effect
        if (event.type == sf::Event::MouseMoved)
        {
            float mouseX = event.mouseMove.x;
            float mouseY = event.mouseMove.y;

            // Check play button hover (x:200-400, y:250-300)
            playHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 250 && mouseY <= 300);

            // Check leaderboard hover (x:200-400, y:330-380)
            leaderboardHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 330 && mouseY <= 380);

            // Check exit hover (x:200-400, y:410-460)
            exitHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 410 && mouseY <= 460);
        }

        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            // Play button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 250 && mouseY <= 300)
            {
                return 2;       // Go to character select
            }

            // Leaderboard button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 330 && mouseY <= 380)
            {
                return 8;       // Go to leaderboard
            }

            // Exit button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 410 && mouseY <= 460)
            {
                return -1;      // Exit game
            }
        }

        return 1;       // Stay on main menu
    }

    // ==========================================
    // DRAW
    // ==========================================

    void draw(sf::RenderWindow& window, std::string playerName)
    {
        // ---- BACKGROUND ----
        sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
        background.setFillColor(sf::Color(10, 10, 40));
        window.draw(background);

        // ---- SNOW DOTS ----
        for (int i = 0; i < 30; i++)
        {
            sf::CircleShape snowDot(2.f);
            snowDot.setFillColor(sf::Color(200, 200, 255, 150));
            snowDot.setPosition(i * 20.f, (i * 37) % 600);
            window.draw(snowDot);
        }

        // ---- TITLE - SNOW BROS ----
        sf::Text title;
        title.setFont(font);
        title.setString("SNOW BROS");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color::Cyan);
        title.setPosition(100.f, 60.f);
        window.draw(title);

        // ---- TITLE UNDERLINE ----
        sf::RectangleShape underline(sf::Vector2f(400.f, 3.f));
        underline.setFillColor(sf::Color::Cyan);
        underline.setPosition(100.f, 130.f);
        window.draw(underline);

        // ---- WELCOME TEXT ----
        sf::Text welcome;
        welcome.setFont(texts);
        welcome.setString("Welcome,  " + playerName);
        welcome.setCharacterSize(18);
        welcome.setFillColor(sf::Color(180, 180, 255));
        welcome.setPosition(180.f, 170.f);
        window.draw(welcome);

        // ---- PLAY BUTTON ----
        drawMenuButton(window, "PLAY", 200.f, 250.f, playHovered);

        // ---- LEADERBOARD BUTTON ----
        drawMenuButton(window, "LEADERBOARD", 200.f, 330.f, leaderboardHovered);

        // ---- EXIT BUTTON ----
        drawMenuButton(window, "EXIT", 200.f, 410.f, exitHovered);

        // ---- VERSION TEXT ----
        sf::Text version;
        version.setFont(font);
        version.setString("v1.0  |  CS1004 OOP Project");
        version.setCharacterSize(12);
        version.setFillColor(sf::Color(80, 80, 120));
        version.setPosition(160.f, 560.f);
        window.draw(version);
    }

private:

    void drawMenuButton(sf::RenderWindow& window, std::string text,
        float x, float y, bool hovered)
    {
        // Button background
        sf::RectangleShape button(sf::Vector2f(200.f, 45.f));

        if (hovered)
        {
            button.setFillColor(sf::Color(0, 100, 180));    // Brighter when hovered
            button.setOutlineColor(sf::Color::Cyan);
            button.setOutlineThickness(2.f);
        }
        else
        {
            button.setFillColor(sf::Color(0, 50, 100));     // Normal color
            button.setOutlineColor(sf::Color(0, 100, 180));
            button.setOutlineThickness(1.f);
        }

        button.setPosition(x, y);
        window.draw(button);

        // Button text
        sf::Text buttonText;
        buttonText.setFont(texts);
        buttonText.setString(text);
        buttonText.setCharacterSize(18);
        buttonText.setFillColor(sf::Color::White);

        // Center text in button
        float textWidth = text.length() * 10.f;
        float textX = x + (200.f - textWidth) / 2.f;
        buttonText.setPosition(textX, y + 12.f);
        window.draw(buttonText);
    }
};