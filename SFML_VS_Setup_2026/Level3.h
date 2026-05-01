#pragma once
#include "levels.h"
#include "Tornado.h"

class Level3 : public Level
{
public:
    Level3()
    {
        loadBackground("assets/bg_level2.png");
        loadPlatformTexture("assets/plat_02.png");

        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(0, 400, 200, 32);   // Left only - no right mid
        addPlatform(300, 340, 150, 32);   // Small right mid - lower
        addPlatform(100, 210, 260, 32);   // Upper mid - shifted left
        addPlatform(380, 140, 220, 32);   // Top right - wide

        addEnemy(new Botom(120.f, 520.f));
        addEnemy(new Botom(300.f, 520.f));
        addEnemy(new Botom(480.f, 300.f));
        addEnemy(new FlyingFoogaFoog(260.f, 170.f));
        addEnemy(new Tornado(420.f, 100.f));
    }
};
