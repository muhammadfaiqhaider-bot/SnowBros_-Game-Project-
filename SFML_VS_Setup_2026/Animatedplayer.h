#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>

// ============================================================
// AnimatedPlayer
// Sits between Player and the concrete character classes.
// Holds ALL the sprite-sheet animation logic so Nick, Faiq,
// and Cheema don't duplicate it.  Each subclass only needs to:
//   1. Call AnimatedPlayer(x, y, name, "assets/mysheet.png")
//   2. Set its own stats (lives, speed, etc.)
// ============================================================
class AnimatedPlayer : public Player
{
protected:
    // ---- DISPLAY SIZE (26:41 ratio, scaled up) ----
    static constexpr float DISPLAY_W = 32.5f;
    static constexpr float DISPLAY_H = 51.25f;

private:
    // ---- ANIMATION STATE ----
    enum class AnimState { IDLE, WALK, THROW, JUMP, FALL, HIT, KNOCKBACK, DEAD };

    AnimState currentState;
    int currentFrame;
    int animTimer;
    static const int ANIM_SPEED = 8;

    // fall
    int fallFrame;
    int fallTimer;
    bool wasAscending;

    // throw
    bool isThrowingAnim;
    int throwFrame;
    int throwTimer;

    // hit
    bool isHitAnim;
    int hitFrame;
    int hitTimer;

    // knockback / dead
    bool isKnockbackAnim;
    int knockbackFrame;
    int knockbackTimer;
    bool isDead;

public:
    // spriteSheetPath  — path to the PNG that uses the same layout as sonic.png
    AnimatedPlayer(float x, float y,
        const std::string& name,
        const std::string& spriteSheetPath)
        : Player(x, y, name)
    {
        resetAnimState();
        loadSheet(spriteSheetPath);
    }

    // ---- Called by Nick::reset() / Faiq::reset() etc. ----
    void resetAnim(float nx, float ny, const std::string& spriteSheetPath)
    {
        x = nx;  y = ny;
        velocityX = 0.f;  velocityY = 0.f;
        onGround = true;  isJumping = false;
        lifeStatus = true;
        isInvincible = false;  invincibleTimer = 0;
        SpeedBoostStatus = false;
        powerBallactiveStatus = false;
        distanceIncreaseStatus = false;
        hasBalloonMode = false;
        powerUpTimer = 0;
        powerupMessage.clear();  powerupMessageTimer = 0;

        if (snowball != nullptr) { delete snowball; snowball = nullptr; }

        resetAnimState();
        loadSheet(spriteSheetPath);
    }

    // ---- Trigger throw animation when a snowball is thrown ----
    void triggerThrowAnim()
    {
        isThrowingAnim = true;
        throwFrame = 0;
        throwTimer = 0;
    }

    // Hides Player::throwSnowball so the animation fires automatically
    void throwSnowball()
    {
        Player::throwSnowball();
        triggerThrowAnim();
    }

    void triggerHitAnim()
    {
        if (isKnockbackAnim || isDead) return;
        isHitAnim = true;
        hitFrame = 0;
        hitTimer = 0;
    }

    // ---- movementsUpdate / displayPlayer (satisfy Player pure virtuals) ----
    void movementsUpdate() override
    {
        if (isDead) return;
        baseMovement();
        updateAnimation();
    }

    void displayPlayer(sf::RenderWindow& window) override
    {
        // Blink during invincibility
        if (isInvincible && invincibleTimer % 10 < 5)
            return;
        window.draw(playerSprite);
    }

    // Override loseLife to trigger the hit animation
    void loseLife() override
    {
        if (isInvincible) return;
        lives--;
        triggerHitAnim();
        if (lives <= 0)
        {
            lives = 0;
            lifeStatus = false;
        }
        else
        {
            isInvincible = true;
            invincibleTimer = 120;
        }
    }

private:

    void resetAnimState()
    {
        currentState = AnimState::IDLE;
        currentFrame = 0;
        animTimer = 0;
        fallFrame = 0;
        fallTimer = 0;
        wasAscending = false;
        isThrowingAnim = false;
        throwFrame = 0;
        throwTimer = 0;
        isHitAnim = false;
        hitFrame = 0;
        hitTimer = 0;
        isKnockbackAnim = false;
        knockbackFrame = 0;
        knockbackTimer = 0;
        isDead = false;
    }

    void loadSheet(const std::string& path)
    {
        bool loaded = playerTexture.loadFromFile(path);
        if (!loaded)
        {
            sf::Image img;
            img.create(26, 41, sf::Color(255, 0, 255));   // magenta fallback
            playerTexture.loadFromImage(img);
        }
        playerSprite.setTexture(playerTexture);
        playerSprite.setTextureRect(sf::IntRect(0, 85, 27, 41));
        playerSprite.setScale(DISPLAY_W / 27.f, DISPLAY_H / 41.f);
        playerSprite.setPosition(x, y);
    }

    // ---- Sets the sprite rect and handles left/right flipping ----
    void setFrame(int fx, int fy, int fw, int fh)
    {
        playerSprite.setTextureRect(sf::IntRect(fx, fy, fw, fh));
        float sx = DISPLAY_W / (float)fw;
        float sy = DISPLAY_H / (float)fh;

        if (direction == -1)   // facing left — flip horizontally
        {
            playerSprite.setScale(-sx, sy);
            playerSprite.setPosition(this->x + DISPLAY_W, this->y);
        }
        else                   // facing right
        {
            playerSprite.setScale(sx, sy);
            playerSprite.setPosition(this->x, this->y);
        }
    }

    void updateAnimation()
    {
        // Track whether we have ever ascended (so fall anim only plays after a jump)
        if (!onGround && velocityY > 0.f && !wasAscending)
            wasAscending = true;
        if (onGround)
            wasAscending = false;

        // ---- DEAD ----
        if (isDead)
        {
            setFrame(0, 939, 37, 37);
            return;
        }

        // ---- KNOCKBACK ----
        if (isKnockbackAnim)
        {
            knockbackTimer++;
            if (knockbackTimer >= ANIM_SPEED) { knockbackTimer = 0; knockbackFrame++; }

            if (knockbackFrame >= 2)
            {
                isKnockbackAnim = false;
                isDead = true;
                setFrame(0, 939, 37, 37);
                return;
            }
            setFrame(461, 536, 37, 24);
            return;
        }

        // ---- HIT ----
        if (isHitAnim)
        {
            hitTimer++;
            if (hitTimer >= ANIM_SPEED) { hitTimer = 0; hitFrame++; }

            if (hitFrame >= 3)
            {
                isHitAnim = false;
                hitFrame = 0;
                if (!lifeStatus)
                {
                    isKnockbackAnim = true;
                    knockbackFrame = 0;
                    knockbackTimer = 0;
                }
                return;
            }
            setFrame(hitFrame * 35, 883, 35, 37);
            return;
        }

        // ---- THROW ----
        if (isThrowingAnim)
        {
            throwTimer++;
            if (throwTimer >= ANIM_SPEED) { throwTimer = 0; throwFrame++; }

            if (throwFrame >= 3)
            {
                isThrowingAnim = false;
                throwFrame = 0;
            }
            else
            {
                if (throwFrame == 0) setFrame(0, 245, 28, 41);
                else if (throwFrame == 1) setFrame(28, 245, 36, 41);
                else                      setFrame(64, 245, 36, 41);
                return;
            }
        }

        // ---- JUMP (ascending) ----
        if (!onGround && velocityY < 0)
        {
            wasAscending = true;
            animTimer++;
            if (animTimer >= ANIM_SPEED) { animTimer = 0; currentFrame = (currentFrame + 1) % 5; }
            setFrame(currentFrame * 24, 406, 24, 44);
            return;
        }

        // ---- FALL (descending after a real jump) ----
        if (!onGround && velocityY >= 0 && wasAscending)
        {
            setFrame(208, 414, 26, 44);
            currentFrame = 0;
            animTimer = 0;
            return;
        }

        // ---- WALK ----
        if (velocityX != 0)
        {
            setFrame(384, 85, 26, 41);
            return;
        }

        // ---- IDLE ----
        animTimer++;
        if (animTimer >= ANIM_SPEED) { animTimer = 0; currentFrame = (currentFrame + 1) % 6; }
        setFrame(currentFrame * 24, 85, 24, 41);
    }
};