#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class GameEnd
{
private:
    sf::Font font;

public:
    GameEnd()
    {
        // Try to load a font; fall back silently if missing
        font.loadFromFile("assets/Title.ttf");
    }

    void draw(sf::RenderWindow& window, int finalScore, int finalGems)
    {
        window.clear(sf::Color(20, 0, 50));

        sf::Text title;
        title.setFont(font);
        title.setString("CONGRATULATIONS!");
        title.setCharacterSize(48);
        title.setFillColor(sf::Color::Yellow);
        title.setPosition(70.f, 80.f);
        window.draw(title);

        sf::Text message;
        message.setFont(font);
        message.setString("You have completed the game.");
        message.setCharacterSize(20);
        message.setFillColor(sf::Color::White);
        message.setPosition(140.f, 160.f);
        window.draw(message);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Final Score: " + std::to_string(finalScore));
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(180.f, 220.f);
        window.draw(scoreText);

        sf::Text gemsText;
        gemsText.setFont(font);
        gemsText.setString("Total Gems: " + std::to_string(finalGems));
        gemsText.setCharacterSize(20);
        gemsText.setFillColor(sf::Color::Yellow);
        gemsText.setPosition(190.f, 260.f);
        window.draw(gemsText);

        sf::Text madeBy;
        madeBy.setFont(font);
        madeBy.setString("Made by Faiq and Cheema");
        madeBy.setCharacterSize(18);
        madeBy.setFillColor(sf::Color(180, 180, 180));
        madeBy.setPosition(180.f, 360.f);
        window.draw(madeBy);

        sf::Text continueText;
        continueText.setFont(font);
        continueText.setString("Press ENTER to return to Main Menu");
        continueText.setCharacterSize(16);
        continueText.setFillColor(sf::Color(180, 180, 180));
        continueText.setPosition(120.f, 420.f);
        window.draw(continueText);
    }
};
