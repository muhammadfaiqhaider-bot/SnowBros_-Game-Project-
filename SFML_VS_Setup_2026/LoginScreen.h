#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "AuthenticationSystem.h"

class LoginScreen
{
private:
    sf::Font font;
    sf::Font text;
    sf::Font subTit;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

    // Input fields
    std::string usernameInput;
    std::string passwordInput;

    // Which box is active at the moment
    bool typingUsername;
    bool typingPassword;

    // Toggle between login and register
    bool showingRegister;

    // Message for error or success
    std::string message;
    sf::Color messageColor;

    // Hover effects
    bool mainButtonHovered;
    bool switchButtonHovered;

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
    LoginScreen()
    {
        font.loadFromFile("assets/Title.ttf");
        text.loadFromFile("assets/Text.ttf");
        subTit.loadFromFile("asstes/Subtitle.ttf");
        bgCurrentFrame = 0;
        bgAnimTimer = 0;
        bgAnimLoaded = false;

        sf::Image fullImage;
        if (fullImage.loadFromFile("assets/login_bg.png"))
        {
            bgAnimLoaded = true;

            for (int c = 0; c < CHUNK_COUNT; c++)
            {
                // Each chunk is 25 frames wide
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
        usernameInput = "";
        passwordInput = "";
        typingUsername = true;
        typingPassword = false;
        showingRegister = false;
        message = "";
        messageColor = sf::Color::Red;
        mainButtonHovered = false;
        switchButtonHovered = false;
    }

    // ==========================================
    // HANDLE EVENTS
    // Returns:
    // 0 = stay on login
    // 1 = go to main menu
    // ==========================================

    int handleEvents(sf::Event& event, AuthManager& auth)
    {
        // Mouse hover effects
        if (event.type == sf::Event::MouseMoved)
        {
            float mouseX = event.mouseMove.x;
            float mouseY = event.mouseMove.y;

            mainButtonHovered = (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 340 && mouseY <= 395);

            switchButtonHovered = (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 406 && mouseY <= 445);
        }

        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            // Username box clicked
            if (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 200 && mouseY <= 250)
            {
                typingUsername = true;
                typingPassword = false;
            }

            // Password box clicked
            else if (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 270 && mouseY <= 315)
            {
                typingPassword = true;
                typingUsername = false;
            }

            // Main button clicked (LOGIN or REGISTER)
            else if (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 340 && mouseY <= 395)
            {
                if (usernameInput == "" || passwordInput == "")
                {
                    message = "Please fill all fields!";
                    messageColor = sf::Color(255, 80, 80);
                }
                else if (showingRegister)
                {
                    // REGISTER mode
                    if (auth.registerUser(usernameInput, passwordInput, ""))
                    {
                        message = "Account created! Now login.";
                        messageColor = sf::Color(80, 220, 255);
                        showingRegister = false;
                        usernameInput = "";
                        passwordInput = "";
                    }
                    else
                    {
                        message = "Username already taken!";
                        messageColor = sf::Color(255, 80, 80);
                    }
                }
                else
                {
                    // LOGIN mode
                    if (auth.loginUser(usernameInput, passwordInput))
                    {
                        message = "Welcome!";
                        messageColor = sf::Color(80, 220, 255);
                        return 1;       // Go to main menu
                    }
                    else
                    {
                        message = "Wrong username or password!";
                        messageColor = sf::Color(255, 80, 80);
                    }
                }
            }

            // Switch button clicked
            else if (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 406 && mouseY <= 445)
            {
                showingRegister = !showingRegister;
                message = "";
                usernameInput = "";
                passwordInput = "";
                typingUsername = false;
                typingPassword = false;
            }

            // Clicked elsewhere - deselect
            else
            {
                typingUsername = false;
                typingPassword = false;
            }
        }

        // Keyboard typing
        if (event.type == sf::Event::TextEntered)
        {
            // Backspace
            if (event.text.unicode == 8)
            {
                if (typingUsername && usernameInput.length() > 0)
                {
                    usernameInput.pop_back();
                }
                if (typingPassword && passwordInput.length() > 0)
                {
                    passwordInput.pop_back();
                }
            }
            // Normal character
            else if (event.text.unicode >= 32 && event.text.unicode < 128)
            {
                if (typingUsername && usernameInput.length() < 20)
                {
                    usernameInput = usernameInput + (char)event.text.unicode;
                }
                if (typingPassword && passwordInput.length() < 20)
                {
                    passwordInput = passwordInput + (char)event.text.unicode;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed)
{
    if (event.key.code == sf::Keyboard::Tab)
    {
        typingUsername = !typingUsername;
        typingPassword = !typingPassword;
    }

    if (event.key.code == sf::Keyboard::Return)
    {
        if (typingUsername)
        {
            typingUsername = false;
            typingPassword = true;
        }
        else if (typingPassword)
        {
            if (usernameInput == "" || passwordInput == "")
            {
                message = "Please fill all fields!";
                messageColor = sf::Color(255, 80, 80);
            }
            else if (showingRegister)
            {
                if (auth.registerUser(usernameInput, passwordInput, ""))
                {
                    message = "Account created! Now login.";
                    messageColor = sf::Color(80, 220, 255);
                    showingRegister = false;
                    usernameInput = "";
                    passwordInput = "";
                    typingUsername = true;
                    typingPassword = false;
                }
                else
                {
                    message = "Username already taken!";
                    messageColor = sf::Color(255, 80, 80);
                }
            }
            else
            {
                if (auth.loginUser(usernameInput, passwordInput))
                {
                    message = "Welcome!";
                    messageColor = sf::Color(80, 220, 255);
                    return 1;
                }
                else
                {
                    message = "Wrong username or password!";
                    messageColor = sf::Color(255, 80, 80);
                }
            }
        }
    }
}
        return 0;
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
        if (bgAnimLoaded)
            window.draw(bgAnimSprite);
        else
        {
            sf::RectangleShape bg(sf::Vector2f(600.f, 600.f));
            bg.setFillColor(sf::Color(5, 10, 30));
            window.draw(bg);
        }

        // ---- SUBTLE DARK OVERLAY ----
        // Makes text readable over the starry background
        sf::RectangleShape overlay(sf::Vector2f(600.f, 600.f));
        overlay.setFillColor(sf::Color(0, 5, 20, 80));
        window.draw(overlay);

        // ---- GAME TITLE above box ----
        sf::Text title;
        title.setFont(font);
        title.setString("SNOW BROS");
        title.setCharacterSize(42);
        title.setFillColor(sf::Color(218, 152, 227));
        title.setPosition(175.f, 55.f);
        window.draw(title);

        // Title glow line below
        sf::RectangleShape titleLine(sf::Vector2f(290.f, 2.f));
        titleLine.setFillColor(sf::Color(188, 109, 199));
        titleLine.setPosition(155.f, 105.f);
        window.draw(titleLine);


        // Main panel
        sf::RectangleShape panel(sf::Vector2f(260.f, 380.f));
        panel.setFillColor(sf::Color(23, 8, 26));
        panel.setOutlineColor(sf::Color(218, 152, 227));
        panel.setOutlineThickness(1.5f);
        panel.setPosition(170.f, 130.f);
        window.draw(panel);

        // Panel top accent line
        sf::RectangleShape topAccent(sf::Vector2f(260.f, 3.f));
        topAccent.setFillColor(sf::Color(189, 113, 199));
        topAccent.setPosition(170.f, 130.f);
        window.draw(topAccent);

        // ---- PANEL TITLE ----
        std::string panelTitle = showingRegister ? "CREATE ACCOUNT" : "SIGN IN";

        sf::Text panelTitleText;
        panelTitleText.setFont(font);
        panelTitleText.setString(panelTitle);
        panelTitleText.setCharacterSize(16);
        panelTitleText.setFillColor(sf::Color(218, 152, 227));

        // Center panel title
        float ptWidth = panelTitle.length() * 9.5f;
        panelTitleText.setPosition(170.f + (260.f - ptWidth) / 2.f, 148.f);
        window.draw(panelTitleText);



        // ---- USERNAME LABEL ----
        sf::Text usernameLabel;
        usernameLabel.setFont(text);
        usernameLabel.setString("Username");
        usernameLabel.setCharacterSize(13);
        usernameLabel.setFillColor(sf::Color(218, 152, 227));
        usernameLabel.setPosition(178.f, 183.f);
        window.draw(usernameLabel);

        // ---- USERNAME BOX ----
        sf::RectangleShape usernameBox(sf::Vector2f(240.f, 36.f));
        if (typingUsername)
        {
            usernameBox.setFillColor(sf::Color(46, 17, 51));
            usernameBox.setOutlineColor(sf::Color(218, 152, 227));
            usernameBox.setOutlineThickness(2.f);
        }
        else
        {
            usernameBox.setFillColor(sf::Color(41, 14, 46));
            usernameBox.setOutlineColor(sf::Color(102, 54, 112));
            usernameBox.setOutlineThickness(1.f);
        }
        usernameBox.setPosition(178.f, 202.f);
        window.draw(usernameBox);

        // Username content or placeholder
        sf::Text usernameContent;
        usernameContent.setFont(text);
        usernameContent.setCharacterSize(13);
        if (usernameInput.empty())
        {
            usernameContent.setString("Enter your username...");
            usernameContent.setFillColor(sf::Color(102, 54, 112));
        }
        else
        {
            usernameContent.setString(usernameInput);
            usernameContent.setFillColor(sf::Color::White);
        }
        usernameContent.setPosition(185.f, 212.f);
        window.draw(usernameContent);

        // ---- PASSWORD LABEL ----
        sf::Text passwordLabel;
        passwordLabel.setFont(text);
        passwordLabel.setString("Password");
        passwordLabel.setCharacterSize(13);
        passwordLabel.setFillColor(sf::Color(218, 152, 227));
        passwordLabel.setPosition(178.f, 255.f);
        window.draw(passwordLabel);

        // ---- PASSWORD BOX ----
        sf::RectangleShape passwordBox(sf::Vector2f(240.f, 36.f));
        if (typingPassword)
        {
            passwordBox.setFillColor(sf::Color(46, 17, 51));
            passwordBox.setOutlineColor(sf::Color(218, 152, 227));
            passwordBox.setOutlineThickness(2.f);
        }
        else
        {
            passwordBox.setFillColor(sf::Color(41, 14, 46));
            passwordBox.setOutlineColor(sf::Color(102, 54, 112));
            passwordBox.setOutlineThickness(1.f);
        }
        passwordBox.setPosition(178.f, 272.f);
        window.draw(passwordBox);

        // Password content or placeholder
        sf::Text passwordContent;
        passwordContent.setFont(text);
        passwordContent.setCharacterSize(13);
        if (passwordInput.empty())
        {
            passwordContent.setString("Enter your password...");
            passwordContent.setFillColor(sf::Color(102, 54, 112));
        }
        else
        {
            std::string stars = "";
            for (int i = 0; i < passwordInput.length(); i++)
            {
                stars = stars + "*";
            }
            passwordContent.setString(stars);
            passwordContent.setFillColor(sf::Color::White);
        }
        passwordContent.setPosition(185.f, 282.f);
        window.draw(passwordContent);

        // ---- PASSWORD HINT ----
        sf::Text hint;
        hint.setFont(text);
        hint.setString("Use 6 or more characters");
        hint.setCharacterSize(10);
        hint.setFillColor(sf::Color(102, 54, 112));
        hint.setPosition(178.f, 315.f);
        window.draw(hint);

        // ---- DIVIDER LINE ----
        sf::RectangleShape divider(sf::Vector2f(240.f, 1.f));
        divider.setFillColor(sf::Color(30, 70, 120, 150));
        divider.setPosition(178.f, 340.f);
        window.draw(divider);

        // ---- MAIN BUTTON (LOGIN or REGISTER) ----
        sf::RectangleShape mainButton(sf::Vector2f(240.f, 40.f));
        if (mainButtonHovered)
        {
            mainButton.setFillColor(sf::Color(165, 64, 179));
            mainButton.setOutlineColor(sf::Color(218, 152, 227));
            mainButton.setOutlineThickness(2.f);
        }
        else
        {
            mainButton.setFillColor(sf::Color(106, 37, 115));
            mainButton.setOutlineColor(sf::Color(189, 113, 199));
            mainButton.setOutlineThickness(1.5f);
        }
        mainButton.setPosition(178.f, 355.f);
        window.draw(mainButton);

        std::string btnLabel = showingRegister ? "CREATE ACCOUNT" : "LOGIN";

        sf::Text mainBtnText;
        mainBtnText.setFont(text);
        mainBtnText.setString(btnLabel);
        mainBtnText.setCharacterSize(14);
        mainBtnText.setFillColor(sf::Color(200, 235, 255));

        float btnTextWidth = btnLabel.length() * 8.5f;
        mainBtnText.setPosition(178.f + (240.f - btnTextWidth) / 2.f, 367.f);
        window.draw(mainBtnText);

        // ---- SWITCH BUTTON ----
        sf::RectangleShape switchButton(sf::Vector2f(240.f, 35.f));
        if (switchButtonHovered)
        {
            switchButton.setFillColor(sf::Color(40, 13, 46));
            switchButton.setOutlineColor(sf::Color(156, 62, 176));
            switchButton.setOutlineThickness(1.5f);
        }
        else
        {
            switchButton.setFillColor(sf::Color(27, 9, 31));
            switchButton.setOutlineColor(sf::Color(102, 54, 112));
            switchButton.setOutlineThickness(1.f);
        }
        switchButton.setPosition(178.f, 408.f);
        window.draw(switchButton);

        std::string switchLabel = showingRegister ?
            "Already have account? Login" :
            "No account? Register here";

        sf::Text switchText;
        switchText.setFont(text);
        switchText.setString(switchLabel);
        switchText.setCharacterSize(11);
        switchText.setFillColor(sf::Color(218, 152, 227));

        float switchTextWidth = switchLabel.length() * 6.5f;
        switchText.setPosition(178.f + (240.f - switchTextWidth) / 2.f, 418.f);
        window.draw(switchText);

        // ---- MESSAGE ----
        if (message != "")
        {
            sf::RectangleShape msgBg(sf::Vector2f(240.f, 26.f));
            msgBg.setFillColor(sf::Color(5, 10, 30, 200));
            msgBg.setOutlineColor(messageColor);
            msgBg.setOutlineThickness(1.f);
            msgBg.setPosition(178.f, 458.f);
            window.draw(msgBg);

            sf::Text messageText;
            messageText.setFont(font);
            messageText.setString(message);
            messageText.setCharacterSize(11);
            messageText.setFillColor(messageColor);
            messageText.setPosition(185.f, 465.f);
            window.draw(messageText);
        }

        // ---- CURSOR in active box ----
        if (typingUsername)
        {
            sf::RectangleShape cursor(sf::Vector2f(2.f, 18.f));
            cursor.setFillColor(sf::Color(80, 180, 255));
            cursor.setPosition(185.f + usernameInput.length() * 8.f, 212.f);
            window.draw(cursor);
        }
        if (typingPassword)
        {
            sf::RectangleShape cursor(sf::Vector2f(2.f, 18.f));
            cursor.setFillColor(sf::Color(80, 180, 255));
            cursor.setPosition(185.f + passwordInput.length() * 8.f, 282.f);
            window.draw(cursor);
        }
    }

    std::string getUsernameInput()
    {
        return usernameInput;
    }
};