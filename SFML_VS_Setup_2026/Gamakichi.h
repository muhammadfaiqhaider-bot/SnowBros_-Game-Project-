#pragma once
#include "Enemy.h"
#include <cmath>

class Rocket
{
private:
    float x;
    float y;
    float velocityX;
    float velocityY;
    bool active;
    float screenWidth;
    float screenHeight;

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

        // Fixed - using Pythagorean distance like Knife class
        float mag = sqrt(dx * dx + dy * dy);

        if (mag == 0)
        {
            mag = 1;
        }

        float speed = 4.0f;
        velocityX = (dx / mag) * speed;
        velocityY = (dy / mag) * speed;
    }

    void update()
    {
        if (!active)
        {
            return;
        }

        x += velocityX;
        y += velocityY;

        if (x < 0 || x > screenWidth || y < 0 || y > screenHeight)
        {
            active = false;
        }
    }

    bool isActive()
    {
        return active;
    }

    float getX()
    {
        return x;
    }

    float getY()
    {
        return y;
    }

    void draw(sf::RenderWindow& window)
    {
        if (!active)
        {
            return;
        }

        // Orange rocket rectangle
        sf::RectangleShape rocketShape(sf::Vector2f(15.f, 8.f));
        rocketShape.setFillColor(sf::Color(255, 100, 0));   // Orange
        rocketShape.setPosition(x, y);
        window.draw(rocketShape);

        // Explosion effect - small red circle at front
        sf::CircleShape explosionTip(5.f);
        explosionTip.setFillColor(sf::Color::Red);
        explosionTip.setPosition(x + 10.f, y - 1.f);
        window.draw(explosionTip);
    }

    ~Rocket()
    {
    }
};


class Gamakichi : public Enemy
{
private:
    int phase;
    int attackTimer;
    int attackDuration;
    int phaseTimer;
    bool isDefeated;
    bool phaseChanged;      // Flag to prevent phase reset every frame

    static const int MAX_ROCKETS = 5;
    Rocket* rockets[MAX_ROCKETS];

    float playerX;
    float playerY;

public:
    Gamakichi(float posX, float posY) : Enemy(posX, posY, "Gamakichi")
    {
        // Use parent health directly
        health = 35;

        phase = 1;
        attackTimer = 0;
        attackDuration = 100;
        phaseTimer = 0;
        isDefeated = false;
        phaseChanged = false;

        playerX = 0;
        playerY = 0;

        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            rockets[i] = nullptr;
        }
    }

    void setPlayerPosition(float px, float py)
    {
        playerX = px;
        playerY = py;
    }

    void movementsUpdate() override
    {
        // Dont update if defeated
        if (isDefeated)
        {
            return;
        }

        // Keep Gamakichi fixed on screen
        if (x < 0)
        {
            x = 0;
        }
        if (x > 520)        // 600 - 80 (boss width)
        {
            x = 520;
        }
        if (y < 0)
        {
            y = 0;
        }
        if (y > 480)        // 600 - 120 (boss height)
        {
            y = 480;
        }

        attackTimer++;
        phaseTimer++;

        phaseChanger();
        handleAttack();
        updateRockets();
    }

    void phaseChanger()
    {
        // Phase 2 - only trigger once when health drops to 25
        if (health <= 25 && phase == 1)
        {
            phase = 2;
            attackDuration = 80;    // Faster attacks in phase 2
            phaseTimer = 0;
        }

        // Phase 3 - only trigger once when health drops to 15
        if (health <= 15 && phase == 2)
        {
            phase = 3;
            attackDuration = 40;    // Even faster in phase 3
            phaseTimer = 0;
        }
    }

    void handleAttack()
    {
        if (attackTimer >= attackDuration)
        {
            attackTimer = 0;

            // Phase 1 - fire 1 rocket
            if (phase == 1)
            {
                fireRocket();
            }

            // Phase 2 - fire 2 rockets
            if (phase == 2)
            {
                fireRocket();
                fireRocket();
            }

            // Phase 3 - fire 3 rockets
            if (phase == 3)
            {
                fireRocket();
                fireRocket();
                fireRocket();
            }
        }
    }

    void fireRocket()
    {
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] == nullptr)
            {
                rockets[i] = new Rocket(x, y, playerX, playerY, 600.f, 600.f);
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

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (isDefeated)
        {
            return;
        }

        // Gamakichi - large purple rectangle (bigger than Mogera)
        sf::RectangleShape gamakichiShape(sf::Vector2f(100.f, 120.f));
        gamakichiShape.setFillColor(sf::Color(100, 0, 150));    // Dark purple
        gamakichiShape.setPosition(x, y);
        window.draw(gamakichiShape);

        // Health bar background (red)
        sf::RectangleShape healthBarBackground(sf::Vector2f(100.f, 12.f));
        healthBarBackground.setFillColor(sf::Color::Red);
        healthBarBackground.setPosition(x, y - 20.f);
        window.draw(healthBarBackground);

        // Health bar foreground (shrinks as health decreases)
        float healthPercent = (float)health / 35.0f;
        sf::RectangleShape healthBarForeground(sf::Vector2f(100.f * healthPercent, 12.f));
        healthBarForeground.setFillColor(sf::Color::Green);
        healthBarForeground.setPosition(x, y - 20.f);
        window.draw(healthBarForeground);

        // Phase indicator - small colored box shows current phase
        sf::RectangleShape phaseIndicator(sf::Vector2f(20.f, 10.f));

        if (phase == 1)
        {
            phaseIndicator.setFillColor(sf::Color::Green);
        }
        if (phase == 2)
        {
            phaseIndicator.setFillColor(sf::Color::Yellow);
        }
        if (phase == 3)
        {
            phaseIndicator.setFillColor(sf::Color::Red);
        }

        phaseIndicator.setPosition(x, y - 35.f);
        window.draw(phaseIndicator);

        // Draw all active rockets
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] != nullptr)
            {
                rockets[i]->draw(window);
            }
        }
    }

    void reduceHealth() override
    {
        health--;

        if (health <= 0)
        {
            health = 0;
            isDefeated = true;
            snowCovered = true;
        }
    }

    bool getIsDefeated()
    {
        return isDefeated;
    }

    int getPhase()
    {
        return phase;
    }

    ~Gamakichi()
    {
        for (int i = 0; i < MAX_ROCKETS; i++)
        {
            if (rockets[i] != nullptr)
            {
                delete rockets[i];
                rockets[i] = nullptr;
            }
        }
    }
};