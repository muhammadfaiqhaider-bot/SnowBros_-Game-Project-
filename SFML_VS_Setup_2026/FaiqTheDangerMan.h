#pragma once
#include "AnimatedPlayer.h"

class Faiq : public AnimatedPlayer
{
public:
    Faiq(float x, float y)
        : AnimatedPlayer(x, y, "Faiq", "assets/faiqaling.png")
    {
        lives = 4;       // one fewer life than Nick
        speed = 4.7f;    // faster
        snowballPower = 1.0f;
        snowballDistance = 200.0f;
    }

    void reset(float nx, float ny)
    {
        lives = 4;
        speed = 4.7f;
        snowballPower = 1.0f;
        snowballDistance = 200.0f;
        resetAnim(nx, ny, "assets/faiqaling.png");
    }
};