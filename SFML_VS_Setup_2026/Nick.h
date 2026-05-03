#pragma once
#include "AnimatedPlayer.h"

class Nick : public AnimatedPlayer
{
public:
    Nick(float x, float y)
        : AnimatedPlayer(x, y, "Nick", "assets/sonic.png")
    {
        lives = 50;
        speed = 3.0f;
        snowballPower = 1.0f;
        snowballDistance = 200.0f;
    }

    // Full reset (called by GamePlay::reset)
    void reset(float nx, float ny)
    {
        lives = 50;
        speed = 3.0f;
        snowballPower = 1.0f;
        snowballDistance = 200.0f;
        resetAnim(nx, ny, "assets/sonic.png");
    }
};