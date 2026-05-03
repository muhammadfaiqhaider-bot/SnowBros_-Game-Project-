#pragma once
#include "levels.h"

class Level2 : public Level
{
public:
    Level2()
    {
        loadBackground("assets/bg_level1.png");
        loadPlatformTexture("assets/plat_01.png");

        // Platforms
        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(50, 430, 180, 32);   // Mid left - shorter
        addPlatform(370, 430, 180, 32);   // Mid right - shorter, bigger gap
        addPlatform(180, 300, 240, 32);   // Upper mid - narrower
        addPlatform(0, 160, 160, 32);   // Top left - shorter
        addPlatform(440, 160, 160, 32);   // Top right - shifted right

        // Enemies
        addEnemy(new Botom(100.f, 220.f));
        addEnemy(new Botom(250.f, 220.f));
        addEnemy(new Botom(400.f, 220.f));
        addEnemy(new Botom(150.f, 190.f));
        addEnemy(new Botom(450.f, 390.f));
        addEnemy(new FlyingFoogaFoog(200.f, 240.f));
        addEnemy(new FlyingFoogaFoog(400.f, 100.f));
        addEnemy(new FlyingFoogaFoog(100.f, 100.f));

        // Level-specific music
        loadLevelMusic("assets/level_music_2.wav");
    }
};