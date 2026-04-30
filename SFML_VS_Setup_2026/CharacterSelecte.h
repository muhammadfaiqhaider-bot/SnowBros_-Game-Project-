#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class CharacterSelect
{
private:
    sf::Font font;

    // Which character is selected
    int selectedCharacter;      // 0=Nick, 1=Character1, 2=Character2

    // Hover effects
    bool nickHovered;
    bool char1Hovered;
    bool char2Hovered;
    bool startHovered;

public:
    CharacterSelect()
    {
        font.loadFromFile("assets/Text.ttf");

        selectedCharacter = 0;      // Nick selected by default

        nickHovered = false;
        char1Hovered = false;
        char2Hovered = false;
        startHovered = false;
    }

    // ==========================================
    // HANDLE EVENTS
    // Returns:
    // 2  = stay on character select
    // 3  = go to gameplay
    // 1  = go back to main menu
    // ==========================================

    int handleEvents(sf::Event& event)
    {
        // Mouse movement - hover effect
        if (event.type == sf::Event::MouseMoved)
        {
            float mouseX = event.mouseMove.x;
            float mouseY = event.mouseMove.y;

            // Nick box hover (x:30-190, y:180-420)
            nickHovered = (mouseX >= 30 && mouseX <= 190 &&
                mouseY >= 180 && mouseY <= 420);

            // Character1 box hover (x:220-380, y:180-420)
            char1Hovered = (mouseX >= 220 && mouseX <= 380 &&
                mouseY >= 180 && mouseY <= 420);

            // Character2 box hover (x:410-570, y:180-420)
            char2Hovered = (mouseX >= 410 && mouseX <= 570 &&
                mouseY >= 180 && mouseY <= 420);

            // Start button hover (x:200-400, y:460-505)
            startHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 460 && mouseY <= 505);
        }

        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            // Nick box clicked
            if (mouseX >= 30 && mouseX <= 190 &&
                mouseY >= 180 && mouseY <= 420)
            {
                selectedCharacter = 0;
            }

            // Character1 box clicked
            if (mouseX >= 220 && mouseX <= 380 &&
                mouseY >= 180 && mouseY <= 420)
            {
                selectedCharacter = 1;
            }

            // Character2 box clicked
            if (mouseX >= 410 && mouseX <= 570 &&
                mouseY >= 180 && mouseY <= 420)
            {
                selectedCharacter = 2;
            }

            // Start Game button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 460 && mouseY <= 505)
            {
                return 3;       // Go to gameplay
            }
        }

        // Escape - back to main menu
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                return 1;       // Back to main menu
            }
        }

        return 2;       // Stay on character select
    }

    // ==========================================
    // DRAW
    // ==========================================

    void draw(sf::RenderWindow& window)
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

        // ---- TITLE ----
        sf::Text title;
        title.setFont(font);
        title.setString("SELECT CHARACTER");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color::Cyan);
        title.setPosition(100.f, 50.f);
        window.draw(title);

        // ---- UNDERLINE ----
        sf::RectangleShape underline(sf::Vector2f(400.f, 3.f));
        underline.setFillColor(sf::Color::Cyan);
        underline.setPosition(100.f, 95.f);
        window.draw(underline);

        // ---- INSTRUCTION ----
        sf::Text instruction;
        instruction.setFont(font);
        instruction.setString("Click a character to select");
        instruction.setCharacterSize(14);
        instruction.setFillColor(sf::Color(150, 150, 200));
        instruction.setPosition(175.f, 115.f);
        window.draw(instruction);

        // ---- CHARACTER BOXES ----
        drawCharacterBox(window,
            30.f, 180.f,                        // position
            sf::Color(30, 80, 180),             // body color (blue - Nick)
            "NICK",                             // name
            "Speed:  Normal",                   // stat 1
            "Range:  Normal",                   // stat 2
            "Balanced",                         // type
            selectedCharacter == 0,             // is selected?
            nickHovered                         // is hovered?
        );

        drawCharacterBox(window,
            220.f, 180.f,
            sf::Color(180, 30, 30),             // body color (red - Character1)
            "SPEEDY",
            "Speed:  Fast",
            "Range:  Normal",
            "Speedster",
            selectedCharacter == 1,
            char1Hovered
        );

        drawCharacterBox(window,
            410.f, 180.f,
            sf::Color(180, 150, 0),             // body color (gold - Character2)
            "RANGER",
            "Speed:  Normal",
            "Range:  Long",
            "Long Range",
            selectedCharacter == 2,
            char2Hovered
        );

        // ---- SELECTED CHARACTER INFO ----
        drawSelectedInfo(window);

        // ---- START BUTTON ----
        sf::RectangleShape startButton(sf::Vector2f(200.f, 45.f));

        if (startHovered)
        {
            startButton.setFillColor(sf::Color(0, 160, 0));
            startButton.setOutlineColor(sf::Color::Green);
            startButton.setOutlineThickness(2.f);
        }
        else
        {
            startButton.setFillColor(sf::Color(0, 100, 0));
            startButton.setOutlineColor(sf::Color(0, 160, 0));
            startButton.setOutlineThickness(1.f);
        }

        startButton.setPosition(200.f, 460.f);
        window.draw(startButton);

        sf::Text startText;
        startText.setFont(font);
        startText.setString("START GAME");
        startText.setCharacterSize(16);
        startText.setFillColor(sf::Color::White);
        startText.setPosition(228.f, 473.f);
        window.draw(startText);

        // ---- BACK HINT ----
        sf::Text backHint;
        backHint.setFont(font);
        backHint.setString("Press ESC to go back");
        backHint.setCharacterSize(12);
        backHint.setFillColor(sf::Color(80, 80, 120));
        backHint.setPosition(205.f, 555.f);
        window.draw(backHint);
    }

    // Getter
    int getSelectedCharacter()
    {
        return selectedCharacter;
    }

private:

    // ==========================================
    // HELPER - Draw one character box
    // ==========================================

    void drawCharacterBox(sf::RenderWindow& window,
        float x, float y,
        sf::Color bodyColor,
        std::string charName,
        std::string stat1,
        std::string stat2,
        std::string charType,
        bool isSelected,
        bool isHovered)
    {
        // ---- BOX BACKGROUND ----
        sf::RectangleShape box(sf::Vector2f(160.f, 240.f));

        if (isSelected)
        {
            box.setFillColor(sf::Color(30, 30, 80));
            box.setOutlineColor(sf::Color::Yellow);     // Yellow outline when selected
            box.setOutlineThickness(3.f);
        }
        else if (isHovered)
        {
            box.setFillColor(sf::Color(25, 25, 65));
            box.setOutlineColor(sf::Color::Cyan);       // Cyan outline when hovered
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

        // ---- CHARACTER BODY (snowman shape) ----
        // Body - big circle
        sf::CircleShape body(35.f);
        body.setFillColor(bodyColor);
        body.setOutlineColor(sf::Color::White);
        body.setOutlineThickness(2.f);
        body.setPosition(x + 45.f, y + 70.f);
        window.draw(body);

        // Head - small circle
        sf::CircleShape head(22.f);
        head.setFillColor(bodyColor);
        head.setOutlineColor(sf::Color::White);
        head.setOutlineThickness(2.f);
        head.setPosition(x + 58.f, y + 32.f);
        window.draw(head);

        // Eyes - two small white dots
        sf::CircleShape eye1(4.f);
        eye1.setFillColor(sf::Color::White);
        eye1.setPosition(x + 68.f, y + 40.f);
        window.draw(eye1);

        sf::CircleShape eye2(4.f);
        eye2.setFillColor(sf::Color::White);
        eye2.setPosition(x + 82.f, y + 40.f);
        window.draw(eye2);

        // ---- SELECTED CHECKMARK ----
        if (isSelected)
        {
            sf::RectangleShape checkmark(sf::Vector2f(20.f, 20.f));
            checkmark.setFillColor(sf::Color::Yellow);
            checkmark.setPosition(x + 130.f, y + 5.f);
            window.draw(checkmark);

            sf::Text checkText;
            checkText.setFont(font);
            checkText.setString("✓");
            checkText.setCharacterSize(14);
            checkText.setFillColor(sf::Color::Black);
            checkText.setPosition(x + 133.f, y + 5.f);
            window.draw(checkText);
        }

        // ---- CHARACTER NAME ----
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(charName);
        nameText.setCharacterSize(16);

        if (isSelected)
        {
            nameText.setFillColor(sf::Color::Yellow);
        }
        else
        {
            nameText.setFillColor(sf::Color::White);
        }

        nameText.setPosition(x + 15.f, y + 145.f);
        window.draw(nameText);

        // ---- TYPE ----
        sf::Text typeText;
        typeText.setFont(font);
        typeText.setString(charType);
        typeText.setCharacterSize(11);
        typeText.setFillColor(sf::Color::Cyan);
        typeText.setPosition(x + 15.f, y + 168.f);
        window.draw(typeText);

        // ---- STATS ----
        sf::Text stat1Text;
        stat1Text.setFont(font);
        stat1Text.setString(stat1);
        stat1Text.setCharacterSize(11);
        stat1Text.setFillColor(sf::Color(180, 180, 180));
        stat1Text.setPosition(x + 15.f, y + 190.f);
        window.draw(stat1Text);

        sf::Text stat2Text;
        stat2Text.setFont(font);
        stat2Text.setString(stat2);
        stat2Text.setCharacterSize(11);
        stat2Text.setFillColor(sf::Color(180, 180, 180));
        stat2Text.setPosition(x + 15.f, y + 212.f);
        window.draw(stat2Text);
    }

    // ==========================================
    // HELPER - Show selected character info
    // ==========================================

    void drawSelectedInfo(sf::RenderWindow& window)
    {
        std::string selectedName = "";
        std::string selectedDesc = "";

        if (selectedCharacter == 0)
        {
            selectedName = "NICK";
            selectedDesc = "Balanced fighter - good for beginners";
        }
        else if (selectedCharacter == 1)
        {
            selectedName = "SPEEDY";
            selectedDesc = "Fastest character - hard to catch!";
        }
        else if (selectedCharacter == 2)
        {
            selectedName = "RANGER";
            selectedDesc = "Longest snowball range - snipe enemies!";
        }

        // Info bar background
        sf::RectangleShape infoBar(sf::Vector2f(540.f, 35.f));
        infoBar.setFillColor(sf::Color(20, 20, 60));
        infoBar.setOutlineColor(sf::Color::Yellow);
        infoBar.setOutlineThickness(1.f);
        infoBar.setPosition(30.f, 430.f);
        window.draw(infoBar);

        // Selected text
        sf::Text selectedText;
        selectedText.setFont(font);
        selectedText.setString("Selected: " + selectedName + "  |  " + selectedDesc);
        selectedText.setCharacterSize(11);
        selectedText.setFillColor(sf::Color::Yellow);
        selectedText.setPosition(40.f, 441.f);
        window.draw(selectedText);
    }
};