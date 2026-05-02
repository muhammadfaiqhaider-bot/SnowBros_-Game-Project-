#pragma once
#include <string>
#include <iostream>
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

    int userCont;
    int progressCount;

    // File names
    std::string usersFile;
    std::string progressFile;
    std::string leaderboardFile;

public:
    DatabaseManager()            // Data base Manager that has following role 
    {                            // 1- take user data and store it 
                                 // 2- keep track of progress and rank them to make leaderboard.........

        userCont = 0;
        progressCount = 0;

        usersFile = "users.txt";
        progressFile = "progress.txt";
        leaderboardFile = "leaderboard.txt";


        loadUsersFromFile();    // Load existing data from files on start of the gamee......
        loadProgressFromFile();
    }


    // THis function is use to create an  account for the user assign there username
    // Set password and save to the file......
    bool addUser(std::string username, std::string passwordHash, std::string email)
    {
        if (userCont >= MAX_USERS)                          // Check for the max user limits means the number of accounts can be formed that is 100
        {
            return false;
        }

        for (int i = 0; i < userCont; i++)
        {
            if (users[i].username == username)               // Check for the User already exists
            {
                return false;                               // Already taken
            }
        }

        users[userCont].user_id = userCont + 1;             // Incremnet upon user added
        users[userCont].username = username;
        users[userCont].password_hash = passwordHash;
        users[userCont].email = email;

        userCont++;
        saveUsersToFile();       // New Account created now add this data into file.....

        return true;
    }












    // This is getter for whole user and we retrive user by user name..........
    User* getUser(std::string username)   /// We'll pass user name in parameter and it search for similar name 
    {                                      // and get ser data of that....
        for (int i = 0; i < userCont; i++)
        {
            if (users[i].username == username)
            {
                return &users[i];
            }
        }
        return nullptr;                // If that user isn't present soo null pointer will resturn .....
    }












    // Chek if user exsist or not........
    bool userExists(std::string username)
    {
        for (int i = 0; i < userCont; i++)
        {
            if (users[i].username == username)
            {
                return true;
            }
        }

        return false;
    }



    // SAVE player / user progresss if he left game in between or some thing.....
    // when he comeback he have save data and he should continue.......
    bool saveProgress(int Id, int lvl, int lives, int gems, int hiScore)
    {
        // Check if progress already exists for this user
        for (int i = 0; i < progressCount; i++)
        {
            if (progress[i].user_id == Id)
            {
                // Update existing progress
                progress[i].current_level = lvl;
                progress[i].lives_remaining = lives;
                progress[i].gem_count = gems;
                progress[i].high_score = hiScore;

                // Save to file
                saveProgressToFile();

                return true;
            }
        }


        if (progressCount >= MAX_PROGRESS)
        {
            return false;
        }

        progress[progressCount].user_id = Id;
        progress[progressCount].current_level = lvl;
        progress[progressCount].lives_remaining = lives;
        progress[progressCount].gem_count = gems;
        progress[progressCount].high_score = hiScore;

        progressCount++;

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







    void saveLeaderboard(int userId, int score, int levelReached)
    {
        // NILL not implemented yet
    }

    void loadLeaderboard()
    {
        // NILL not implemented yet 
    }




    // Getters
    int getUserCount()
    {
        return userCont;
    }
    int getprogresscount()
    {
        return progressCount;
    }




















private:


    // FILE HANDLING STUFF IN PRIAVTE
    // SAVE ALL USERS TO FILE
    void saveUsersToFile()
    {
        std::ofstream file(usersFile);

        if (!file.is_open())
        {
            std::cout << "Sorry BRAT there is some error while opening THIS SHYT....... /n";
            return;
        }

        for (int i = 0; i < userCont; i++)
        {
            file << users[i].user_id << ","       // Save these info's in file..
                << users[i].username << ","
                << users[i].password_hash << ","
                << users[i].email << "\n";
        }

        file.close();
    }

    // Retrive all the save stuff from the file..
    void loadUsersFromFile()
    {
        std::ifstream file(usersFile);

        if (!file.is_open())
        {
            std::cout << "File doesnot exsist first create it N";
            return;
        }

        std::string line;

        while (std::getline(file, line) && userCont < MAX_USERS)
        {
            std::stringstream ss(line);
            std::string token;

            // Parse: user_id,username,password_hash,email
            std::getline(ss, token, ',');
            users[userCont].user_id = std::stoi(token);

            std::getline(ss, token, ',');
            users[userCont].username = token;

            std::getline(ss, token, ',');
            users[userCont].password_hash = token;

            std::getline(ss, token, ',');
            users[userCont].email = token;

            userCont++;
        }

        file.close();
    }





    // File handling Stuff to save all the progrss in filee
    void saveProgressToFile()
    {
        std::ofstream file(progressFile);

        if (!file.is_open())
        {
            std::cout << "Progress Is not saved... there is some error(error 404)/n";
            return;
        }

        for (int i = 0; i < progressCount; i++)
        {
            // 
            file << progress[i].user_id << ","
                << progress[i].current_level << ","
                << progress[i].lives_remaining << ","
                << progress[i].gem_count << ","
                << progress[i].high_score << "\n";
        }

        file.close();
    }



    void loadProgressFromFile()
    {
        std::ifstream file(progressFile);

        if (!file.is_open())
        {
            return;
        }

        std::string line;

        while (std::getline(file, line) && progressCount < MAX_PROGRESS)
        {
            std::stringstream ss(line);
            std::string token;

            // Parse: user_id,current_level,lives_remaining,gem_count,high_score
            std::getline(ss, token, ',');
            progress[progressCount].user_id = std::stoi(token); // stoi is inbuild function for string to int conversion

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