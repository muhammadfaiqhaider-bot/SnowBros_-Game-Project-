#pragma once
#include "Enemy.h"
#include <cstdlib>

class Botom : public Enemy
{
protected:
    bool hitWall;
    bool onGround;

    //Shyt added for random jumps and direction switch  -Cheema
    int jumpTimer;
    int jumpCooldown;  
    int directionTimer;     // for random direction switches
    int directionCooldown;

    // Snow/encasing system
    int hitCount;               // How many times hit (max 3)
    bool isRolling;             // Is encased snowball rolling?
    float rollVelocityX;        // Rolling speed and direction
    int rollTimer;              // 3 second timer before vanish
    int rollDuration;           // 3 seconds at 60 FPS = 180 frames
    bool isDead;                // Fully dead  should be removed

public:
    Botom(float xPos, float yPos) : Enemy(xPos, yPos, "Botom")   // This is parametrized Function whenever in main my Botom is created it 
    {                                                            // Does demand its X and Y coordinate for object creation 
        health = 3;             // 3 hits to encase
        hitWall = false;
        onGround = false;


       //Ts added by me, cheema, Shyt for setting frame height and w for a frame of sprite, setting total frames and animspeed to make animation not change every tick
       // also added random jump and direction changing wali shyt

        frameWidth = 93;
        frameHeight = 95;
        totalFrames = 3;
        animSpeed = 10;
        animTimer = 0;

        jumpTimer = 0;
        jumpCooldown = 180 + rand() % 120;          // jump every 3-5 seconds randomly
        directionTimer = 0;
        directionCooldown = 200 + rand() % 200;

        // Snow system
        hitCount = 0;
        isRolling = false;
        rollVelocityX = 0;
        rollTimer = 0;
        rollDuration = 240;     // 4 seconds at 60 FPS
        isDead = false;

        if (enemyTexture.loadFromFile("assets/Botom_Blue.png")) //Animation shyt, setting texture to the botom png. - cheema
        {
            textureLoaded = true;
            enemySprite.setTexture(enemyTexture);
            enemySprite.setTextureRect(sf::IntRect(0, 136, 88, 88)); //Idk why the idle animation is smaller than the walking -Cheema
            enemySprite.setScale(40.f / 88.f, 40.f / 88.f);
        }

        int direction;
        if (rand() % 2 == 0)          // Randomly direction Assigned 1 for right 
        {
            direction = 1;
        }
        else                          // -1 for left...........
        {
            direction = -1;     
        }

        velocityX = 1.3 * direction;            // Velocity is setted to 1.3 by testing...Some physics applied here like my game is setted
        velocityY = 0;                          // to 60 frames and direction is either 1 or -1 so  (1x1.3x60) that much pixels are moved along
        facingRight = (direction == 1);
    }



    void reduceHealth() override      // Ovverriden Function........
    {
        if (isDead || isRolling)
        {
            return;     // Already dead or rolling ignore hits
        }

        hitCount++;

        if (hitCount >= 3)
        {
            // Fully encased!
            snowCovered = true;
            health = 0;
            velocityX = 0;
            velocityY = 0;
        }
        else
        {
            // Partially hit - slow down
            health--;

            if (hitCount == 1)
            {
                velocityX = velocityX * 0.7f;   // 30% slower after 1st hit
            }
            else if (hitCount == 2)
            {
                velocityX = velocityX * 0.5f;   // 50% slower after 2nd hit
            }
        }
    }

 //26

    void kickRoll(int direction)
    {
        if (!snowCovered || isRolling || isDead)
        {
            return;
        }

        isRolling = true;
        rollTimer = 0;
        rollVelocityX = 6.0f * direction;   // Fast roll speed
    }

    // 26 updates.....
    void movementsUpdate() override // Ovverriden Function........
    {
        // If dead - do nothing
        if (isDead)
        {
            return;
        }


        if (isRolling)
        {
            // Apply gravity while rolling
            if (!onGround)
            {
                velocityY = velocityY + 1.4f;
                if (velocityY > 8.0f)
                {
                    velocityY = 8.0f;
                }
            }
            else
            {
                velocityY = 0;
            }

            // Move rolling snowball
            x = x + rollVelocityX;
            y = y + velocityY;

            // change dir on colliding upon wall.......
            if (x <= 0)
            {
                x = 0;
                rollVelocityX = -rollVelocityX;
            }
            if (x >= 560)
            {
                x = 560;
                rollVelocityX = -rollVelocityX;
            }

            // Count down roll timer
            rollTimer++;
            if (rollTimer >= rollDuration)
            {
                isDead = true;      // Vanish after 4 seconds
            }

            return;
        }


        if (snowCovered)
        {
            // Just apply gravity so it falls to ground
            if (!onGround)
            {
                velocityY = velocityY + 0.5f;
                if (velocityY > 8.f)
                {
                    velocityY = 8.f;
                }
                y = y + velocityY;
            }
            else
            {
                velocityY = 0;
            }

            return;
        }



        if (hitWall)
        { 
            velocityX = -velocityX;                // Collision Logic if I collide with wall Botom will change its direction.....
            hitWall = false;        
            facingRight = (velocityX > 0);       //added this line for facing right in terms of velocity -Cheema
        }

        directionTimer++;
        if (directionTimer >= directionCooldown && onGround)
        {
            directionTimer = 0;
            directionCooldown = 200 + rand() % 200;
            velocityX = -velocityX;
            facingRight = (velocityX > 0);
        }

        jumpTimer++;          //jumps randomly - Cheema
        if (jumpTimer >= jumpCooldown && onGround)
        {
            jumpTimer = 0;
            jumpCooldown = 180 + rand() % 120;

            if (rand() % 9 != 0)      // 1/9 chance to actually jump when timer hits
            {
                velocityY = -12.f;    // jump force
                onGround = false;
            }
        }

        if (!onGround)                // Gravity logic If not on ground so pull's down until touches the ground.....
        {
            velocityY = velocityY + 0.5f;  // Gravity Applied here +0.5 is bcz scrren resolutions are not like normal graphs it works a bit 
            if (velocityY > 8.f)  // cap fall speed
            {
                velocityY = 8.f;
            }
        }                 // Different...top left is (0,0) and +y means u move downwards...
        else
        {
            velocityY = 0;              // Stop falling when on ground
        }

        x = x + velocityX;           // Apply movement along x axis
        y = y + velocityY;           // Apply movement along y axis

        if (x < 0)                             // Screen boundary check here MY screen Resolution is 600x600
        {                                      // So i'll try to merge things inside it ........
            x = 0;
            hitWall = true;
        }

        if (x > 560)                            // 560 bcz to remain things in game screen...
        {
            x = 560;
            hitWall = true;
        }

        // Animation shyt, theres almost no problem with this so if you run into botom animation bugs later do tell, but i garuentree its not from here -Cheema
        int spriteY;

        if (onGround)
        {
            spriteY = 364;    // walk animation row

            animTimer++;
            if (animTimer >= animSpeed)
            {
                animTimer = 0;
                currentFrame = (currentFrame + 1) % 3;
            }
        }
        else if (velocityY <= 0)
        {
            spriteY = 480;    // jump animation
            currentFrame = 0;
            animTimer = 0;
        }
        else
        {
            spriteY = 590;    // drop down animation
            currentFrame = 0;
            animTimer = 0;
        }

        enemySprite.setTextureRect(sf::IntRect(
            currentFrame * 93,
            spriteY,    //sets height from where to pick the frame
            95,
            93  //acc height and weidht of frame
        ));

        if (!facingRight)
        {
            enemySprite.setScale(40.f / 88.f, 40.f / 88.f);
            enemySprite.setPosition(x, y);
        }
        else
        {
            enemySprite.setScale(-(40.f / 88.f), 40.f / 88.f);
            enemySprite.setPosition(x + 40.f, y);
        }
    }

 

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (isDead)
        {
            return;     // Dead  don't draw anything
        }

        if (isRolling)
        {
            sf::CircleShape rollingBall(22.0f);
            rollingBall.setFillColor(sf::Color::White);
            rollingBall.setOutlineColor(sf::Color(180, 220, 255));
            rollingBall.setOutlineThickness(3.0f);
            rollingBall.setPosition(x, y);
            window.draw(rollingBall);

            

            return;
        }

        // FULLY ENCASED (stationary) 
        if (snowCovered)
        {
            sf::CircleShape snowBall(20.0f);
            snowBall.setFillColor(sf::Color::White);
            snowBall.setOutlineColor(sf::Color(180, 220, 255));
            snowBall.setOutlineThickness(2.0f);
            snowBall.setPosition(x, y);
            window.draw(snowBall);

            // Small indicator showing its kickable
            sf::Text kickText;
            // No font here - just draw arrow indicators
            sf::RectangleShape leftArrow(sf::Vector2f(8.0f, 3.0f));
            leftArrow.setFillColor(sf::Color::Cyan);
            leftArrow.setPosition(x - 12.0f, y + 18.0f);
            window.draw(leftArrow);

            sf::RectangleShape rightArrow(sf::Vector2f(8.0f, 3.0f));
            rightArrow.setFillColor(sf::Color::Cyan);
            rightArrow.setPosition(x + 44.0f, y + 18.0f);
            window.draw(rightArrow);

            return;
        }

        // PARTIALLY HIT  snow overlay 
        if (textureLoaded)
        {
            window.draw(enemySprite);
        }
        else
        {
            sf::RectangleShape tempShape(sf::Vector2f(40.f, 40.f));
            tempShape.setFillColor(sf::Color::Red);
            tempShape.setPosition(x, y);
            window.draw(tempShape);
        }

        // Show snow overlay based on hit count
        if (hitCount == 1)
        {
            // Light snow - 25% covered
            sf::CircleShape snowOverlay(20.f);
            snowOverlay.setFillColor(sf::Color(255, 255, 255, 80));
            snowOverlay.setPosition(x, y);
            window.draw(snowOverlay);
        }
        else if (hitCount == 2)
        {
            // Heavy snow - 65% covered
            sf::CircleShape snowOverlay(20.f);
            snowOverlay.setFillColor(sf::Color(255, 255, 255, 160));
            snowOverlay.setPosition(x, y);
            window.draw(snowOverlay);
        }
    }


    void instantKill()
    {
        isDead = true;
        snowCovered = true;
        velocityX = 0;
        velocityY = 0;
    }


    // GETTERS
  
    bool getSnowCoveredStatus()
    {
        return snowCovered;
    }

    bool getIsDead() 
    { 
        return isDead; 
    }
    bool getIsRolling() 
    { 
        return isRolling; 
    }
    bool getIsEncased() 
    { 
        return snowCovered; 
    }
    int getHitCount()
    { 
        return hitCount; 
    }

    float getRollVelocityX() 
    { 
        return rollVelocityX; 
    }

    void snapToGround(float groundY)
    {
        y = groundY;
        velocityY = 0;
        onGround = true;
    }

    void setHitWall(bool value)                // Cheema  calls this when Botom hits a wall or some edges.......Sybau lil gigga nilla UwU
    { 
        hitWall = value; 
    }
    void setOnGround(bool value)           // Cheema use this  when Botom lands on flat surfaxe
    { 
        onGround = value; 
    }

    virtual ~Botom()
    {
    }
};