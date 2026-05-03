#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class CharacterSelect
{
private:
    sf::Font font;
    // Animated background (same as MainMenu)
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

        bgAnimLoaded = false;
        bgCurrentFrame = 0;
        bgAnimTimer = 0;
        // Load the same large starry image used by MainMenu and split into chunks
        sf::Image fullImage;
        if (fullImage.loadFromFile("assets/login_bg.png"))
        {
            bgAnimLoaded = true;
            for (int c = 0; c < CHUNK_COUNT; c++)
            {
                sf::IntRect chunkRect(
                    c * FRAMES_PER_CHUNK * BG_FRAME_W,
                    0,
                    FRAMES_PER_CHUNK * BG_FRAME_W,
                    BG_FRAME_H
                );
                bgChunks[c].loadFromImage(fullImage, chunkRect);
            }
            bgAnimSprite.setTexture(bgChunks[0]);
            bgAnimSprite.setTextureRect(sf::IntRect(0, 0, BG_FRAME_W, BG_FRAME_H));
            bgAnimSprite.setScale(600.f / BG_FRAME_W, 600.f / BG_FRAME_H);
        }

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


    int handleEvents(sf::Event& event)
    {
        if (event.type == sf::Event::MouseMoved)
        {
            float mx = event.mouseMove.x;
            float my = event.mouseMove.y;

            nickHovered = (mx >= 55 && mx <= 175 && my >= 175 && my <= 330);
            char1Hovered = (mx >= 240 && mx <= 360 && my >= 175 && my <= 330);
            char2Hovered = (mx >= 425 && mx <= 545 && my >= 175 && my <= 330);
            // mode buttons moved slightly down: left=130..230 and 298..418, y=470..505
            twoPlayerHovered = (mx >= 358 && mx <= 468 && my >= 470 && my <= 505);
            onePlayerHovered = (mx >= 130 && mx <= 230 && my >= 470 && my <= 505);
            // start button moved slightly down
            startHovered = (mx >= 200 && mx <= 400 && my >= 525 && my <= 570);
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mx = event.mouseButton.x;
            float my = event.mouseButton.y;

            //  Nick box 
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
            onePlayerHovered = (mx >= 130 && mx <= 230 && my >= 470 && my <= 505);
            twoPlayerHovered = (mx >= 358 && mx <= 468 && my >= 470 && my <= 505);

            if (mx >= 130 && mx <= 230 && my >= 470 && my <= 505)
            {
                onePlayerSelected = true;
                selectedCharacter2 = -1;
            }
            if (mx >= 358 && mx <= 468 && my >= 470 && my <= 505)
                onePlayerSelected = false;

            // --- Start button ---
            if (mx >= 200 && mx <= 400 && my >= 525 && my <= 570)
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
    void draw(sf::RenderWindow& window)
    {
        // advance animated background (matches MainMenu behavior)
        update();

        if (bgAnimLoaded)
        {
            window.draw(bgAnimSprite);
        }
        else
        {
            // Use the same dark fallback as MainMenu so screen isn't bright blue
            sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
            background.setFillColor(sf::Color(5, 10, 30));
            window.draw(background);
        }

        sf::Text title;
        title.setFont(font);
        title.setString("SELECT CHARACTER");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(218, 152, 227));
        title.setPosition(157.0f, 50.f);
        window.draw(title);

        sf::RectangleShape underline(sf::Vector2f(400.f, 3.f));
        underline.setFillColor(sf::Color(218, 152, 227));
        underline.setPosition(100.f, 95.f);
        window.draw(underline);

        sf::Text instruction;
        instruction.setFont(font);
        instruction.setString("Click a character to select");
        instruction.setCharacterSize(14);
        instruction.setFillColor(sf::Color(218, 152, 227));
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
        onePBtn.setPosition(130.f, 470.f);
        if (onePlayerSelected)
        {
            onePBtn.setFillColor(sf::Color(46, 17, 51));
            onePBtn.setOutlineColor(sf::Color(218, 152, 227));
            onePBtn.setOutlineThickness(onePlayerHovered ? 2.f : 1.f);
        }
        else
        {
            onePBtn.setFillColor(sf::Color(41, 14, 46));
            onePBtn.setOutlineColor(sf::Color(102, 54, 112));
            onePBtn.setOutlineThickness(onePlayerHovered ? 2.f : 1.f);
        }
        window.draw(onePBtn);

        sf::Text onePText;
        onePText.setFont(font);
        onePText.setString("1 PLAYER");
        onePText.setCharacterSize(13);
        onePText.setFillColor(sf::Color::White);
        // center text inside 1P button
        {
            sf::FloatRect tb = onePText.getLocalBounds();
            onePText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            onePText.setPosition(130.f + 100.f / 2.f, 470.f + 35.f / 2.f);
        }
        window.draw(onePText);

        // --- 2P button ---
        sf::RectangleShape twoPBtn(sf::Vector2f(110.f, 35.f));
        twoPBtn.setPosition(358.f, 470.f);
        if (!onePlayerSelected)
        {
            twoPBtn.setFillColor(sf::Color(46, 17, 51));
            twoPBtn.setOutlineColor(sf::Color(218, 152, 227));
            twoPBtn.setOutlineThickness(twoPlayerHovered ? 2.f : 1.f);
        }
        else
        {
            twoPBtn.setFillColor(sf::Color(41, 14, 46));
            twoPBtn.setOutlineColor(sf::Color(102, 54, 112));
            twoPBtn.setOutlineThickness(twoPlayerHovered ? 2.f : 1.f);
        }
        window.draw(twoPBtn);

        sf::Text twoPText;
        twoPText.setFont(font);
        twoPText.setString("2 PLAYERS");
        twoPText.setCharacterSize(13);
        twoPText.setFillColor(sf::Color::White);
            // center text inside the 2P button
        {
            sf::FloatRect tb = twoPText.getLocalBounds();
            twoPText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            twoPText.setPosition(358.f + 110.f / 2.f, 470.f + 35.f / 2.f);
        }
        window.draw(twoPText);

        // --- Start button ---
        sf::RectangleShape startButton(sf::Vector2f(200.f, 45.f));
        // Use same theme as MainMenu buttons
        if (startHovered)
        {
            startButton.setFillColor(sf::Color(46, 17, 51));
            startButton.setOutlineColor(sf::Color(218, 152, 227));
            startButton.setOutlineThickness(2.f);
        }
        else
        {
            startButton.setFillColor(sf::Color(41, 14, 46));
            startButton.setOutlineColor(sf::Color(102, 54, 112));
            startButton.setOutlineThickness(1.f);
        }
        startButton.setPosition(200.f, 525.f);
        window.draw(startButton);

        sf::Text startText;
        startText.setFont(font);
        startText.setString("START GAME");
        startText.setCharacterSize(16);
        startText.setFillColor(sf::Color(218, 152, 227));
        {
            sf::FloatRect tb = startText.getLocalBounds();
            startText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            startText.setPosition(200.f + 200.f / 2.f, 525.f + 45.f / 2.f);
        }
        window.draw(startText);

        sf::Text backHint;
        backHint.setFont(font);
        backHint.setString("Press ESC to go back");
        backHint.setCharacterSize(12);
        backHint.setFillColor(sf::Color(80, 80, 120));
        backHint.setPosition(225.f, 575.f);
        window.draw(backHint);
    }
    int getSelectedCharacter() const 
    { 
        return selectedCharacter; 
    }
    int getP2Character()       const 
    { return 
        selectedCharacter2; 
    }
    int getPlayerMode()        const 
    { 
        return onePlayerSelected ? 1 : 2; 
    }

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
            box.setFillColor(sf::Color(218, 152, 227));
            box.setOutlineColor(sf::Color(218, 152, 227));
            box.setOutlineThickness(3.f);
        }
        else if (isHovered)
        {
            box.setFillColor(sf::Color(218, 152, 227));
            box.setOutlineColor(sf::Color(218, 152, 227));
            box.setOutlineThickness(2.f);
        }
        else
        {
            box.setFillColor(sf::Color(218, 152, 227));
            box.setOutlineColor(sf::Color(218, 152, 227));
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
        infoBar.setFillColor(sf::Color(46, 17, 51));
        infoBar.setOutlineColor(sf::Color(218, 152, 227));
        infoBar.setOutlineThickness(1.f);
        infoBar.setPosition(30.f, 400.f);
        window.draw(infoBar);

        sf::Text selectedText;
        selectedText.setFont(font);
        selectedText.setString("P1: " + selectedName + "  |  " + selectedDesc);
        selectedText.setCharacterSize(11);
        selectedText.setFillColor(sf::Color::White);
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