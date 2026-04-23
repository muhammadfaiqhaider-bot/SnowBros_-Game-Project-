#pragma once
#include <string>

class PasswordHidding
{
private:

public:
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