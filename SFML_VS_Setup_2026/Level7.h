#pragma once
#include "levels.h"
#include "ColorVarients.h"

class Level7 : public Level
{
public:
    Level7()
    {
        loadBackground("assets/bg_level4.jpg");
        loadPlatformTexture("assets/plat_04.png");

        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(0, 440, 160, 32);   // Left low
        addPlatform(440, 440, 160, 32);   // Right low
        addPlatform(200, 360, 200, 32);   // Center mid
        addPlatform(0, 240, 140, 32);   // Left upper
        addPlatform(460, 240, 140, 32);   // Right upper
        addPlatform(180, 130, 240, 32);   // Top center

        addEnemy(new GreenBotom(80.f, 520.f));
        addEnemy(new GreenBotom(220.f, 520.f));
        addEnemy(new GreenBotom(360.f, 520.f));
        addEnemy(new GreenBotom(500.f, 400.f));
        addEnemy(new GreenFlyingFoogo(200.f, 320.f));
        addEnemy(new GreenFlyingFoogo(320.f, 200.f));
        addEnemy(new GreenFlyingFoogo(430.f, 90.f));
    }
};
