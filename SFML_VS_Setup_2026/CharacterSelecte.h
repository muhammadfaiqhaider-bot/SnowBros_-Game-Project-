#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class CharacterSelect
{
private:
    sf::Font font;

    // Which character is selected
    int selectedCharacter;      // 0=Nick, 1=Speedy, 2=Ranger

    // Hover effects
    bool nickHovered;
    bool char1Hovered;
    bool char2Hovered;
    bool startHovered;

    // Character images (load your own sprites here)
    sf::Texture nickTexture;
    sf::Texture char1Texture;
    sf::Texture char2Texture;

    sf::Sprite nickSprite;
    sf::Sprite char1Sprite;
    sf::Sprite char2Sprite;

    bool nickImgLoaded;
    bool char1ImgLoaded;
    bool char2ImgLoaded;

public:
    CharacterSelect()
    {
        font.loadFromFile("assets/Text.ttf");

        selectedCharacter = 0;      // Nick selected by default

        nickHovered = false;
        char1Hovered = false;
        char2Hovered = false;
        startHovered = false;

        nickImgLoaded = false;
        char1ImgLoaded = false;
        char2ImgLoaded = false;

        // ---- Load character images (swap paths with your actual assets) ----
        if (nickTexture.loadFromFile("assets/MainPlayer.png"))
        {
            nickImgLoaded = true;
            nickSprite.setTexture(nickTexture);

            // Scale to fit inside the box (120x140)
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

            // Nick box hover      (x:55-175, y:175-330)
            nickHovered = (mouseX >= 55 && mouseX <= 175 &&
                mouseY >= 175 && mouseY <= 330);

            // Speedy box hover    (x:240-360, y:175-330)
            char1Hovered = (mouseX >= 240 && mouseX <= 360 &&
                mouseY >= 175 && mouseY <= 330);

            // Ranger box hover    (x:425-545, y:175-330)
            char2Hovered = (mouseX >= 425 && mouseX <= 545 &&
                mouseY >= 175 && mouseY <= 330);

            // Start button hover  (x:200-400, y:510-555)
            startHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 510 && mouseY <= 555);
        }

        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            // Nick box clicked
            if (mouseX >= 55 && mouseX <= 175 &&
                mouseY >= 175 && mouseY <= 330)
            {
                selectedCharacter = 0;
            }

            // Speedy box clicked
            if (mouseX >= 240 && mouseX <= 360 &&
                mouseY >= 175 && mouseY <= 330)
            {
                selectedCharacter = 1;
            }

            // Ranger box clicked
            if (mouseX >= 425 && mouseX <= 545 &&
                mouseY >= 175 && mouseY <= 330)
            {
                selectedCharacter = 2;
            }

            // Start Game button clicked
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 510 && mouseY <= 555)
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
        background.setFillColor(sf::Color(128, 0, 128));
        window.draw(background);

       

        // ---- TITLE ----
        sf::Text title;
        title.setFont(font);
        title.setString("SELECT CHARACTER");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color::Cyan);
        title.setPosition(157.0f, 50.f);
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
        instruction.setPosition(190.f, 115.f);
        window.draw(instruction);

        // ---- CHARACTER BOXES ----
        // Box size is smaller now: 120x140  (just for image display)
        // All attribute text is drawn BELOW each box, outside of it

        drawCharacterBox(window,
            55.f, 175.f,               // position
            sf::Color(30, 80, 180),     // fallback color if no image (blue - Nick)
            "      NICK",                     // name shown below box
            "     Balanced",                 // single attribute shown below name
            nickSprite,                 // sprite
            nickImgLoaded,              // use image?
            selectedCharacter == 0,     // is selected?
            nickHovered                 // is hovered?
        );

        drawCharacterBox(window,
            240.f, 175.f,
            sf::Color(180, 30, 30),     // fallback color (red - Speedy)
            "FAIQ SIGMA GUY",
            "   Speedster",
            char1Sprite,
            char1ImgLoaded,
            selectedCharacter == 1,
            char1Hovered
        );

        drawCharacterBox(window,
            425.f, 175.f,
            sf::Color(180, 150, 0),     // fallback color (gold - Ranger)
            "CHEEMA TUFF GUY",
            "    Long Range",
            char2Sprite,
            char2ImgLoaded,
            selectedCharacter == 2,
            char2Hovered
        );

        // ---- SELECTED CHARACTER INFO BAR ----
        drawSelectedInfo(window);

        // ---- START BUTTON (pushed down to y:510) ----
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

        startButton.setPosition(200.f, 510.f);
        window.draw(startButton);

        sf::Text startText;
        startText.setFont(font);
        startText.setString("START GAME");
        startText.setCharacterSize(16);
        startText.setFillColor(sf::Color::White);
        startText.setPosition(228.f, 523.f);
        window.draw(startText);

        // ---- BACK HINT ----
        sf::Text backHint;
        backHint.setFont(font);
        backHint.setString("Press ESC to go back");
        backHint.setCharacterSize(12);
        backHint.setFillColor(sf::Color(80, 80, 120));
        backHint.setPosition(205.f, 575.f);
        window.draw(backHint);
    }

    // Getter
    int getSelectedCharacter()
    {
        return selectedCharacter;
    }

private:

    // ==========================================
    // HELPER - Draw one character box (image only, small)
    // Name + attribute label are drawn OUTSIDE below the box
    // ==========================================

    void drawCharacterBox(sf::RenderWindow& window,
        float x, float y,
        sf::Color fallbackColor,
        std::string charName,
        std::string charType,
        sf::Sprite& charSprite,
        bool imgLoaded,
        bool isSelected,
        bool isHovered)
    {
        // Box dimensions - smaller now, just for the image
        const float BOX_W = 120.f;
        const float BOX_H = 140.f;

        // ---- BOX BACKGROUND ----
        sf::RectangleShape box(sf::Vector2f(BOX_W, BOX_H));

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

        // ---- CHARACTER IMAGE or fallback snowman ----
        if (imgLoaded)
        {
            // Position image centered inside box
            charSprite.setPosition(x, y);
            window.draw(charSprite);
        }
        else
        {
            // Fallback: simple snowman shape if image not loaded
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

            // Eyes
            sf::CircleShape eye1(3.f);
            eye1.setFillColor(sf::Color::White);
            eye1.setPosition(x + 48.f, y + 38.f);
            window.draw(eye1);

            sf::CircleShape eye2(3.f);
            eye2.setFillColor(sf::Color::White);
            eye2.setPosition(x + 60.f, y + 38.f);
            window.draw(eye2);
        }

        // ---- SELECTED CHECKMARK (top-right corner of box) ----
        if (isSelected)
        {
            sf::RectangleShape checkBg(sf::Vector2f(20.f, 20.f));
            checkBg.setFillColor(sf::Color::Yellow);
            checkBg.setPosition(x + BOX_W - 22.f, y + 2.f);
            window.draw(checkBg);

            sf::Text checkText;
            checkText.setFont(font);
            checkText.setString("v");           // simple checkmark
            checkText.setCharacterSize(13);
            checkText.setFillColor(sf::Color::Black);
            checkText.setPosition(x + BOX_W - 19.f, y + 3.f);
            window.draw(checkText);
        }

        // ---- CHARACTER NAME  (outside the box, below it) ----
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(charName);
        nameText.setCharacterSize(15);
        nameText.setFillColor(isSelected ? sf::Color::Yellow : sf::Color::White);
        nameText.setPosition(x + 10.f, y + BOX_H + 8.f);
        window.draw(nameText);

        // ---- SINGLE ATTRIBUTE LABEL  (outside the box, below name) ----
        sf::Text typeText;
        typeText.setFont(font);
        typeText.setString(charType);
        typeText.setCharacterSize(12);
        typeText.setFillColor(sf::Color::Cyan);
        typeText.setPosition(x + 10.f, y + BOX_H + 28.f);
        window.draw(typeText);
    }

    // ==========================================
    // HELPER - Show selected character info bar
    // ==========================================

    void drawSelectedInfo(sf::RenderWindow& window)
    {
        std::string selectedName = "";
        std::string selectedDesc = "";

        if (selectedCharacter == 0)
        {
            selectedName = "NICK";
            selectedDesc = "By choosing me You will Get nothing ";
        }
        else if (selectedCharacter == 1)
        {
            selectedName = "SPEEDY";
            selectedDesc = "By Choosing me you will get 100000+ Aura Points";
        }
        else if (selectedCharacter == 2)
        {
            selectedName = "RANGER";
            selectedDesc = "By Choosing me you will fell Tall and Handsome";
        }

        // Info bar pushed down to sit between boxes and start button
        sf::RectangleShape infoBar(sf::Vector2f(540.f, 35.f));
        infoBar.setFillColor(sf::Color(20, 20, 60));
        infoBar.setOutlineColor(sf::Color::Yellow);
        infoBar.setOutlineThickness(1.f);
        infoBar.setPosition(30.f, 465.f);
        window.draw(infoBar);

        sf::Text selectedText;
        selectedText.setFont(font);
        selectedText.setString("Selected: " + selectedName + "  |  " + selectedDesc);
        selectedText.setCharacterSize(11);
        selectedText.setFillColor(sf::Color::Yellow);
        selectedText.setPosition(40.f, 476.f);
        window.draw(selectedText);
    }
};