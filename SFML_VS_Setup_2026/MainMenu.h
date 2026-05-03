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

    static const int FRAMES_PER_CHUNK = 25;  // 25 frames each chunk = 4 chunks for 100 frames
    static const int CHUNK_COUNT = 4;
    sf::Texture bgChunks[CHUNK_COUNT];
    sf::Sprite bgAnimSprite;
    bool bgAnimLoaded;
    int bgCurrentFrame;
    int bgAnimTimer;
    static const int BG_ANIM_SPEED = 3;
    static const int BG_TOTAL_FRAMES = 100;
    static const int BG_FRAME_W = 640;
    static const int BG_FRAME_H = 480;

public:
    MainMenu()
    {
        font.loadFromFile("assets/Title.ttf");

        texts.loadFromFile("assets/Text.ttf");

        playHovered = false;
        leaderboardHovered = false;
        exitHovered = false;



        bgCurrentFrame = 0;
        bgAnimTimer = 0;
        bgAnimLoaded = false;

        sf::Image fullImage;
        if (fullImage.loadFromFile("assets/login_bg.png"))
        {
            bgAnimLoaded = true;

            for (int c = 0; c < CHUNK_COUNT; c++)
            {

                sf::IntRect chunkRect(
                    c * FRAMES_PER_CHUNK * BG_FRAME_W,  // x start
                    0,                                    // y start
                    FRAMES_PER_CHUNK * BG_FRAME_W,       // width = 25 * 640 = 16000px
                    BG_FRAME_H                            // height
                );
                bgChunks[c].loadFromImage(fullImage, chunkRect);
            }



            bgAnimSprite.setTexture(bgChunks[0]);
            bgAnimSprite.setTextureRect(sf::IntRect(0, 0, BG_FRAME_W, BG_FRAME_H));
            bgAnimSprite.setScale(600.f / BG_FRAME_W, 600.f / BG_FRAME_H);
        }

    }



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

    void update()
    {
        if (!bgAnimLoaded) return;

        bgAnimTimer++;
        if (bgAnimTimer >= BG_ANIM_SPEED)
        {
            bgAnimTimer = 0;
            bgCurrentFrame = (bgCurrentFrame + 1) % BG_TOTAL_FRAMES;

            int chunk = bgCurrentFrame / FRAMES_PER_CHUNK;
            int frameInChunk = bgCurrentFrame % FRAMES_PER_CHUNK;

            bgAnimSprite.setTexture(bgChunks[chunk]);
            bgAnimSprite.setTextureRect(sf::IntRect(
                frameInChunk * BG_FRAME_W, 0, BG_FRAME_W, BG_FRAME_H
            ));
        }
    }
    void draw(sf::RenderWindow& window, std::string playerName)
    {
        if (bgAnimLoaded)
            window.draw(bgAnimSprite);
        else
        {
            // Fallback - dark blue if no image
            sf::RectangleShape bg(sf::Vector2f(600.f, 600.f));
            bg.setFillColor(sf::Color(5, 10, 30));
            window.draw(bg);
        }
        

        //  TITLE SNOW BROS 
        sf::Text title;
        title.setFont(font);
        title.setString("SNOW BROS");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color(218, 152, 227));
        title.setPosition(135.f, 60.f);
        window.draw(title);

        //  TITLE UNDERLINE 
        sf::RectangleShape underline(sf::Vector2f(400.f, 3.f));
        underline.setFillColor(sf::Color(188, 109, 199));
        underline.setPosition(110.f, 130.f);
        window.draw(underline);

        //  WELCOME TEXT 
        sf::Text welcome;
        welcome.setFont(texts);
        welcome.setString("WELCOME,  " + playerName);
        welcome.setCharacterSize(18);
        welcome.setFillColor(sf::Color(180, 180, 255));
        welcome.setPosition(230.0f, 170.0f);
        window.draw(welcome);

        //  PLAY BUTTON 
        drawMenuButton(window, "PLAY", 200.f, 250.f, playHovered);

        //  LEADERboard BUTTON 
        drawMenuButton(window, "LEADERBOARD", 200.f, 330.f, leaderboardHovered);

        //  EXIT BUTN 
        drawMenuButton(window, "EXIT", 200.f, 410.f, exitHovered);

        //  verision text
        sf::Text version;
        version.setFont(font);
        version.setString("Made by Cheema The Tuff Guy & Faiq The Danger Man");
        version.setCharacterSize(12);
        version.setFillColor(sf::Color(80, 80, 120));
        version.setPosition(120.f, 560.f);
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
            button.setFillColor(sf::Color(46, 17, 51));    // Brighter when hovered
            button.setOutlineColor(sf::Color(218, 152, 227));
            button.setOutlineThickness(2.f);
        }
        else
        {
            button.setFillColor(sf::Color(41, 14, 46));     // Normal color
            button.setOutlineColor(sf::Color(102, 54, 112));
            button.setOutlineThickness(1.f);
        }

        button.setPosition(x, y);
        window.draw(button);
 
        // Button text
        sf::Text buttonText;
        buttonText.setFont(texts);
        buttonText.setString(text);
        buttonText.setCharacterSize(18);
        buttonText.setFillColor(sf::Color(218, 152, 227));

        // Center text in button
        float textWidth = text.length() * 10.f;
        float textX = x + (200.f - textWidth) / 2.f;
        buttonText.setPosition(textX, y + 12.f);
        window.draw(buttonText);
    }
};