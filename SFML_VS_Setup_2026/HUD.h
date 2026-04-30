#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class HUD
{
private:
    sf::Font font;

    // Game data to display
    int score;
    int lives;
    int gems;
    int level;

public:
    HUD()
    {

        font.loadFromFile("assets/Text.ttf");

        score = 0;
        lives = 2;
        gems = 0;
        level = 1;
    }

    // ==========================================
    // UPDATE DATA - call every frame
    // ==========================================

    void update(int currentScore, int currentLives, int currentGems, int currentLevel)
    {
        score = currentScore;
        lives = currentLives;
        gems = currentGems;
        level = currentLevel;
    }

    // ==========================================
    // DRAW - call after drawing gameplay
    // ==========================================

    void draw(sf::RenderWindow& window)
    {
        // ---- HUD BACKGROUND BAR ----
        sf::RectangleShape hudBar(sf::Vector2f(600.f, 40.f));
        hudBar.setFillColor(sf::Color(0, 0, 0, 180));   // Semi transparent black
        hudBar.setPosition(0.f, 0.f);
        window.draw(hudBar);

        // ---- SCORE - TOP LEFT ----
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(14);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);

        // ---- LIVES - TOP LEFT (below score) ----
        drawLives(window);

        // ---- LEVEL - TOP CENTER ----
        sf::Text levelText;
        levelText.setFont(font);
        levelText.setString("Level  " + std::to_string(level) + " / 10");
        levelText.setCharacterSize(14);
        levelText.setFillColor(sf::Color::Cyan);

        // Center it
        levelText.setPosition(230.f, 12.f);
        window.draw(levelText);

        // ---- GEMS - TOP RIGHT ----
        drawGems(window);
    }

private:

    void drawLives(sf::RenderWindow& window)
    {
        // Lives label
        sf::Text livesLabel;
        livesLabel.setFont(font);
        livesLabel.setString("Lives:");
        livesLabel.setCharacterSize(12);
        livesLabel.setFillColor(sf::Color(200, 200, 200));
        livesLabel.setPosition(10.f, 45.f);
        window.draw(livesLabel);

        // Draw heart icons for each life
        for (int i = 0; i < lives; i++)
        {
            sf::CircleShape heart(8.f);
            heart.setFillColor(sf::Color::Red);
            heart.setPosition(55.f + i * 22.f, 43.f);
            window.draw(heart);
        }

        // Grey hearts for lost lives (max 5 shown)
        for (int i = lives; i < 5; i++)
        {
            sf::CircleShape heart(8.f);
            heart.setFillColor(sf::Color(60, 60, 60));
            heart.setPosition(55.f + i * 22.f, 43.f);
            window.draw(heart);
        }
    }

    void drawGems(sf::RenderWindow& window)
    {
        // Gem icon (small diamond shape)
        sf::RectangleShape gemIcon(sf::Vector2f(12.f, 12.f));
        gemIcon.setFillColor(sf::Color::Cyan);
        gemIcon.setRotation(45.f);
        gemIcon.setPosition(510.f, 13.f);
        window.draw(gemIcon);

        // Gem count
        sf::Text gemText;
        gemText.setFont(font);
        gemText.setString(std::to_string(gems));
        gemText.setCharacterSize(14);
        gemText.setFillColor(sf::Color::Cyan);
        gemText.setPosition(530.f, 10.f);
        window.draw(gemText);
    }
};