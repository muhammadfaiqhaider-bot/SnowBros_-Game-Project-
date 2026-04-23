#pragma once
#include <string>
#include <fstream>      // For file handling
#include <sstream>      // For parsing file lines

struct User                                             // Created this separate Structure that has some attributes to hold for the creation
{                                                       // of user.............
    int user_id;
    std::string username;
    std::string password_hash;
    std::string email;
};

struct PlayerProgress                                   // Structure for saving player progress
{
    int user_id;
    int current_level;
    int lives_remaining;
    int gem_count;
    int high_score;
};

class DatabaseManager
{
private:
    static const int MAX_USERS = 100;                   // Static is used otherwise Syntax error / or we can use DMA as well.......
    static const int MAX_PROGRESS = 100;

    User users[MAX_USERS];
    PlayerProgress progress[MAX_PROGRESS];

    int userCount;
    int progressCount;

    // File names
    std::string usersFile;
    std::string progressFile;
    std::string leaderboardFile;

public:
    DatabaseManager()
    {
        userCount = 0;
        progressCount = 0;

        usersFile = "users.txt";
        progressFile = "progress.txt";
        leaderboardFile = "leaderboard.txt";

        // Load existing data from files on startup
        loadUsersFromFile();
        loadProgressFromFile();
    }

    // ==================
    // USER OPERATIONS
    // ==================

    // ADD USER (REGISTER)
    bool addUser(std::string username, std::string passwordHash, std::string email)
    {
        if (userCount >= MAX_USERS)                          // Check for the max user limits means the number of accounts can be formed that is 100
        {
            return false;
        }

        for (int i = 0; i < userCount; i++)
        {
            if (users[i].username == username)               // Check for the User already exists
            {
                return false;                               // Already taken
            }
        }

        users[userCount].user_id = userCount + 1;
        users[userCount].username = username;
        users[userCount].password_hash = passwordHash;
        users[userCount].email = email;

        userCount++;

        // Save to file immediately after adding
        saveUsersToFile();

        return true;
    }

    // GET USER BY USERNAME
    User* getUser(std::string username)
    {
        for (int i = 0; i < userCount; i++)
        {
            if (users[i].username == username)
            {
                return &users[i];
            }
        }

        return nullptr;
    }

    // CHECK IF USER EXISTS
    bool userExists(std::string username)
    {
        for (int i = 0; i < userCount; i++)
        {
            if (users[i].username == username)
            {
                return true;
            }
        }

        return false;
    }

    // =====================
    // PROGRESS OPERATIONS
    // =====================

    // SAVE PLAYER PROGRESS
    bool saveProgress(int userId, int level, int lives, int gems, int highScore)
    {
        // Check if progress already exists for this user
        for (int i = 0; i < progressCount; i++)
        {
            if (progress[i].user_id == userId)
            {
                // Update existing progress
                progress[i].current_level = level;
                progress[i].lives_remaining = lives;
                progress[i].gem_count = gems;
                progress[i].high_score = highScore;

                // Save to file
                saveProgressToFile();

                return true;
            }
        }

        // New progress entry
        if (progressCount >= MAX_PROGRESS)
        {
            return false;
        }

        progress[progressCount].user_id = userId;
        progress[progressCount].current_level = level;
        progress[progressCount].lives_remaining = lives;
        progress[progressCount].gem_count = gems;
        progress[progressCount].high_score = highScore;

        progressCount++;

        // Save to file
        saveProgressToFile();

        return true;
    }

    // LOAD PLAYER PROGRESS
    PlayerProgress* loadProgress(int userId)
    {
        for (int i = 0; i < progressCount; i++)
        {
            if (progress[i].user_id == userId)
            {
                return &progress[i];
            }
        }

        return nullptr;     // No progress found for this user
    }

    // ====================
    // LEADERBOARD (LATER)
    // ====================

    void saveLeaderboard(int userId, int score, int levelReached)
    {
        // File handling - implement later
    }

    void loadLeaderboard()
    {
        // File handling - implement later
    }

    // Getter
    int getUserCount()
    {
        return userCount;
    }

private:

    // ========================
    // FILE HANDLING FUNCTIONS
    // ========================

    // SAVE ALL USERS TO FILE
    void saveUsersToFile()
    {
        std::ofstream file(usersFile);

        if (!file.is_open())
        {
            return;     // Could not open file
        }

        for (int i = 0; i < userCount; i++)
        {
            // Format: user_id,username,password_hash,email
            file << users[i].user_id << ","
                << users[i].username << ","
                << users[i].password_hash << ","
                << users[i].email << "\n";
        }

        file.close();
    }

    // LOAD ALL USERS FROM FILE
    void loadUsersFromFile()
    {
        std::ifstream file(usersFile);

        if (!file.is_open())
        {
            return;     // File doesnt exist yet - first run
        }

        std::string line;

        while (std::getline(file, line) && userCount < MAX_USERS)
        {
            std::stringstream ss(line);
            std::string token;

            // Parse: user_id,username,password_hash,email
            std::getline(ss, token, ',');
            users[userCount].user_id = std::stoi(token);

            std::getline(ss, token, ',');
            users[userCount].username = token;

            std::getline(ss, token, ',');
            users[userCount].password_hash = token;

            std::getline(ss, token, ',');
            users[userCount].email = token;

            userCount++;
        }

        file.close();
    }

    // SAVE ALL PROGRESS TO FILE
    void saveProgressToFile()
    {
        std::ofstream file(progressFile);

        if (!file.is_open())
        {
            return;
        }

        for (int i = 0; i < progressCount; i++)
        {
            // Format: user_id,current_level,lives_remaining,gem_count,high_score
            file << progress[i].user_id << ","
                << progress[i].current_level << ","
                << progress[i].lives_remaining << ","
                << progress[i].gem_count << ","
                << progress[i].high_score << "\n";
        }

        file.close();
    }

    // LOAD ALL PROGRESS FROM FILE
    void loadProgressFromFile()
    {
        std::ifstream file(progressFile);

        if (!file.is_open())
        {
            return;     // File doesnt exist yet - first run
        }

        std::string line;

        while (std::getline(file, line) && progressCount < MAX_PROGRESS)
        {
            std::stringstream ss(line);
            std::string token;

            // Parse: user_id,current_level,lives_remaining,gem_count,high_score
            std::getline(ss, token, ',');
            progress[progressCount].user_id = std::stoi(token);

            std::getline(ss, token, ',');
            progress[progressCount].current_level = std::stoi(token);

            std::getline(ss, token, ',');
            progress[progressCount].lives_remaining = std::stoi(token);

            std::getline(ss, token, ',');
            progress[progressCount].gem_count = std::stoi(token);

            std::getline(ss, token, ',');
            progress[progressCount].high_score = std::stoi(token);

            progressCount++;
        }

        file.close();
    }
};