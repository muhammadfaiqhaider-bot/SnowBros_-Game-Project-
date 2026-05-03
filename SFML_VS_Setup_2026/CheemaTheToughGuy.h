#pragma once
#include "AnimatedPlayer.h"

class Cheema : public AnimatedPlayer
{
public:
    Cheema(float x, float y)
        : AnimatedPlayer(x, y, "Cheema", "assets/cheemaling.png")
    {
        lives = 3;       // fewer lives, offset by range
        speed = 2.8f;    // slightly slower
        snowballPower = 1.5f;    // hits harder
        snowballDistance = 350.0f;  // long range
    }

    void reset(float nx, float ny)
    {
        lives = 3;
        speed = 2.8f;
        snowballPower = 1.5f;
        snowballDistance = 350.0f;
        resetAnim(nx, ny, "assets/cheemaling.png");
    }
};