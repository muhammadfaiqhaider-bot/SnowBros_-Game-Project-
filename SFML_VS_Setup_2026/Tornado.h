#pragma once
#include "FlyingFoogo.h"
#include <cstdlib>

class Knives
{
private:
    float x;
    float velocityX;
    bool inAir;
    float screenSizeLimit;

public:
    Knives(float startX, float playerPos, float lim)
    {
        x = startX;
        inAir = true;
        screenSizeLimit = lim;

        if (playerPos > startX)
        {
            velocityX = 2;
        }
        else
        {
            velocityX = -2;
        }
    }

    void update()
    {
        if (inAir)
        {
            x = x + velocityX;
        }
        if (x < 0 || x > screenSizeLimit)
        {
            inAir = false;
        }
    }

    bool isActive()
    {
        return inAir;
    }

    void deactivate()
    {
        inAir = false;
    }

    void draw()
    {
        // SFML work cheema knive drawing type shyt
    }
};

class Tornado : public FlyingFoogaFoog
{
private:
    bool isTeleporting;
    int teleportTimer;
    int teleportDuration;
    bool throwKnife;
    int attackTimer;
    int attackDuration;
    Knives* churi;

public:
    Tornado(float posX, float posY) : FlyingFoogaFoog(posX, posY)
    {
        isTeleporting = false;
        teleportTimer = 0;
        teleportDuration = 60;

        throwKnife = false;
        attackTimer = 0;
        attackDuration = 120;
        churi = nullptr;
    }

    void movementsUpdate() override
    {

        teleportTimer++;
        attackTimer++;

        if (isTeleporting)
        {

            if (teleportTimer >= teleportDuration)
            {
                x = x + rand() % 31 - 15;
                y = y + rand() % 31 - 15;

                isTeleporting = false;
                teleportTimer = 0;
            }
        }
        else
        {
            FlyingFoogaFoog::movementsUpdate();
        }

        if (!isTeleporting && teleportTimer > 15) 
        {
            if (rand() % 4 == 0)
            {
                isTeleporting = true;
                teleportTimer = 0; 
            }
        }


        if (attackTimer >= attackDuration)
        {
            attackTimer = 0;

            if (churi == nullptr)
            {
                churi = new Knives(x, y, 200);
            }
        }

        if (churi != nullptr)
        {
            churi->update();

            if (!churi->isActive())
            {
                delete churi;
                churi = nullptr;
            }
        }
    }


    






    void DisplayEnemy() override
    {
        // Cheeem ayour work for tornadooo making and all stuff

        if (churi != nullptr)
        {
            churi->draw();
        }
    }


    ~Tornado()
    {
        delete churi;
    }

};