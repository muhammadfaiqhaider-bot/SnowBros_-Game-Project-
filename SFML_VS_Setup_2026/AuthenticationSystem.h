#pragma once
#include "LogginSystem.h"
#include "DataBase.h"
#include <string>

class AuthManager
{
private:
    bool isLoggedIn;
    std::string currentUsername;
    int currentUserId;
    PasswordHidding passwordHidding;    // For hashing passwords
    DatabaseManager* dbManager;         // For saving/loading users

public:
    AuthManager(DatabaseManager* db)
    {
        isLoggedIn = false;
        currentUsername = "";
        currentUserId = -1;
        dbManager = db;
    }

    // REGISTER NEW USER
    bool registerUser(std::string username, std::string password, std::string email)
    {
        // Check if username already exists
        if (dbManager->userExists(username))
        {
            return false;               // Username already taken
        }

        // Hash the password before saving
        std::string hashedPassword = passwordHidding.hashingPassword(password);

        // Save to database
        bool success = dbManager->addUser(username, hashedPassword, email);

        return success;
    }

    // LOGIN EXISTING USER
    bool loginUser(std::string username, std::string password)
    {
        // Load user from database
        User* user = dbManager->getUser(username);

        if (user == nullptr)
        {
            return false;               // User not found
        }

        // Verify password
        bool passwordCorrect = passwordHidding.verification(password, user->password_hash);

        if (passwordCorrect)
        {
            isLoggedIn = true;
            currentUsername = username;
            currentUserId = user->user_id;
            return true;                // Login successful
        }
        else
        {
            return false;               // Wrong password
        }
    }

    // LOGOUT
    void logoutUser()
    {
        isLoggedIn = false;
        currentUsername = "";
        currentUserId = -1;
    }

    // Getters
    bool getIsLoggedIn()
    {
        return isLoggedIn;
    }

    std::string getCurrentUsername()
    {
        return currentUsername;
    }

    int getCurrentUserId()
    {
        return currentUserId;
    }
};