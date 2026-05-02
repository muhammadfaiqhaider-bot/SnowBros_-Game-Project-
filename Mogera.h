#pragma once
#include "Enemy.h"
#include "MogeraChild.h"

class Mogera : public Enemy
{
private:
    float spawnTimer;
    float childSpawnInterval;
    float playerX;
    float playerY;
    bool isDefeated;
    bool onGround;
    float velocityY;
    int frameGap;

    static const int numberOfChilds = 10;
    MogeraChild child[numberOfChilds];

public:
    Mogera(float posX, float posY) : Enemy(posX, posY, "Mogera")
    {

        health = 10;           // Health Increased

        spawnTimer = 0;
        childSpawnInterval = 180;      // Spawn child every 3 seconds simple Maths time= frames(180)/fps(60)..

        playerX = 0;     // 0 bcz Mogera is stationary ......
        playerY = 0;

        onGround = false;
        velocityY = 0;

        isDefeated = false;

        // Setup animation for Mogera: sprite sheet is a single row with 3 frames.
        // Sprite sheet settings (updated): single row with 3 frames.
        // New sheet resolution: total ~1660x457, frame width = 529, frame height = 457, gap between frames = 15
        frameWidth = 529;
        frameHeight = 457;
        totalFrames = 3;
        currentFrame = 0;
        animTimer = 0;
        animSpeed = 15; // slower idle animation
        frameGap = 15;

        // Try to load Mogera sprite row. Fallback to simple rectangle if missing.
        if (enemyTexture.loadFromFile("assets/Mogera_row.png"))
        {
            textureLoaded = true;
            enemySprite.setTexture(enemyTexture);
            // Start with first frame
            enemySprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
            // Scale sprite down to roughly match previous rectangle size (60x60)
            float sx = 60.f / (float)frameWidth;
            float sy = 60.f / (float)frameHeight;
            enemySprite.setScale(sx, sy);
            enemySprite.setPosition(x, y);
        }
    }


    void setPlayerPosition(float pX, float pY) // every frame this function is called and 
    {                                          // player coordinates are passed........
        playerX = pX;
        playerY = pY;
    }

    void movementsUpdate() override
    {


        if (isDefeated)
        {
            return;
        }

        // Apply gravity
        if (!onGround)
        {
            velocityY += 0.5f;
            if (velocityY > 8.f)
            {
                velocityY = 8.f;
            }
            y += velocityY;
        }
        else
        {
            velocityY = 0;
        }

        if (isDefeated)                      // base condition if it is defeated we ll be instantly 
        {                                    // backed from the function..........
            return;
        }


        spawnTimer++;
        if (spawnTimer >= childSpawnInterval)    // This is the child Mogera spawn logic
        {
            for (int i = 0; i < numberOfChilds; i++)
            {
                if (!child[i].isActive())
                {
                    child[i].spawn(x, y, playerX);
                    break;
                }
            }
            spawnTimer = 0;
        }


        for (int i = 0; i < numberOfChilds; i++)
        {
            if (child[i].isActive())
            {
                child[i].movementsUpdate();      // Function called and its movmement is adjusted.....
            }
        }

        // Handle idle animation (single row sprite sheet)
        if (textureLoaded && totalFrames > 1)
        {
            animTimer++;
            if (animTimer >= animSpeed)
            {
                animTimer = 0;
                currentFrame = (currentFrame + 1) % totalFrames;
            }

            int left = currentFrame * (frameWidth + frameGap);
            enemySprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
            enemySprite.setPosition(x, y);
        }
    }


    void snapChildToGround(int index, float groundY)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            child[index].snapToGround(groundY);
            child[index].setOnGround(true);
        }
    }

    // Called by Level when a snowball hits a child to kill it
    void killChild(int index)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            child[index].instantKill();
        }
    }

    bool isChildActive(int index)
    {
        if (index >= 0 && index < numberOfChilds)
            return child[index].isActive();
        return false;
    }


    void deactivateChild(int index)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            child[index].deactivate();
        }
    }


    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (isDefeated)
        {
            return;
        }

        if (textureLoaded)
        {
            // Ensure sprite uses current frame and position
            const int gap = 5;
            int left = currentFrame * (frameWidth + gap);
            enemySprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
            enemySprite.setPosition(x, y);
            window.draw(enemySprite);
        }
        else
        {
            sf::RectangleShape mogeraShape(sf::Vector2f(60.f, 60.0f)); // Fallback
            mogeraShape.setFillColor(sf::Color(0, 180, 0));           // Dark green
            mogeraShape.setPosition(x, y);
            window.draw(mogeraShape);
        }


        sf::RectangleShape healthBarBackground(sf::Vector2f(80.0f, 10.0f));         // Red Bar on the Top of Mogera indicating his health 
        healthBarBackground.setFillColor(sf::Color::Red);                           // Decreasing Health........which is overlapped by 
        healthBarBackground.setPosition(x, y - 15.0f);                              // Green bar which is shring with the health decreasing
        window.draw(healthBarBackground);


        float healthPercent = (float)health / 10.0f;
        sf::RectangleShape healthBarForeground(sf::Vector2f(80.f * healthPercent, 10.f));
        healthBarForeground.setFillColor(sf::Color::Green);
        healthBarForeground.setPosition(x, y - 15.f);
        window.draw(healthBarForeground);



        for (int i = 0; i < numberOfChilds; i++)
        {
            if (child[i].isActive())
            {
                child[i].DisplayEnemy(window);
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

    // Integrate with Level's expectation
    bool getIsDead() override
    {
        return isDefeated;
    }



    void setChildOnGround(int index, bool value)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            child[index].setOnGround(value);
        }
    }

    int getNumberOfChilds()
    {
        return numberOfChilds;
    }

    float getChildX(int index)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            return child[index].getPositionX();
        }
        return -1;
    }


    void setOnGround(bool value)
    {
        onGround = value;
    }

    void snapToGround(float groundY)
    {
        y = groundY;
        velocityY = 0;
        onGround = true;
    }



    float getChildY(int index)
    {
        if (index >= 0 && index < numberOfChilds)
        {
            return child[index].getPositionY();
        }
        return -1;
    }

    virtual ~Mogera()
    {
    }
};