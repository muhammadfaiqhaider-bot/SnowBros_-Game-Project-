#pragma once
#include "Enemy.h"
#include <cmath>

class Rocket
{
private:
    float x, y;
    float velocityX, velocityY;
    bool active;

    float screenWidth, screenHeight;

public:
    Rocket(float startX, float startY,
        float playerX, float playerY,
        float w, float h)
    {
        x = startX;
        y = startY;

        screenWidth = w;
        screenHeight = h;

        active = true;

        float dx = playerX - startX;
        float dy = playerY - startY;

        float speed = 4.0f;

        float mag = std::abs(dx) + std::abs(dy);
        if (mag == 0) mag = 1;

        velocityX = (dx / mag) * speed;
        velocityY = (dy / mag) * speed;
    }

    void update()
    {
        if (!active) return;

        x += velocityX;
        y += velocityY;

        if (x < 0 || x > screenWidth || y < 0 || y > screenHeight)
            active = false;
    }

    bool isActive()
    {
        return active;
    }

    void draw()
    {
        if (!active) return;
        // SFML draw rocket here
    }
};




class Gamakichi : public Enemy
{
private:
    int phase;

    int health;

    int attackTimer;
    int attackDuration;

    int phaseTimer;

    static const int MAX_ROCKETS = 5;
    Rocket* rockets[MAX_ROCKETS];

    float playerX, playerY; // must be updated from game

public:
    Gamakichi(float posX, float posY)
        : Enemy(posX, posY, "Gamakichi")
    {
        health = 35;
        phase = 1;

        attackTimer = 0;
        attackDuration = 100;
        phaseTimer = 0;

        playerX = 0;
        playerY = 0;

        for (int i = 0; i < MAX_ROCKETS; i++)
            rockets[i] = nullptr;
    }

    void setPlayerPosition(float px, float py)
    {
        playerX = px;
        playerY = py;
    }

    void movementsUpdate() override
    {
        attackTimer++;
        phaseTimer++;

        phaseChanger();
        handleAttack();
        updateRockets();
    }

    void phaseChanger()
    {
        if (health <= 25)
        {
            phase = 2;
            attackDuration = 80;
        }

        if (health <= 15)
        {
            phase = 3;
            attackDuration = 40;
        }
    }

    void handleAttack()
    {
        if (attackTimer >= attackDuration)
        {
            attackTimer = 0;
            fireRocket();
        }
    }

    void fireRocket()
    {
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] == nullptr)
            {
                rockets[i] = new Rocket(x, y, playerX, playerY, 600, 600);
                break;
            }
        }
    }

    void updateRockets()
    {
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] != nullptr)
            {
                rockets[i]->update();

                if (!rockets[i]->isActive())
                {
                    delete rockets[i];
                    rockets[i] = nullptr;
                }
            }
        }
    }

    void DisplayEnemy() override
    {
        // draw Gamakichi here

        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] != nullptr)
                rockets[i]->draw();
        }
    }

    void reduceHealth() override
    {
        health--;

        if (health <= 0)
        {
            // boss defeated logic
        }
    }

    ~Gamakichi()
    {
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            delete rockets[i];
        }
    }
};