#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class PauseMenu
{
private:
    sf::Font font;

    // Hover effects
    bool resumeHovered;
    bool shopHovered;
    bool saveHovered;
    bool exitHovered;

public:
    PauseMenu()
    {
        font.loadFromFile("assets/Text.ttf");

        resumeHovered = false;
        shopHovered = false;
        saveHovered = false;
        exitHovered = false;
    }

    // ==========================================
    // HANDLE EVENTS
    // Returns:
    // 3  = resume gameplay
    // 4  = stay on pause
    // 9  = go to shop
    // 10 = save game
    // 1  = exit to main menu
    // ==========================================

    int handleEvents(sf::Event& event)
    {
        // Mouse movement - hover effect
        if (event.type == sf::Event::MouseMoved)
        {
            float mouseX = event.mouseMove.x;
            float mouseY = event.mouseMove.y;

            // Resume hover (x:200-400, y:220-265)
            resumeHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 220 && mouseY <= 265);

            // Shop hover (x:200-400, y:290-335)
            shopHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 290 && mouseY <= 335);

            // Save hover (x:200-400, y:360-405)
            saveHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 360 && mouseY <= 405);

            // Exit hover (x:200-400, y:430-475)
            exitHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 430 && mouseY <= 475);
        }

        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            // Resume button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 220 && mouseY <= 265)
            {
                return 3;       // Resume gameplay
            }

            // Shop button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 290 && mouseY <= 335)
            {
                return 9;       // Go to shop
            }

            // Save button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 360 && mouseY <= 405)
            {
                return 10;      // Save game
            }

            // Exit button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 430 && mouseY <= 475)
            {
                return 1;       // Exit to main menu
            }
        }

        // ESC key  resume
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                return 3;       // Resume gameplay
            }
        }

        return 4;       // Stay on pause
    }

  

    void draw(sf::RenderWindow& window)
    {
        //  DARK OVERLAY 
        sf::RectangleShape overlay(sf::Vector2f(600.f, 600.f));
        overlay.setFillColor(sf::Color(0, 0, 0, 170));
        window.draw(overlay);

        //  PAUSE BOX 
        sf::RectangleShape pauseBox(sf::Vector2f(320.f, 340.f));
        pauseBox.setFillColor(sf::Color(10, 10, 40));
        pauseBox.setOutlineColor(sf::Color::Cyan);
        pauseBox.setOutlineThickness(2.f);
        pauseBox.setPosition(140.f, 130.f);
        window.draw(pauseBox);

        //  TITLE 
        sf::Text pauseTitle;
        pauseTitle.setFont(font);
        pauseTitle.setString("PAUSED");
        pauseTitle.setCharacterSize(35);
        pauseTitle.setFillColor(sf::Color::Cyan);
        pauseTitle.setPosition(240.f, 148.f);
        window.draw(pauseTitle);

        //  UNDERLINE 
        sf::RectangleShape underline(sf::Vector2f(280.f, 2.f));
        underline.setFillColor(sf::Color::Cyan);
        underline.setPosition(160.f, 195.f);
        window.draw(underline);

        //  BUTTONS 
        drawPauseButton(window, "RESUME", 200.f, 220.f, sf::Color(0, 130, 0), resumeHovered);
        drawPauseButton(window, "SHOP", 200.f, 290.f, sf::Color(150, 100, 0), shopHovered);
        drawPauseButton(window, "SAVE GAME", 200.f, 360.f, sf::Color(0, 0, 130), saveHovered);
        drawPauseButton(window, "MAIN MENU", 200.f, 430.f, sf::Color(130, 0, 0), exitHovered);

        //  HINT 
        sf::Text hint;
        hint.setFont(font);
        hint.setString("ESC to resume");
        hint.setCharacterSize(12);
        hint.setFillColor(sf::Color(80, 80, 120));
        hint.setPosition(235.f, 490.f);
        window.draw(hint);
    }

private:

    void drawPauseButton(sf::RenderWindow& window, std::string text,
        float x, float y, sf::Color color, bool hovered)
    {
        // Button background
        sf::RectangleShape button(sf::Vector2f(200.f, 40.f));

        if (hovered)
        {
            // Brighten color on hover
            button.setFillColor(sf::Color(
                color.r + 50 > 255 ? 255 : color.r + 50,
                color.g + 50 > 255 ? 255 : color.g + 50,
                color.b + 50 > 255 ? 255 : color.b + 50
            ));
            button.setOutlineColor(sf::Color::White);
            button.setOutlineThickness(2.f);
        }
        else
        {
            button.setFillColor(color);
            button.setOutlineColor(sf::Color(150, 150, 150));
            button.setOutlineThickness(1.f);
        }

        button.setPosition(x, y);
        window.draw(button);

        // Button text
        sf::Text buttonText;
        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(16);
        buttonText.setFillColor(sf::Color::White);

        // Center text
        float textWidth = text.length() * 9.f;
        float textX = x + (200.f - textWidth) / 2.f;
        buttonText.setPosition(textX, y + 10.f);
        window.draw(buttonText);
    }
};