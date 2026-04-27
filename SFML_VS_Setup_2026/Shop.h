#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Player.h"

struct ShopItem
{
    std::string name;
    std::string description;
    int cost;
    bool hovered;
};

class Shop
{
private:
    sf::Font font;
    int coins;

    static const int ITEM_COUNT = 5;
    ShopItem items[ITEM_COUNT];

    std::string feedbackMessage;
    int feedbackTimer;

public:
    Shop()
    {
        font.loadFromFile("assets/Text.ttf");
        coins = 0;
        feedbackMessage = "";
        feedbackTimer = 0;

        items[0] = { "Speed Boost",       "Move 50% faster for 15s",   40,  false };
        items[1] = { "Power Snowball",     "One-hit encase enemies",     60,  false };
        items[2] = { "Long Throw",         "Double snowball range",      35,  false };
        items[3] = { "Balloon Mode",       "Float for 10 seconds",       50,  false };
        items[4] = { "Extra Life",         "+1 Life",                    80,  false };
    }

    // ---- COIN MANAGEMENT ----
    void addCoins(int amount)
    {
        coins += amount;
    }

    int getCoins()
    {
        return coins;
    }

    // Called when enemy killed
    void onEnemyKilled()
    {
        coins += 5;
    }

    // Called when level complete - bonus based on time
    void onLevelComplete(int secondsTaken)
    {
        if (secondsTaken < 60)
        {
            coins += 50;
            feedbackMessage = "+50 Speed Bonus!";
            feedbackTimer = 120;
        }
    }

    // ==========================================
    // HANDLE EVENTS
    // Returns:
    // 4  = stay in shop (back to pause)
    // ==========================================
    int handleEvents(sf::Event& event, Player& player)
    {
        if (event.type == sf::Event::MouseMoved)
        {
            float mx = event.mouseMove.x;
            float my = event.mouseMove.y;

            for (int i = 0; i < ITEM_COUNT; i++)
            {
                float btnY = 160.f + i * 70.f;
                items[i].hovered = (mx >= 150 && mx <= 450 &&
                    my >= btnY && my <= btnY + 55.f);
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mx = event.mouseButton.x;
            float my = event.mouseButton.y;

            for (int i = 0; i < ITEM_COUNT; i++)
            {
                float btnY = 160.f + i * 70.f;

                if (mx >= 150 && mx <= 450 &&
                    my >= btnY && my <= btnY + 55.f)
                {
                    tryBuy(i, player);
                }
            }

            // Back button
            if (mx >= 200 && mx <= 400 &&
                my >= 535 && my <= 575)
            {
                return 4;   // Back to pause
            }
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                return 4;   // Back to pause
            }
        }

        return 9;   // Stay in shop
    }

    // ==========================================
    // DRAW
    // ==========================================
    void draw(sf::RenderWindow& window)
    {
        // Background
        sf::RectangleShape bg(sf::Vector2f(600.f, 600.f));
        bg.setFillColor(sf::Color(10, 10, 40));
        window.draw(bg);

        // Title
        sf::Text title;
        title.setFont(font);
        title.setString("SHOP");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color::Yellow);
        title.setPosition(245.f, 15.f);
        window.draw(title);

        // Coin display
        sf::Text coinText;
        coinText.setFont(font);
        coinText.setString("Coins: " + std::to_string(coins));
        coinText.setCharacterSize(20);
        coinText.setFillColor(sf::Color::Yellow);
        coinText.setPosition(20.f, 20.f);
        window.draw(coinText);

        // Underline
        sf::RectangleShape line(sf::Vector2f(500.f, 2.f));
        line.setFillColor(sf::Color::Yellow);
        line.setPosition(50.f, 65.f);
        window.draw(line);

        // Items
        for (int i = 0; i < ITEM_COUNT; i++)
        {
            float btnY = 160.f + i * 70.f;
            drawItem(window, items[i], btnY);
        }

        // Back button
        sf::RectangleShape backBtn(sf::Vector2f(200.f, 40.f));
        backBtn.setFillColor(sf::Color(80, 0, 0));
        backBtn.setOutlineColor(sf::Color::Red);
        backBtn.setOutlineThickness(1.f);
        backBtn.setPosition(200.f, 535.f);
        window.draw(backBtn);

        sf::Text backText;
        backText.setFont(font);
        backText.setString("BACK  (ESC)");
        backText.setCharacterSize(16);
        backText.setFillColor(sf::Color::White);
        backText.setPosition(240.f, 545.f);
        window.draw(backText);

        // Feedback message
        if (feedbackTimer > 0)
        {
            feedbackTimer--;
            sf::Text fb;
            fb.setFont(font);
            fb.setString(feedbackMessage);
            fb.setCharacterSize(16);
            fb.setFillColor(sf::Color::Green);
            fb.setPosition(200.f, 75.f);
            window.draw(fb);
        }
    }

private:

    void tryBuy(int index, Player& player)
    {
        if (coins < items[index].cost)
        {
            feedbackMessage = "Not enough coins!";
            feedbackTimer = 90;
            return;
        }

        coins -= items[index].cost;

        switch (index)
        {
        case 0: player.applySpeedBoost();       break;
        case 1: player.applySnowballPower();    break;
        case 2: player.applyDistanceIncrease(); break;
        case 3: player.applyBalloonMode();      break;
        case 4: player.addLife();               break;
        }

        feedbackMessage = items[index].name + " purchased!";
        feedbackTimer = 90;
    }

    void drawItem(sf::RenderWindow& window, ShopItem& item, float y)
    {
        sf::RectangleShape btn(sf::Vector2f(300.f, 55.f));
        btn.setPosition(150.f, y);

        if (item.hovered)
        {
            btn.setFillColor(sf::Color(30, 30, 80));
            btn.setOutlineColor(sf::Color::Cyan);
            btn.setOutlineThickness(2.f);
        }
        else
        {
            btn.setFillColor(sf::Color(20, 20, 60));
            btn.setOutlineColor(sf::Color(60, 60, 120));
            btn.setOutlineThickness(1.f);
        }
        window.draw(btn);

        // Item name
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(item.name);
        nameText.setCharacterSize(16);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(160.f, y + 5.f);
        window.draw(nameText);

        // Description
        sf::Text descText;
        descText.setFont(font);
        descText.setString(item.description);
        descText.setCharacterSize(12);
        descText.setFillColor(sf::Color(180, 180, 180));
        descText.setPosition(160.f, y + 28.f);
        window.draw(descText);

        // Cost
        sf::Text costText;
        costText.setFont(font);
        costText.setString(std::to_string(item.cost) + "c");
        costText.setCharacterSize(16);
        costText.setFillColor(sf::Color::Yellow);
        costText.setPosition(390.f, y + 18.f);
        window.draw(costText);
    }
};