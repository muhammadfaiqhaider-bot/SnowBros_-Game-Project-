#pragma once
#include "Player.h"

class Nick : public Player
{
private:
    int animFrame;
    int animTimer;
    static const int ANIM_SPEED = 8; // frames per sprite switch

public:
    Nick(float x, float y) : Player(x, y, "Nick")
    {
        lives = 5;
        speed = 3.0f;
        snowballPower = 1.0f;
        snowballDistance = 200.0f;

        animFrame = 0;
        animTimer = 0;

        playerTexture.loadFromFile("assets/Nick.png");
        playerSprite.setTexture(playerTexture);
        playerSprite.setTextureRect(sf::IntRect(120, 75, 200, 250));
        playerSprite.setScale(40.f / 200.f, 50.f / 250.f);
    }

    void movementsUpdate() override
    {                                         // Movements are the same for player mechanics would be same sao whole 
        baseMovement();                       // fucntion is called form the base........
    }

    void displayPlayer(sf::RenderWindow& window) override
    {
        // Blinking when invincible
        if (isInvincible && invincibleTimer % 10 < 5)
            return;

        // Sprite dimensions and padding
        const int FRAME_W = 200;
        const int FRAME_H = 250;
        const int X_START = 120;  // left border to first frame
        const int X_GAP = 90;   // gap between frames
        const int Y_START = 75;   // top border to first row

        // Frame indices in the sheet (left to right):
        // 0 = idle, 1 = walk mid, 2 = walk extreme1, 3 = walk extreme2, 4 = jump

        int sheetFrame = 0;

        if (!onGround)
        {
            // Jumping
            sheetFrame = 4;
            animFrame = 0; // reset walk cycle for when landing
            animTimer = 0;
        }
        else if (velocityX != 0)
        {
            // Walk cycle: extreme1 -> middle -> extreme2 -> middle 
            // animFrame goes 0,1,2,3 mapped to sheetFrames 1,2,3,2
            static const int walkMap[4] = { 1, 2, 3, 2 };

            animTimer++;
            if (animTimer >= ANIM_SPEED)
            {
                animTimer = 0;
                animFrame = (animFrame + 1) % 4;
            }
            sheetFrame = walkMap[animFrame];
        }
        else
        {
            // Idle
            sheetFrame = 0;
            animFrame = 0;
            animTimer = 0;
        }
        int frameX = X_START + sheetFrame * (FRAME_W + X_GAP);
        int frameY = Y_START;
        playerSprite.setTextureRect(sf::IntRect(frameX, frameY, FRAME_W, FRAME_H));

        // Flip when facing left
        if (direction == -1)
        {
            playerSprite.setScale(-(40.f / 200.f), 50.f / 250.f);
            playerSprite.setPosition(x + 40.f, y);
        }
        else
        {
            playerSprite.setScale(40.f / 200.f, 50.f / 250.f);
            playerSprite.setPosition(x, y);
        }

        window.draw(playerSprite);
        playerSprite.setTextureRect(sf::IntRect(frameX, frameY, FRAME_W, FRAME_H));

    }
};