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

public:
    LoginScreen()
    {
        font.loadFromFile("assets/Title.ttf");
        text.loadFromFile("assets/Text.ttf");
        subTit.loadFromFile("asstes/Subtitle.ttf");


        // Load your starry background image
        backgroundLoaded = backgroundTexture.loadFromFile("assets/login_bg.png");
        if (backgroundLoaded)
        {
            backgroundSprite.setTexture(backgroundTexture);

            // Scale to fit 600x600 window
            float scaleX = 600.f / backgroundTexture.getSize().x;
            float scaleY = 600.f / backgroundTexture.getSize().y;
            backgroundSprite.setScale(scaleX, scaleY);
        }

        usernameInput = "";
        passwordInput = "";
        typingUsername = false;
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
                mouseY >= 385 && mouseY <= 425);

            switchButtonHovered = (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 440 && mouseY <= 475);
        }

        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            // Username box clicked
            if (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 215 && mouseY <= 255)
            {
                typingUsername = true;
                typingPassword = false;
            }

            // Password box clicked
            else if (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 305 && mouseY <= 345)
            {
                typingPassword = true;
                typingUsername = false;
            }

            // Main button clicked (LOGIN or REGISTER)
            else if (mouseX >= 175 && mouseX <= 425 &&
                mouseY >= 385 && mouseY <= 425)
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
                mouseY >= 440 && mouseY <= 475)
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

        return 0;
    }

    // ==========================================
    // DRAW
    // ==========================================

    void draw(sf::RenderWindow& window)
    {
        // ---- BACKGROUND IMAGE ----
        if (backgroundLoaded)
        {
            window.draw(backgroundSprite);
        }
        else
        {
            // Fallback - dark blue if no image
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
        title.setFillColor(sf::Color(120, 220, 255));
        title.setPosition(155.f, 55.f);
        window.draw(title);

        // Title glow line below
        sf::RectangleShape titleLine(sf::Vector2f(290.f, 2.f));
        titleLine.setFillColor(sf::Color(80, 180, 255, 180));
        titleLine.setPosition(155.f, 105.f);
        window.draw(titleLine);

        // ---- CENTRE PANEL ----
        // Outer glow effect
        sf::RectangleShape panelGlow(sf::Vector2f(268.f, 388.f));
        panelGlow.setFillColor(sf::Color(0, 0, 0, 0));
        panelGlow.setOutlineColor(sf::Color(60, 140, 220, 80));
        panelGlow.setOutlineThickness(6.f);
        panelGlow.setPosition(165.f, 125.f);
        window.draw(panelGlow);

        // Main panel
        sf::RectangleShape panel(sf::Vector2f(260.f, 380.f));
        panel.setFillColor(sf::Color(5, 15, 45, 210));
        panel.setOutlineColor(sf::Color(60, 140, 220, 200));
        panel.setOutlineThickness(1.5f);
        panel.setPosition(170.f, 130.f);
        window.draw(panel);

        // Panel top accent line
        sf::RectangleShape topAccent(sf::Vector2f(260.f, 3.f));
        topAccent.setFillColor(sf::Color(80, 180, 255));
        topAccent.setPosition(170.f, 130.f);
        window.draw(topAccent);

        // ---- PANEL TITLE ----
        std::string panelTitle = showingRegister ? "CREATE ACCOUNT" : "SIGN IN";

        sf::Text panelTitleText;
        panelTitleText.setFont(font);
        panelTitleText.setString(panelTitle);
        panelTitleText.setCharacterSize(16);
        panelTitleText.setFillColor(sf::Color(150, 210, 255));

        // Center panel title
        float ptWidth = panelTitle.length() * 9.5f;
        panelTitleText.setPosition(170.f + (260.f - ptWidth) / 2.f, 148.f);
        window.draw(panelTitleText);

        // Small stars decoration around title
        for (int i = 0; i < 3; i++)
        {
            sf::CircleShape star(2.f);
            star.setFillColor(sf::Color(150, 210, 255, 180));
            star.setPosition(180.f + i * 8.f, 155.f);
            window.draw(star);

            sf::CircleShape star2(2.f);
            star2.setFillColor(sf::Color(150, 210, 255, 180));
            star2.setPosition(400.f - i * 8.f, 155.f);
            window.draw(star2);
        }

        // ---- USERNAME LABEL ----
        sf::Text usernameLabel;
        usernameLabel.setFont(text);
        usernameLabel.setString("Username");
        usernameLabel.setCharacterSize(13);
        usernameLabel.setFillColor(sf::Color(150, 200, 255));
        usernameLabel.setPosition(178.f, 183.f);
        window.draw(usernameLabel);

        // ---- USERNAME BOX ----
        sf::RectangleShape usernameBox(sf::Vector2f(240.f, 36.f));
        if (typingUsername)
        {
            usernameBox.setFillColor(sf::Color(10, 25, 70));
            usernameBox.setOutlineColor(sf::Color(80, 180, 255));
            usernameBox.setOutlineThickness(2.f);
        }
        else
        {
            usernameBox.setFillColor(sf::Color(8, 18, 50));
            usernameBox.setOutlineColor(sf::Color(40, 80, 140));
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
            usernameContent.setFillColor(sf::Color(60, 90, 130));
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
        passwordLabel.setFillColor(sf::Color(150, 200, 255));
        passwordLabel.setPosition(178.f, 255.f);
        window.draw(passwordLabel);

        // ---- PASSWORD BOX ----
        sf::RectangleShape passwordBox(sf::Vector2f(240.f, 36.f));
        if (typingPassword)
        {
            passwordBox.setFillColor(sf::Color(10, 25, 70));
            passwordBox.setOutlineColor(sf::Color(80, 180, 255));
            passwordBox.setOutlineThickness(2.f);
        }
        else
        {
            passwordBox.setFillColor(sf::Color(8, 18, 50));
            passwordBox.setOutlineColor(sf::Color(40, 80, 140));
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
            passwordContent.setFillColor(sf::Color(60, 90, 130));
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
        hint.setFillColor(sf::Color(60, 100, 150));
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
            mainButton.setFillColor(sf::Color(20, 100, 200));
            mainButton.setOutlineColor(sf::Color(120, 210, 255));
            mainButton.setOutlineThickness(2.f);
        }
        else
        {
            mainButton.setFillColor(sf::Color(10, 60, 140));
            mainButton.setOutlineColor(sf::Color(60, 140, 220));
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
            switchButton.setFillColor(sf::Color(8, 25, 60));
            switchButton.setOutlineColor(sf::Color(60, 120, 200));
            switchButton.setOutlineThickness(1.5f);
        }
        else
        {
            switchButton.setFillColor(sf::Color(5, 15, 40));
            switchButton.setOutlineColor(sf::Color(30, 70, 130));
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
        switchText.setFillColor(sf::Color(100, 170, 230));

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