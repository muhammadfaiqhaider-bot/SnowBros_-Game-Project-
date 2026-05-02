#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>

class Nick : public Player
{
public:
    int currentFrame;
    int animTimer;
    int animSpeed;

    // Sprite sheet dimensions
    // Sheet is 2577x4536 with 6 columns and many rows
    // Each frame = 2577/6 = 429 wide, 4536/10 = 453 tall (approx)
    static const int FRAME_W = 429;
    static const int FRAME_H = 453;
    float targetSize;

    Nick(float x, float y) : Player(x, y, "Nick")
    {
        lives = 20;
        speed = 3.0f;
        snowballPower = 1.0f;
        snowballDistance = 200.0f;

        currentFrame = 0;
        animTimer = 0;
        animSpeed = 8;
        targetSize = 50.0f;     // Size on screen

        // Try multiple common filenames (case differences) and provide a visible placeholder if load fails
        bool texLoaded = false;
        texLoaded = playerTexture.loadFromFile("assets/nick.png");
        if (!texLoaded) texLoaded = playerTexture.loadFromFile("assets/Nick.png");
        if (!texLoaded) texLoaded = playerTexture.loadFromFile("nick.png");

        if (texLoaded)
        {
            playerSprite.setTexture(playerTexture);

            // Set initial frame
            playerSprite.setTextureRect(sf::IntRect(
                0,          // x on sheet
                0,          // y on sheet
                FRAME_W,
                FRAME_H
            ));
        }
        else
        {
            // Placeholder visible texture so programmer can see something and detect missing asset
            sf::Image img;
            img.create(FRAME_W, FRAME_H, sf::Color(255, 0, 255)); // magenta
            playerTexture.loadFromImage(img);
            playerSprite.setTexture(playerTexture);
            playerSprite.setTextureRect(sf::IntRect(0, 0, FRAME_W, FRAME_H));
        }
    }

    void movementsUpdate() override
    {
        baseMovement();
        updateAnimation();
    }

    void displayPlayer(sf::RenderWindow& window) override
    {
        // Blink when invincible
        if (isInvincible)
        {
            if (invincibleTimer % 10 < 5)
            {
                return;
            }
        }

        window.draw(playerSprite);
    }

private:
    void updateAnimation()
    {
        int frameRow = 0;
        int frameCount = 4;

        if (!onGround)
        {
            frameRow = 1;
            frameCount = 1;
            currentFrame = 0;
            animTimer = 0;
        }
        else if (velocityX != 0)
        {
            frameRow = 0;
            frameCount = 4;

            animTimer++;
            if (animTimer >= animSpeed)
            {
                animTimer = 0;
                currentFrame = (currentFrame + 1) % frameCount;
            }
        }
        else
        {
            frameRow = 0;
            currentFrame = 0;
            animTimer = 0;
        }

        // Set texture rectangle
        playerSprite.setTextureRect(sf::IntRect(
            currentFrame * FRAME_W,
            frameRow * FRAME_H,
            FRAME_W,
            FRAME_H
        ));

        // Fix - scale like Botom does it
        // Target display size is 40x40 like Botom
        float displaySize = 40.0f;
        float scaleX = displaySize / (float)FRAME_W;
        float scaleY = displaySize / (float)FRAME_H;

        if (direction == -1)
        {
            // Facing left
            playerSprite.setScale(scaleX, scaleY);
            playerSprite.setPosition(x, y);
        }
        else
        {
            // Facing right - flip
            playerSprite.setScale(-scaleX, scaleY);
            playerSprite.setPosition(x + displaySize, y);
        }
    }
};