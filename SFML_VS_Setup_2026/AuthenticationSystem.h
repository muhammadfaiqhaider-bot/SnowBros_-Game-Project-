#pragma once
#include "PasswordHidding.h"
#include "DataBase.h"
#include <string>



// Purpose of this class is to check the authtication of the user 
// runs in Login phase .......
class AuthManager
{
private:
    bool isLogin;                       
    std::string currusername;
    int currentUserId;                  
    PasswordHidding passwordHidding;    // For hashing passwords
    DatabaseManager* dbManager;         // For saving/loading users

public:
    AuthManager(DatabaseManager* db)  // Constructor
    {
        isLogin = false;
        currusername = "";
        currentUserId = -1;
        dbManager = db;
    }

    // Registering new user.......
    bool registerUser(std::string username, std::string password, std::string email)
    {
    // Chek if user of this name exsist ot nott function available in database class......
        if (dbManager->userExists(username))
        {
            return false;               // Username already taken
        }

     // when acount is creating the password must be hashed before it is saved......for protection..
        std::string hashedPassword = passwordHidding.hashingPassword(password);

     // now pass these all info to the database so that my data get saved....and all user data is protected......
        bool success = dbManager->addUser(username, hashedPassword, email);

        return success;
    }

    // Loggin Phase only for those who hav signed up already 
    bool loginUser(std::string username, std::string password)
    {
        
        User* user = dbManager->getUser(username);

        if (user == nullptr)
        {
            return false;             // Check bcz if usr doesn't exsit so return at onces...
        }

        // Verify password
        bool passwordCorrect = passwordHidding.verification(password, user->password_hash);

        if (passwordCorrect)
        {
            isLogin = true;
            currusername = username;
            currentUserId = user->user_id;
            return true;                // Login successful
        }
        else
        {
            return false;               // Wrong password
        }
    }

    


    // Logout phasee 
    void logoutUser()
    {
        isLogin = false;
        currusername = "";
        currentUserId = -1;
    }




    // Getters
    bool getIsLoggedIn()
    {
        return isLogin;
    }

    std::string getCurrentUsername()
    {
        return currusername;
    }

    int getCurrentUserId()
    {
        return currentUserId;
    }
};