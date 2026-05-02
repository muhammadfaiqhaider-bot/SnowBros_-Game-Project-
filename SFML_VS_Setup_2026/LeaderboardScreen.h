#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>

// Structure for one leaderboard entry
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

        // Initialize all entries
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

    void saveScore(std::string playerName, int score, int levelReached)
    {
        // Check if this player already has an entry
        for (int i = 0; i < entryCount; i++)
        {
            if (entries[i].playerName == playerName)
            {
                // Update if new score is higher
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

        // New entry
        if (entryCount < MAX_ENTRIES)
        {
            entries[entryCount].playerName = playerName;
            entries[entryCount].score = score;
            entries[entryCount].levelReached = levelReached;
            entries[entryCount].date = "2025";
            entryCount++;
        }
        else
        {
            // Replace lowest score if new score is higher
            if (score > entries[MAX_ENTRIES - 1].score)
            {
                entries[MAX_ENTRIES - 1].playerName = playerName;
                entries[MAX_ENTRIES - 1].score = score;
                entries[MAX_ENTRIES - 1].levelReached = levelReached;
                entries[MAX_ENTRIES - 1].date = "2025";
            }
        }

        sortEntries();
        saveToFile();
    }

    // ==========================================
    // HANDLE EVENTS
    // Returns:
    // 1 = go back to main menu
    // 8 = stay on leaderboard
    // ==========================================

    int handleEvents(sf::Event& event)
    {
        // Mouse hover
        if (event.type == sf::Event::MouseMoved)
        {
            float mouseX = event.mouseMove.x;
            float mouseY = event.mouseMove.y;

            backHovered = (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 535 && mouseY <= 575);
        }

        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed)
        {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            // Back button
            if (mouseX >= 200 && mouseX <= 400 &&
                mouseY >= 535 && mouseY <= 575)
            {
                return 1;   // Back to main menu
            }
        }

        // ESC key
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                return 1;   // Back to main menu
            }
        }

        return 8;   // Stay on leaderboard
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

        // ---- TITLE ----
        sf::Text title;
        title.setFont(titleFont);
        title.setString("LEADERBOARD");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color::Yellow);
        title.setPosition(150.f, 20.f);
        window.draw(title);

        // ---- TITLE UNDERLINE ----
        sf::RectangleShape underline(sf::Vector2f(500.f, 2.f));
        underline.setFillColor(sf::Color::Yellow);
        underline.setPosition(50.f, 72.f);
        window.draw(underline);

        // ---- COLUMN HEADERS ----
        sf::Text rankHeader;
        rankHeader.setFont(font);
        rankHeader.setString("RANK");
        rankHeader.setCharacterSize(14);
        rankHeader.setFillColor(sf::Color::Cyan);
        rankHeader.setPosition(80.f, 85.f);
        window.draw(rankHeader);

        sf::Text nameHeader;
        nameHeader.setFont(font);
        nameHeader.setString("PLAYER");
        nameHeader.setCharacterSize(14);
        nameHeader.setFillColor(sf::Color::Cyan);
        nameHeader.setPosition(220.f, 85.f);
        window.draw(nameHeader);

        sf::Text scoreHeader;
        scoreHeader.setFont(font);
        scoreHeader.setString("SCORE");
        scoreHeader.setCharacterSize(14);
        scoreHeader.setFillColor(sf::Color::Cyan);
        scoreHeader.setPosition(350.f, 85.f);
        window.draw(scoreHeader);

        sf::Text levelHeader;
        levelHeader.setFont(font);
        levelHeader.setString("LEVEL");
        levelHeader.setCharacterSize(14);
        levelHeader.setFillColor(sf::Color::Cyan);
        levelHeader.setPosition(480.f, 85.f);
        window.draw(levelHeader);

        // Header underline
        sf::RectangleShape headerLine(sf::Vector2f(500.f, 1.f));
        headerLine.setFillColor(sf::Color(60, 60, 120));
        headerLine.setPosition(50.f, 108.f);
        window.draw(headerLine);

        // ---- ENTRIES ----
        if (entryCount == 0)
        {
            // No entries yet
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

                // Alternate row background
                if (i % 2 == 0)
                {
                    sf::RectangleShape rowBg(sf::Vector2f(500.f, 38.f));
                    rowBg.setFillColor(sf::Color(20, 20, 60));
                    rowBg.setPosition(50.f, rowY);
                    window.draw(rowBg);
                }

                // Top 3 get special colors
                sf::Color rankColor = sf::Color::White;
                if (i == 0) rankColor = sf::Color::Yellow;       // Gold
                if (i == 1) rankColor = sf::Color(200, 200, 200); // Silver
                if (i == 2) rankColor = sf::Color(180, 100, 40);  // Bronze

                // Rank number
                sf::Text rankText;
                rankText.setFont(font);
                rankText.setString("#" + std::to_string(i + 1));
                rankText.setCharacterSize(16);
                rankText.setFillColor(rankColor);
                rankText.setPosition(80.f, rowY + 8.f);
                window.draw(rankText);

                // Crown for rank 1
                if (i == 0)
                {
                    sf::CircleShape crown(8.f, 3);
                    crown.setFillColor(sf::Color::Yellow);
                    crown.setPosition(55.f, rowY + 2.f);
                    window.draw(crown);
                }

                // Player name
                sf::Text nameText;
                nameText.setFont(font);
                std::string displayName = entries[i].playerName;

                // Truncate long names
                if (displayName.length() > 12)
                {
                    displayName = displayName.substr(0, 12) + "..";
                }

                nameText.setString(displayName);
                nameText.setCharacterSize(15);
                nameText.setFillColor(rankColor);
                nameText.setPosition(220.f, rowY + 8.f);
                window.draw(nameText);

                // Score
                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setString(std::to_string(entries[i].score));
                scoreText.setCharacterSize(15);
                scoreText.setFillColor(rankColor);
                scoreText.setPosition(350.f, rowY + 8.f);
                window.draw(scoreText);

                // Level reached
                sf::Text levelText;
                levelText.setFont(font);
                levelText.setString(std::to_string(entries[i].levelReached) + " / 10");
                levelText.setCharacterSize(15);
                levelText.setFillColor(rankColor);
                levelText.setPosition(480.f, rowY + 8.f);
                window.draw(levelText);
            }
        }

        // ---- BACK BUTTON ----
        sf::RectangleShape backBtn(sf::Vector2f(200.f, 40.f));
        if (backHovered)
        {
            backBtn.setFillColor(sf::Color(0, 100, 180));
            backBtn.setOutlineColor(sf::Color::Cyan);
            backBtn.setOutlineThickness(2.f);
        }
        else
        {
            backBtn.setFillColor(sf::Color(0, 50, 100));
            backBtn.setOutlineColor(sf::Color(0, 100, 180));
            backBtn.setOutlineThickness(1.f);
        }
        backBtn.setPosition(200.f, 535.f);
        window.draw(backBtn);

        sf::Text backText;
        backText.setFont(font);
        backText.setString("MAIN MENU");
        backText.setCharacterSize(16);
        backText.setFillColor(sf::Color::White);
        backText.setPosition(240.f, 547.f);
        window.draw(backText);

        // ESC hint
        sf::Text escHint;
        escHint.setFont(font);
        escHint.setString("Press ESC to go back");
        escHint.setCharacterSize(11);
        escHint.setFillColor(sf::Color(80, 80, 120));
        escHint.setPosition(215.f, 580.f);
        window.draw(escHint);
    }

private:

    // ==========================================
    // SORT ENTRIES BY SCORE (highest first)
    // ==========================================

    void sortEntries()
    {
        // Simple bubble sort - no vectors needed
        for (int i = 0; i < entryCount - 1; i++)
        {
            for (int j = 0; j < entryCount - i - 1; j++)
            {
                if (entries[j].score < entries[j + 1].score)
                {
                    // Swap
                    LeaderboardEntry temp = entries[j];
                    entries[j] = entries[j + 1];
                    entries[j + 1] = temp;
                }
            }
        }
    }

    // ==========================================
    // SAVE TO FILE
    // ==========================================

    void saveToFile()
    {
        std::ofstream file(leaderboardFile);

        if (!file.is_open())
        {
            return;
        }

        for (int i = 0; i < entryCount; i++)
        {
            // Format: name,score,level,date
            file << entries[i].playerName << ","
                << entries[i].score << ","
                << entries[i].levelReached << ","
                << entries[i].date << "\n";
        }

        file.close();
    }

    // ==========================================
    // LOAD FROM FILE
    // ==========================================

    void loadFromFile()
    {
        std::ifstream file(leaderboardFile);

        if (!file.is_open())
        {
            return;     // File doesn't exist yet - first run
        }

        std::string line;
        entryCount = 0;

        while (std::getline(file, line) && entryCount < MAX_ENTRIES)
        {
            std::stringstream ss(line);
            std::string token;

            // Parse: name,score,level,date
            std::getline(ss, token, ',');
            entries[entryCount].playerName = token;

            std::getline(ss, token, ',');
            entries[entryCount].score = std::stoi(token);

            std::getline(ss, token, ',');
            entries[entryCount].levelReached = std::stoi(token);

            std::getline(ss, token, ',');
            entries[entryCount].date = token;

            entryCount++;
        }

        file.close();
    }
};