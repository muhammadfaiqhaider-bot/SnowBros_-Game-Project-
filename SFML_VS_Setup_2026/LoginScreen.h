#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "AuthenticationSystem.h"

class LoginScreen
{
private:
    sf::Font font;

    // Input fields
    std::string usernameInput;
    std::string passwordInput;

    // Which box is active
    bool typingUsername;
    bool typingPassword;

    // Message to show (error or success)
    std::string message;
    sf::Color messageColor;

public:
    LoginScreen()
    {
        font.loadFromFile("Orbitron-VariableFont_wght.ttf");

        usernameInput = "";
        passwordInput = "";
        typingUsername = false;
        typingPassword = false;
        message = "";
        messageColor = sf::Color::Red;
    }

    // ==========================================
    // HANDLE EVENTS
    // Returns:
    // 0 = stay on login
    // 1 = go to main menu
    // ==========================================

    int handleEvents(sf::Event& event, AuthManager& auth)
    {
        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            // Username box clicked (x:150-450, y:210-245)
            if (mouseX >= 150 && mouseX <= 450 &&
                mouseY >= 210 && mouseY <= 245)
            {
                typingUsername = true;
                typingPassword = false;
            }

            // Password box clicked (x:150-450, y:300-335)
            else if (mouseX >= 150 && mouseX <= 450 &&
                mouseY >= 300 && mouseY <= 335)
            {
                typingPassword = true;
                typingUsername = false;
            }

            // Login button clicked (x:150-270, y:370-410)
            // Login button clicked - FIXED coordinates
            else if (mouseX >= 155 && mouseX <= 265 &&
                mouseY >= 340 && mouseY <= 380)
            {
                if (usernameInput == "" || passwordInput == "")
                {
                    message = "Please fill all fields!";
                    messageColor = sf::Color::Red;
                }
                else if (auth.loginUser(usernameInput, passwordInput))
                {
                    message = "Login Successful!";
                    messageColor = sf::Color::Green;
                    return 1;       // Go to main menu
                }
                else
                {
                    message = "Wrong username or password!";
                    messageColor = sf::Color::Red;
                }
            }

            // Register button clicked - FIXED coordinates
            else if (mouseX >= 335 && mouseX <= 445 &&
                mouseY >= 340 && mouseY <= 380)
            {
                if (usernameInput == "" || passwordInput == "")
                {
                    message = "Please fill all fields!";
                    messageColor = sf::Color::Red;
                }
                else if (auth.registerUser(usernameInput, passwordInput, ""))
                {
                    message = "Registered! Now login.";
                    messageColor = sf::Color::Green;
                }
                else
                {
                    message = "Username already taken!";
                    messageColor = sf::Color::Red;
                }
            }

            // Clicked elsewhere - deselect boxes
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
            // Normal character (space to ~)
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

        return 0;       // Stay on login screen
    }

    // ==========================================
    // DRAW
    // ==========================================

    void draw(sf::RenderWindow& window)
    {
        // ---- BACKGROUND ----
        sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
        background.setFillColor(sf::Color(10, 10, 40));     // Very dark blue
        window.draw(background);

        // ---- SNOW EFFECT (simple dots) ----
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
        title.setString("SNOW BROS");
        title.setCharacterSize(55);
        title.setFillColor(sf::Color::Cyan);
        title.setPosition(120.f, 50.f);
        window.draw(title);

        // ---- SUBTITLE ----
        sf::Text subtitle;
        subtitle.setFont(font);
        subtitle.setString("Please login to continue");
        subtitle.setCharacterSize(16);
        subtitle.setFillColor(sf::Color(150, 150, 200));
        subtitle.setPosition(175.f, 130.f);
        window.draw(subtitle);

        // ---- LOGIN BOX ----
        sf::RectangleShape loginBox(sf::Vector2f(320.f, 320.f));
        loginBox.setFillColor(sf::Color(20, 20, 60));
        loginBox.setOutlineColor(sf::Color(100, 100, 200));
        loginBox.setOutlineThickness(2.f);
        loginBox.setPosition(140.f, 160.f);
        window.draw(loginBox);

        // ---- USERNAME LABEL ----
        sf::Text usernameLabel;
        usernameLabel.setFont(font);
        usernameLabel.setString("Username");
        usernameLabel.setCharacterSize(16);
        usernameLabel.setFillColor(sf::Color(200, 200, 255));
        usernameLabel.setPosition(155.f, 178.f);
        window.draw(usernameLabel);

        // ---- USERNAME BOX ----
        sf::RectangleShape usernameBox(sf::Vector2f(290.f, 35.f));
        if (typingUsername)
        {
            usernameBox.setFillColor(sf::Color(40, 40, 90));
            usernameBox.setOutlineColor(sf::Color::Cyan);   // Cyan outline when active
        }
        else
        {
            usernameBox.setFillColor(sf::Color(30, 30, 70));
            usernameBox.setOutlineColor(sf::Color(80, 80, 150));
        }
        usernameBox.setOutlineThickness(1.5f);
        usernameBox.setPosition(155.f, 200.f);
        window.draw(usernameBox);

        // ---- USERNAME TEXT ----
        sf::Text usernameText;
        usernameText.setFont(font);
        usernameText.setString(usernameInput);
        usernameText.setCharacterSize(16);
        usernameText.setFillColor(sf::Color::White);
        usernameText.setPosition(163.f, 208.f);
        window.draw(usernameText);

        // ---- PASSWORD LABEL ----
        sf::Text passwordLabel;
        passwordLabel.setFont(font);
        passwordLabel.setString("Password");
        passwordLabel.setCharacterSize(16);
        passwordLabel.setFillColor(sf::Color(200, 200, 255));
        passwordLabel.setPosition(155.f, 255.f);
        window.draw(passwordLabel);

        // ---- PASSWORD BOX ----
        sf::RectangleShape passwordBox(sf::Vector2f(290.f, 35.f));
        if (typingPassword)
        {
            passwordBox.setFillColor(sf::Color(40, 40, 90));
            passwordBox.setOutlineColor(sf::Color::Cyan);
        }
        else
        {
            passwordBox.setFillColor(sf::Color(30, 30, 70));
            passwordBox.setOutlineColor(sf::Color(80, 80, 150));
        }
        passwordBox.setOutlineThickness(1.5f);
        passwordBox.setPosition(155.f, 275.f);
        window.draw(passwordBox);

        // ---- PASSWORD TEXT (show stars) ----
        std::string stars = "";
        for (int i = 0; i < passwordInput.length(); i++)
        {
            stars = stars + "*";
        }

        sf::Text passwordText;
        passwordText.setFont(font);
        passwordText.setString(stars);
        passwordText.setCharacterSize(16);
        passwordText.setFillColor(sf::Color::White);
        passwordText.setPosition(163.f, 283.f);
        window.draw(passwordText);

        // ---- LOGIN BUTTON ----
        sf::RectangleShape loginButton(sf::Vector2f(110.f, 40.f));
        loginButton.setFillColor(sf::Color(0, 130, 0));
        loginButton.setOutlineColor(sf::Color(0, 200, 0));
        loginButton.setOutlineThickness(1.f);
        loginButton.setPosition(155.f, 340.f);
        window.draw(loginButton);

        sf::Text loginBtnText;
        loginBtnText.setFont(font);
        loginBtnText.setString("LOGIN");
        loginBtnText.setCharacterSize(16);
        loginBtnText.setFillColor(sf::Color::White);
        loginBtnText.setPosition(182.f, 350.f);
        window.draw(loginBtnText);

        // ---- REGISTER BUTTON ----
        sf::RectangleShape registerButton(sf::Vector2f(110.f, 40.f));
        registerButton.setFillColor(sf::Color(0, 0, 130));
        registerButton.setOutlineColor(sf::Color(0, 0, 200));
        registerButton.setOutlineThickness(1.f);
        registerButton.setPosition(335.f, 340.f);
        window.draw(registerButton);

        sf::Text registerBtnText;
        registerBtnText.setFont(font);
        registerBtnText.setString("REGISTER");
        registerBtnText.setCharacterSize(14);
        registerBtnText.setFillColor(sf::Color::White);
        registerBtnText.setPosition(348.f, 352.f);
        window.draw(registerBtnText);

        // ---- MESSAGE ----
        sf::Text messageText;
        messageText.setFont(font);
        messageText.setString(message);
        messageText.setCharacterSize(14);
        messageText.setFillColor(messageColor);
        messageText.setPosition(155.f, 395.f);
        window.draw(messageText);

        // ---- CURSOR BLINK ----
        // Show cursor in active box
        if (typingUsername)
        {
            sf::RectangleShape cursor(sf::Vector2f(2.f, 20.f));
            cursor.setFillColor(sf::Color::White);
            cursor.setPosition(163.f + usernameInput.length() * 9.f, 210.f);
            window.draw(cursor);
        }
        if (typingPassword)
        {
            sf::RectangleShape cursor(sf::Vector2f(2.f, 20.f));
            cursor.setFillColor(sf::Color::White);
            cursor.setPosition(163.f + passwordInput.length() * 9.f, 283.f);
            window.draw(cursor);
        }
    }

    // Getters
    std::string getUsernameInput()
    {
        return usernameInput;
    }
};