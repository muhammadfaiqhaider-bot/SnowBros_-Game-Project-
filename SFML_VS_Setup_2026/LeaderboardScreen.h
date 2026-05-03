#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>

struct LeaderboardEntry
{
    std::string playerName;
    int score;
    int levelReached;
    std::string date;
};

class LeaderboardScreen
{
private:
    sf::Font font;
    sf::Font titleFont;

    static const int MAX_ENTRIES = 10;
    LeaderboardEntry entries[MAX_ENTRIES];
    int entryCount;

    bool backHovered;
    std::string leaderboardFile;

public:
    LeaderboardScreen()
    {
        font.loadFromFile("assets/Text.ttf");
        titleFont.loadFromFile("assets/Title.ttf");

        entryCount = 0;
        backHovered = false;
        leaderboardFile = "leaderboard.txt";

        for (int i = 0; i < MAX_ENTRIES; i++)
        {
            entries[i].playerName = "";
            entries[i].score = 0;
            entries[i].levelReached = 0;
            entries[i].date = "";
        }

        loadFromFile();
    }

    // ==========================================
    // SAVE NEW SCORE
    // ==========================================
    void saveScore(const std::string& playerName, int score, int levelReached)
    {
        // Ignore empty/invalid names
        if (playerName.empty()) return;

        // Update existing entry if player already on the board
        for (int i = 0; i < entryCount; i++)
        {
            if (entries[i].playerName == playerName)
            {
                if (score > entries[i].score)
                {
                    entries[i].score = score;
                    entries[i].levelReached = levelReached;
                }
                sortEntries();
                saveToFile();
                return;
            }
        }

        // New entry — add to bottom or replace lowest if board is full
        if (entryCount < MAX_ENTRIES)
        {
            entries[entryCount].playerName = playerName;
            entries[entryCount].score = score;
            entries[entryCount].levelReached = levelReached;
            entries[entryCount].date = "2025";
            entryCount++;
        }
        else if (score > entries[MAX_ENTRIES - 1].score)
        {
            entries[MAX_ENTRIES - 1].playerName = playerName;
            entries[MAX_ENTRIES - 1].score = score;
            entries[MAX_ENTRIES - 1].levelReached = levelReached;
            entries[MAX_ENTRIES - 1].date = "2025";
        }

        sortEntries();
        saveToFile();
    }

    // ==========================================
    // HANDLE EVENTS
    // 1 = back to main menu,  8 = stay
    // ==========================================
    int handleEvents(sf::Event& event)
    {
        if (event.type == sf::Event::MouseMoved)
        {
            float mx = event.mouseMove.x;
            float my = event.mouseMove.y;
            backHovered = (mx >= 200 && mx <= 400 && my >= 535 && my <= 575);
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mx = event.mouseButton.x;
            float my = event.mouseButton.y;
            if (mx >= 200 && mx <= 400 && my >= 535 && my <= 575)
                return 1;
        }

        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::Escape)
                return 1;

        return 8;
    }

    // ==========================================
    // DRAW
    // ==========================================
    void draw(sf::RenderWindow& window)
    {
        // Background
        sf::RectangleShape background(sf::Vector2f(600.f, 600.f));
        background.setFillColor(sf::Color(10, 10, 40));
        window.draw(background);

        // Title
        sf::Text title;
        title.setFont(titleFont);
        title.setString("LEADERBOARD");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color::Yellow);
        title.setPosition(150.f, 20.f);
        window.draw(title);

        sf::RectangleShape underline(sf::Vector2f(500.f, 2.f));
        underline.setFillColor(sf::Color::Yellow);
        underline.setPosition(50.f, 72.f);
        window.draw(underline);

        // Column headers
        auto makeHeader = [&](const std::string& s, float x)
            {
                sf::Text t;
                t.setFont(font);
                t.setString(s);
                t.setCharacterSize(14);
                t.setFillColor(sf::Color::Cyan);
                t.setPosition(x, 85.f);
                window.draw(t);
            };
        makeHeader("RANK", 80.f);
        makeHeader("PLAYER", 220.f);
        makeHeader("SCORE", 350.f);
        makeHeader("LEVEL", 480.f);

        sf::RectangleShape headerLine(sf::Vector2f(500.f, 1.f));
        headerLine.setFillColor(sf::Color(60, 60, 120));
        headerLine.setPosition(50.f, 108.f);
        window.draw(headerLine);

        // Entries
        if (entryCount == 0)
        {
            sf::Text noEntries;
            noEntries.setFont(font);
            noEntries.setString("No scores yet! Play the game!");
            noEntries.setCharacterSize(18);
            noEntries.setFillColor(sf::Color(150, 150, 150));
            noEntries.setPosition(130.f, 280.f);
            window.draw(noEntries);
        }
        else
        {
            for (int i = 0; i < entryCount; i++)
            {
                float rowY = 120.f + i * 40.f;

                if (i % 2 == 0)
                {
                    sf::RectangleShape rowBg(sf::Vector2f(500.f, 38.f));
                    rowBg.setFillColor(sf::Color(20, 20, 60));
                    rowBg.setPosition(50.f, rowY);
                    window.draw(rowBg);
                }

                sf::Color rankColor = sf::Color::White;
                if (i == 0) rankColor = sf::Color::Yellow;
                if (i == 1) rankColor = sf::Color(200, 200, 200);
                if (i == 2) rankColor = sf::Color(180, 100, 40);

                // Crown for #1
                if (i == 0)
                {
                    sf::CircleShape crown(8.f, 3);
                    crown.setFillColor(sf::Color::Yellow);
                    crown.setPosition(55.f, rowY + 2.f);
                    window.draw(crown);
                }

                auto makeCell = [&](const std::string& s, float x)
                    {
                        sf::Text t;
                        t.setFont(font);
                        t.setString(s);
                        t.setCharacterSize(15);
                        t.setFillColor(rankColor);
                        t.setPosition(x, rowY + 8.f);
                        window.draw(t);
                    };

                makeCell("#" + std::to_string(i + 1), 80.f);

                std::string displayName = entries[i].playerName;
                if (displayName.length() > 12)
                    displayName = displayName.substr(0, 12) + "..";
                makeCell(displayName, 220.f);

                makeCell(std::to_string(entries[i].score), 350.f);
                makeCell(std::to_string(entries[i].levelReached) + " / 10", 480.f);
            }
        }

        // Back button
        sf::RectangleShape backBtn(sf::Vector2f(200.f, 40.f));
        backBtn.setFillColor(backHovered ? sf::Color(0, 100, 180) : sf::Color(0, 50, 100));
        backBtn.setOutlineColor(backHovered ? sf::Color::Cyan : sf::Color(0, 100, 180));
        backBtn.setOutlineThickness(backHovered ? 2.f : 1.f);
        backBtn.setPosition(200.f, 535.f);
        window.draw(backBtn);

        sf::Text backText;
        backText.setFont(font);
        backText.setString("MAIN MENU");
        backText.setCharacterSize(16);
        backText.setFillColor(sf::Color::White);
        backText.setPosition(240.f, 547.f);
        window.draw(backText);

        sf::Text escHint;
        escHint.setFont(font);
        escHint.setString("Press ESC to go back");
        escHint.setCharacterSize(11);
        escHint.setFillColor(sf::Color(80, 80, 120));
        escHint.setPosition(215.f, 580.f);
        window.draw(escHint);
    }

private:

    void sortEntries()
    {
        for (int i = 0; i < entryCount - 1; i++)
            for (int j = 0; j < entryCount - i - 1; j++)
                if (entries[j].score < entries[j + 1].score)
                {
                    LeaderboardEntry tmp = entries[j];
                    entries[j] = entries[j + 1];
                    entries[j + 1] = tmp;
                }
    }

    // ==========================================
    // SAVE — writes cleanly, no trailing newline issues
    // ==========================================
    void saveToFile()
    {
        std::ofstream file(leaderboardFile, std::ios::trunc);
        if (!file.is_open()) return;

        for (int i = 0; i < entryCount; i++)
        {
            // Skip any entry that somehow got a blank name
            if (entries[i].playerName.empty()) continue;

            file << entries[i].playerName << ","
                << entries[i].score << ","
                << entries[i].levelReached << ","
                << entries[i].date << "\n";
        }
        file.close();
    }

    // ==========================================
    // LOAD — safe parsing, never crashes on bad data
    // ==========================================
    void loadFromFile()
    {
        std::ifstream file(leaderboardFile);
        if (!file.is_open()) return;   // first run, file doesn't exist yet

        std::string line;
        entryCount = 0;

        while (std::getline(file, line) && entryCount < MAX_ENTRIES)
        {
            // Skip blank lines (the main crash cause)
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string token;

            // ---- name ----
            if (!std::getline(ss, token, ',')) continue;
            std::string name = token;
            if (name.empty()) continue;   // skip nameless entries

            // ---- score ----
            if (!std::getline(ss, token, ',')) continue;
            int score = 0;
            try { score = std::stoi(token); }
            catch (...) { continue; }   // bad integer → skip whole line

            // ---- level ----
            if (!std::getline(ss, token, ',')) continue;
            int level = 0;
            try { level = std::stoi(token); }
            catch (...) { continue; }

            // ---- date (optional — don't crash if missing) ----
            std::string date = "2025";
            if (std::getline(ss, token, ',') && !token.empty())
                date = token;

            // All fields parsed cleanly — commit the entry
            entries[entryCount].playerName = name;
            entries[entryCount].score = score;
            entries[entryCount].levelReached = level;
            entries[entryCount].date = date;
            entryCount++;
        }

        file.close();
    }
};