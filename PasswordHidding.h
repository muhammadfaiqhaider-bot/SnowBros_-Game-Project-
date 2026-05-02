#pragma once
#include <string>

// used to hash the password so that the password of the user is hide frome others only player should know ..
class PasswordHidding
{
private:

public:

    // Hide the password / Hashing it : if i write this faiq -> convert to something like 12fz 
    std::string hashingPassword(std::string pass)
    {
        std::string fake = "";

        for (int i = 0; i < pass.length(); i++)
        {
            char c = pass[i];
            fake = fake + char(126 % (c + 2));
        }

        return fake;
    }


    // Verification while loggin to match the encoded password with my input.......
    bool verification(std::string input, std::string storedHash)
    {
        std::string temp = hashingPassword(input);

        if (temp == storedHash)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};