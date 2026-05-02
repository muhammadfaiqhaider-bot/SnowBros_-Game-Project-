#pragma once
#include <string>
#include <iostream>
#include <fstream>      // For file handling
#include <sstream>      // For parsing file lines
#include <climits>


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

    // Helper: parse a decimal integer from a validated numeric string.
    // Returns true on success, false on failure (including overflow).
    static bool parseInteger(const std::string& s, int& out)
    {
        if (s.empty()) return false;
        size_t i = 0;
        bool neg = false;
        if (s[0] == '+' || s[0] == '-') {
            if (s.size() == 1) return false;
            neg = (s[0] == '-');
            i = 1;
        }
        long long acc = 0;
        for (; i < s.size(); ++i) {
            char c = s[i];
            if (c < '0' || c > '9') return false;
            acc = acc * 10 + (c - '0');
            if (!neg && acc > INT_MAX) return false;
            if (neg && acc > (long long)INT_MAX + 1) return false;
        }
        out = neg ? -static_cast<int>(acc) : static_cast<int>(acc);
        return true;
    }
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
        // Open with truncation to ensure old/binary data is removed
        std::ofstream file(usersFile, std::ios::out | std::ios::trunc);

        if (!file.is_open())
        {
            std::cerr << "Failed to open users file for writing\n";
            return;
        }

        for (int i = 0; i < userCont; i++)
        {
            file << users[i].user_id << ","       // Save these infos in file (CSV)
                << users[i].username << ","
                << users[i].password_hash << ","
                << users[i].email << "\n";
        }

        file.close();
    }

    // Retrive all the save stuff from the file..
    void loadUsersFromFile()
    {
        std::ifstream file(usersFile, std::ios::in);

        if (!file.is_open())
        {
            // File not present -- start with empty DB
            return;
        }

        std::string line;
        int lineNo = 0;

        while (std::getline(file, line) && userCont < MAX_USERS)
        {
            ++lineNo;
            if (line.empty())
                continue;

            // parse CSV fields without using advanced libs
            size_t p = 0;
            size_t next = line.find(',', p);
            if (next == std::string::npos) {
                std::cerr << "Malformed users.txt line " << lineNo << " (missing fields)\n";
                continue;
            }

            std::string idtoken = line.substr(p, next - p);

            // validate idtoken: optional sign then digits
            const char* s = idtoken.c_str();
            if (idtoken.empty()) {
                std::cerr << "Empty id token on line " << lineNo << "\n";
                continue;
            }
            size_t si = 0;
            if (s[0] == '+' || s[0] == '-') {
                if (idtoken.size() == 1) {
                    std::cerr << "Invalid id token on line " << lineNo << "\n";
                    continue;
                }
                si = 1;
            }
            bool ok = true;
            for (; si < idtoken.size(); ++si) {
                if (s[si] < '0' || s[si] > '9') { ok = false; break; }
            }
            if (!ok) {
                std::cerr << "Non-numeric id token on line " << lineNo << "\n";
                continue;
            }

            int id = 0;
            if (!parseInteger(idtoken, id)) {
                std::cerr << "Failed to parse id on line " << lineNo << "\n";
                continue;
            }

            // username
            p = next + 1;
            next = line.find(',', p);
            if (next == std::string::npos) {
                std::cerr << "Malformed users.txt line " << lineNo << " (missing username)\n";
                continue;
            }
            std::string uname = line.substr(p, next - p);

            // password_hash
            p = next + 1;
            next = line.find(',', p);
            if (next == std::string::npos) {
                std::cerr << "Malformed users.txt line " << lineNo << " (missing password_hash)\n";
                continue;
            }
            std::string phash = line.substr(p, next - p);

            // email = rest of line
            p = next + 1;
            std::string email = line.substr(p);

            // Commit to users array
            users[userCont].user_id = id;
            users[userCont].username = uname;
            users[userCont].password_hash = phash;
            users[userCont].email = email;
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


        int lineNo = 0;
        while (std::getline(file, line) && progressCount < MAX_PROGRESS)
        {
            ++lineNo;
            if (line.empty()) continue;

            size_t p = 0;
            size_t next = line.find(',', p);
            if (next == std::string::npos) { std::cerr << "Malformed progress.txt line " << lineNo << "\n"; continue; }
            std::string tok0 = line.substr(p, next - p);

            p = next + 1; next = line.find(',', p);
            if (next == std::string::npos) { std::cerr << "Malformed progress.txt line " << lineNo << "\n"; continue; }
            std::string tok1 = line.substr(p, next - p);

            p = next + 1; next = line.find(',', p);
            if (next == std::string::npos) { std::cerr << "Malformed progress.txt line " << lineNo << "\n"; continue; }
            std::string tok2 = line.substr(p, next - p);

            p = next + 1; next = line.find(',', p);
            if (next == std::string::npos) { std::cerr << "Malformed progress.txt line " << lineNo << "\n"; continue; }
            std::string tok3 = line.substr(p, next - p);

            p = next + 1;
            std::string tok4 = line.substr(p);

            int vals[5] = {0,0,0,0,0};
            std::string toks[5] = {tok0,tok1,tok2,tok3,tok4};
            bool fail = false;
            for (int i = 0; i < 5; ++i) {
                const std::string &t = toks[i];
                if (t.empty()) { fail = true; break; }
                int v = 0;
                if (!parseInteger(t, v)) { fail = true; break; }
                vals[i] = v;
            }
            if (fail) { std::cerr << "Invalid numeric token in progress.txt line " << lineNo << "\n"; continue; }

            progress[progressCount].user_id = vals[0];
            progress[progressCount].current_level = vals[1];
            progress[progressCount].lives_remaining = vals[2];
            progress[progressCount].gem_count = vals[3];
            progress[progressCount].high_score = vals[4];

            progressCount++;
        }

        file.close();
    }
};