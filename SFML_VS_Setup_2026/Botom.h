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
    int directionTimer;      // for random direction switches
    int directionCooldown;

public:
    Botom(float xPos, float yPos) : Enemy(xPos, yPos, "Botom")   // This is parametrized Function whenever in main my Botom is created it 
    {                                                            // Does demand its X and Y coordinate for object creation 
        
        health = 1;
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
        jumpCooldown = 180 + rand() % 120;   // jump every 3-5 seconds randomly

        directionTimer = 0;
        directionCooldown = 200 + rand() % 200;


        if (enemyTexture.loadFromFile("assets/Botom_Blue.png")) //Animation shyt, setting texture to the botom png. - cheema
        {
            textureLoaded = true;
            enemySprite.setTexture(enemyTexture);

            // Start on idle
            enemySprite.setTextureRect(sf::IntRect(0, 136, 88, 88)); //Idk why the idle animation is smaller than the walking -Cheema
            enemySprite.setScale(40.f / 88.f, 40.f / 88.f);
        }


        int direction;                              
        if (rand() % 2 == 0)                                 // Randomly direction Assigned 1 for right 
        {                                                    // -1 for left...........
            direction = 1;
        } 
        else
        {
            direction = -1;
        }
        velocityX = 1.3 * direction;                    // Velocity is setted to 1.3 by testing...Some physics applied here like my game is setted
        velocityY = 0;                                  // to 60 frames and direction is either 1 or -1 so  (1x1.3x60) that much pixels are moved along 
        facingRight = (direction == 1);
    }                                                   // X-axis......




    void movementsUpdate() override // Ovverriden Function........
    {

        if (hitWall)
        {
            velocityX = -velocityX;            // Collision Logic if I collide with wall Botom will change its direction.....
            hitWall = false;
            facingRight = (velocityX > 0);  //added this line for facing right in terms of velocity -Cheema
        }
        directionTimer++;
        if (directionTimer >= directionCooldown && onGround) //added this, changes direction randomly - Cheema
        {
            directionTimer = 0;
            directionCooldown = 200 + rand() % 200;
            velocityX = -velocityX;
            facingRight = (velocityX > 0);
        }


        jumpTimer++;    //jumps randomly - Cheema
        if (jumpTimer >= jumpCooldown && onGround)
        {
            jumpTimer = 0;
            jumpCooldown = 180 + rand() % 120;

            if (rand() % 9 != 0)    // 1/9 chance to actually jump when timer hits
            {
                velocityY = -12.f;   // jump force
                onGround = false;
            }
        }
        
        if (!onGround)                         // Gravity logic If not on ground so pull's down until touches the ground.....
        {
            velocityY = velocityY + 0.5f;      // Gravity Applied here +0.5 is bcz scrren resolutions are not like normal graphs it works a bit 
            if (velocityY > 8.f)    // cap fall speed
                velocityY = 8.f;
        }                                      // Different...top left is (0,0) and +y means u move downwards...
        else
        {
            velocityY = 0;                     // Stop falling when on ground
        }



        
        x = x +  velocityX;                    // Apply movement along x axis
        y = y +  velocityY;                    // Apply movement along y axis

        
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

        // Flipping of sprite in terms of facing right -Cheema i think
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

    void snapToGround(float groundY) //new func snap to ground, automatically gets on ground to true but snaps the enermy to the groundY in argument -Cheema
    {
        y = groundY;
        velocityY = 0;
        onGround = true;
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (textureLoaded)
        {
            window.draw(enemySprite);   //animation shyt - Cheema
        }
        else
        {
            // Fallback rectangle if texture fails
            sf::RectangleShape tempShape(sf::Vector2f(40.f, 40.f));
            tempShape.setFillColor(sf::Color::Red);
            tempShape.setPosition(x, y);
            window.draw(tempShape);
        }
    }


    

    
    void setHitWall(bool value)              // Cheema  calls this when Botom hits a wall or some edges.......Sybau lil gigga nilla UwU
    {
        hitWall = value;
    }

    
    void setOnGround(bool value)    
    {                                        // Cheema use this  when Botom lands on flat surfaxe
        onGround = value;
    }

    virtual ~Botom()
    {
    }
};