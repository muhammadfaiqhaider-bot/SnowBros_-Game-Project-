#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class CharacterSelect
{
private:
    sf::Font font;

    int selectedCharacter;      // 0=Nick, 1=Speedy, 2=Ranger  (P1)
    int selectedCharacter2;     // P2 selection (-1 = none)

    bool nickHovered;
    bool char1Hovered;
    bool char2Hovered;
    bool startHovered;

    sf::Texture nickTexture;
    sf::Texture char1Texture;
    sf::Texture char2Texture;

    sf::Sprite nickSprite;
    sf::Sprite char1Sprite;
    sf::Sprite char2Sprite;

    bool nickImgLoaded;
    bool char1ImgLoaded;
    bool char2ImgLoaded;

    bool onePlayerSelected;
    bool twoPlayerHovered;
    bool onePlayerHovered;

public:
    CharacterSelect()
    {
        font.loadFromFile("assets/Text.ttf");

        selectedCharacter = 0;
        selectedCharacter2 = -1;

        nickHovered = false;
        char1Hovered = false;
        char2Hovered = false;
        startHovered = false;

        nickImgLoaded = false;
        char1ImgLoaded = false;
        char2ImgLoaded = false;

        onePlayerSelected = true;
        twoPlayerHovered = false;
        onePlayerHovered = false;

        if (nickTexture.loadFromFile("assets/MainPlayer.png"))
        {
            nickImgLoaded = true;
            nickSprite.setTexture(nickTexture);
            float sx = 120.f / nickTexture.getSize().x;
            float sy = 140.f / nickTexture.getSize().y;
            nickSprite.setScale(sx, sy);
        }

        if (char1Texture.loadFromFile("assets/faiq.png"))
        {
            char1ImgLoaded = true;
            char1Sprite.setTexture(char1Texture);
            float sx = 120.f / char1Texture.getSize().x;
            float sy = 140.f / char1Texture.getSize().y;
            char1Sprite.setScale(sx, sy);
        }

        if (char2Texture.loadFromFile("assets/cheema.png"))
        {
            char2ImgLoaded = true;
            char2Sprite.setTexture(char2Texture);
            float sx = 120.f / char2Texture.getSize().x;
            float sy = 140.f / char2Texture.getSize().y;
            char2Sprite.setScale(sx, sy);
        }
    }

    // ==========================================
    // Returns:
    //   2 = stay on character select
    //   3 = go to gameplay
    //   1 = go back to main menu
    // ==========================================
    int handleEvents(sf::Event& event)
    {
        if (event.type == sf::Event::MouseMoved)
        {
            float mx = event.mouseMove.x;
            float my = event.mouseMove.y;

            nickHovered = (mx >= 55 && mx <= 175 && my >= 175 && my <= 330);
            char1Hovered = (mx >= 240 && mx <= 360 && my >= 175 && my <= 330);
            char2Hovered = (mx >= 425 && mx <= 545 && my >= 175 && my <= 330);
            startHovered = (mx >= 200 && mx <= 400 && my >= 510 && my <= 555);
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mx = event.mouseButton.x;
            float my = event.mouseButton.y;

            // --- Nick box ---
            if (mx >= 55 && mx <= 175 && my >= 175 && my <= 330)
            {
                if (!onePlayerSelected && selectedCharacter != 0)
                    selectedCharacter2 = 0;
                else if (!onePlayerSelected && selectedCharacter2 == 0)
                    selectedCharacter2 = -1;
                else
                    selectedCharacter = 0;
            }

            // --- Speedy box ---
            if (mx >= 240 && mx <= 360 && my >= 175 && my <= 330)
            {
                if (!onePlayerSelected && selectedCharacter != 1)
                    selectedCharacter2 = 1;
                else if (!onePlayerSelected && selectedCharacter2 == 1)
                    selectedCharacter2 = -1;
                else
                    selectedCharacter = 1;
            }

            // --- Ranger box ---
            if (mx >= 425 && mx <= 545 && my >= 175 && my <= 330)
            {
                if (!onePlayerSelected && selectedCharacter != 2)
                    selectedCharacter2 = 2;
                else if (!onePlayerSelected && selectedCharacter2 == 2)
                    selectedCharacter2 = -1;
                else
                    selectedCharacter = 2;
            }

            // --- Mode buttons ---
            onePlayerHovered = (mx >= 130 && mx <= 230 && my >= 460 && my <= 495);
            twoPlayerHovered = (mx >= 250 && mx <= 370 && my >= 460 && my <= 495);

            if (mx >= 130 && mx <= 230 && my >= 460 && my <= 495)
            {
                onePlayerSelected = true;
                selectedCharacter2 = -1;
            }
            if (mx >= 250 && mx <= 370 && my >= 460 && my <= 495)
                onePlayerSelected = false;

            // --- Start button ---
            if (mx >= 200 && mx <= 400 && my >= 510 && my <= 555)
            {
                if (!onePlayerSelected && selectedCharacter2 == -1)
                    return 2;   // block: P2 not chosen yet
                return 3;
            }
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                return 1;
        }

        return 2;
    }

    // ==========================================
    // DRAW
    // ==========================================
    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
        background.setFillColor(sf::Color(128, 0, 128));
        window.draw(background);

        sf::Text title;
        title.setFont(font);
        title.setString("SELECT CHARACTER");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color::Cyan);
        title.setPosition(157.0f, 50.f);
        window.draw(title);

        sf::RectangleShape underline(sf::Vector2f(400.f, 3.f));
        underline.setFillColor(sf::Color::Cyan);
        underline.setPosition(100.f, 95.f);
        window.draw(underline);

        sf::Text instruction;
        instruction.setFont(font);
        instruction.setString("Click a character to select");
        instruction.setCharacterSize(14);
        instruction.setFillColor(sf::Color(150, 150, 200));
        instruction.setPosition(190.f, 115.f);
        window.draw(instruction);

        drawCharacterBox(window, 55.f, 175.f, sf::Color(30, 80, 180),
            "      NICK", "     Balanced",
            nickSprite, nickImgLoaded,
            selectedCharacter == 0, nickHovered, selectedCharacter2 == 0);

        drawCharacterBox(window, 240.f, 175.f, sf::Color(180, 30, 30),
            "FAIQ SIGMA GUY", "   Speedster",
            char1Sprite, char1ImgLoaded,
            selectedCharacter == 1, char1Hovered, selectedCharacter2 == 1);

        drawCharacterBox(window, 425.f, 175.f, sf::Color(180, 150, 0),
            "CHEEMA TUFF GUY", "    Long Range",
            char2Sprite, char2ImgLoaded,
            selectedCharacter == 2, char2Hovered, selectedCharacter2 == 2);

        drawSelectedInfo(window);

        // --- 1P button ---
        sf::RectangleShape onePBtn(sf::Vector2f(100.f, 35.f));
        onePBtn.setPosition(130.f, 460.f);
        onePBtn.setFillColor(onePlayerSelected ? sf::Color(0, 130, 0) : sf::Color(20, 20, 60));
        onePBtn.setOutlineColor(onePlayerSelected ? sf::Color::Green : sf::Color(60, 60, 120));
        onePBtn.setOutlineThickness(onePlayerHovered ? 2.f : 1.f);
        window.draw(onePBtn);

        sf::Text onePText;
        onePText.setFont(font);
        onePText.setString("1 PLAYER");
        onePText.setCharacterSize(13);
        onePText.setFillColor(sf::Color::White);
        onePText.setPosition(138.f, 471.f);
        window.draw(onePText);

        // --- 2P button ---
        sf::RectangleShape twoPBtn(sf::Vector2f(120.f, 35.f));
        twoPBtn.setPosition(250.f, 460.f);
        twoPBtn.setFillColor(!onePlayerSelected ? sf::Color(0, 130, 0) : sf::Color(20, 20, 60));
        twoPBtn.setOutlineColor(!onePlayerSelected ? sf::Color::Green : sf::Color(60, 60, 120));
        twoPBtn.setOutlineThickness(twoPlayerHovered ? 2.f : 1.f);
        window.draw(twoPBtn);

        sf::Text twoPText;
        twoPText.setFont(font);
        twoPText.setString("2 PLAYERS");
        twoPText.setCharacterSize(13);
        twoPText.setFillColor(sf::Color::White);
        twoPText.setPosition(258.f, 471.f);
        window.draw(twoPText);

        // --- Start button ---
        sf::RectangleShape startButton(sf::Vector2f(200.f, 45.f));
        startButton.setFillColor(startHovered ? sf::Color(0, 160, 0) : sf::Color(0, 100, 0));
        startButton.setOutlineColor(startHovered ? sf::Color::Green : sf::Color(0, 160, 0));
        startButton.setOutlineThickness(startHovered ? 2.f : 1.f);
        startButton.setPosition(200.f, 510.f);
        window.draw(startButton);

        sf::Text startText;
        startText.setFont(font);
        startText.setString("START GAME");
        startText.setCharacterSize(16);
        startText.setFillColor(sf::Color::White);
        startText.setPosition(228.f, 523.f);
        window.draw(startText);

        sf::Text backHint;
        backHint.setFont(font);
        backHint.setString("Press ESC to go back");
        backHint.setCharacterSize(12);
        backHint.setFillColor(sf::Color(80, 80, 120));
        backHint.setPosition(205.f, 575.f);
        window.draw(backHint);
    }

    // ==========================================
    // PUBLIC GETTERS
    // ==========================================
    int getSelectedCharacter() const { return selectedCharacter; }
    int getP2Character()       const { return selectedCharacter2; }
    int getPlayerMode()        const { return onePlayerSelected ? 1 : 2; }

private:

    void drawCharacterBox(sf::RenderWindow& window,
        float x, float y,
        sf::Color fallbackColor,
        std::string charName,
        std::string charType,
        sf::Sprite& charSprite,
        bool imgLoaded,
        bool isSelected,
        bool isHovered,
        bool isP2Selected = false)
    {
        const float BOX_W = 120.f;
        const float BOX_H = 140.f;

        sf::RectangleShape box(sf::Vector2f(BOX_W, BOX_H));
        if (isSelected)
        {
            box.setFillColor(sf::Color(30, 30, 80));
            box.setOutlineColor(sf::Color::Yellow);
            box.setOutlineThickness(3.f);
        }
        else if (isP2Selected)
        {
            box.setFillColor(sf::Color(30, 30, 80));
            box.setOutlineColor(sf::Color(255, 100, 0));
            box.setOutlineThickness(3.f);
        }
        else if (isHovered)
        {
            box.setFillColor(sf::Color(25, 25, 65));
            box.setOutlineColor(sf::Color::Cyan);
            box.setOutlineThickness(2.f);
        }
        else
        {
            box.setFillColor(sf::Color(20, 20, 55));
            box.setOutlineColor(sf::Color(60, 60, 120));
            box.setOutlineThickness(1.f);
        }
        box.setPosition(x, y);
        window.draw(box);

        if (imgLoaded)
        {
            charSprite.setPosition(x, y);
            window.draw(charSprite);
        }
        else
        {
            sf::CircleShape body(30.f);
            body.setFillColor(fallbackColor);
            body.setOutlineColor(sf::Color::White);
            body.setOutlineThickness(2.f);
            body.setPosition(x + 30.f, y + 65.f);
            window.draw(body);

            sf::CircleShape head(20.f);
            head.setFillColor(fallbackColor);
            head.setOutlineColor(sf::Color::White);
            head.setOutlineThickness(2.f);
            head.setPosition(x + 40.f, y + 30.f);
            window.draw(head);

            sf::CircleShape eye1(3.f); eye1.setFillColor(sf::Color::White); eye1.setPosition(x + 48.f, y + 38.f); window.draw(eye1);
            sf::CircleShape eye2(3.f); eye2.setFillColor(sf::Color::White); eye2.setPosition(x + 60.f, y + 38.f); window.draw(eye2);
        }

        if (isSelected)
        {
            sf::Text p1Label; p1Label.setFont(font); p1Label.setString("P1");
            p1Label.setCharacterSize(12); p1Label.setFillColor(sf::Color::Yellow);
            p1Label.setPosition(x + 4.f, y + 4.f);
            window.draw(p1Label);
        }
        if (isP2Selected)
        {
            sf::Text p2Label; p2Label.setFont(font); p2Label.setString("P2");
            p2Label.setCharacterSize(12); p2Label.setFillColor(sf::Color(255, 100, 0));
            p2Label.setPosition(x + 4.f, y + 4.f);
            window.draw(p2Label);
        }

        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(charName);
        nameText.setCharacterSize(15);
        nameText.setFillColor(isSelected ? sf::Color::Yellow : sf::Color::White);
        nameText.setPosition(x + 10.f, y + BOX_H + 8.f);
        window.draw(nameText);

        sf::Text typeText;
        typeText.setFont(font);
        typeText.setString(charType);
        typeText.setCharacterSize(12);
        typeText.setFillColor(sf::Color::Cyan);
        typeText.setPosition(x + 10.f, y + BOX_H + 28.f);
        window.draw(typeText);
    }

    void drawSelectedInfo(sf::RenderWindow& window)
    {
        std::string selectedName, selectedDesc;
        if (selectedCharacter == 0) { selectedName = "NICK";   selectedDesc = "By choosing me You will Get nothing"; }
        else if (selectedCharacter == 1) { selectedName = "SPEEDY"; selectedDesc = "By Choosing me you will get 100000+ Aura Points"; }
        else if (selectedCharacter == 2) { selectedName = "RANGER"; selectedDesc = "By Choosing me you will feel Tall and Handsome"; }

        sf::RectangleShape infoBar(sf::Vector2f(540.f, 35.f));
        infoBar.setFillColor(sf::Color(20, 20, 60));
        infoBar.setOutlineColor(sf::Color::Yellow);
        infoBar.setOutlineThickness(1.f);
        infoBar.setPosition(30.f, 400.f);
        window.draw(infoBar);

        sf::Text selectedText;
        selectedText.setFont(font);
        selectedText.setString("P1: " + selectedName + "  |  " + selectedDesc);
        selectedText.setCharacterSize(11);
        selectedText.setFillColor(sf::Color::Yellow);
        selectedText.setPosition(40.f, 411.f);
        window.draw(selectedText);

        if (!onePlayerSelected)
        {
            sf::Text p2Prompt;
            p2Prompt.setFont(font);
            if (selectedCharacter2 == -1)
                p2Prompt.setString("P2: Click a different character to assign Player 2");
            else
            {
                std::string p2Name = (selectedCharacter2 == 0) ? "NICK"
                    : (selectedCharacter2 == 1) ? "FAIQ" : "CHEEMA";
                p2Prompt.setString("P2: " + p2Name + " selected");
            }
            p2Prompt.setCharacterSize(11);
            p2Prompt.setFillColor(sf::Color(255, 100, 0));
            p2Prompt.setPosition(40.f, 450.f);
            window.draw(p2Prompt);
        }
    }
};