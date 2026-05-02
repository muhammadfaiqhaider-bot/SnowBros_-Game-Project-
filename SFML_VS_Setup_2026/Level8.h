#pragma once
#include "levels.h"
#include "ColorVarients.h"

class Level8 : public Level
{
public:
    Level8()
    {
        loadBackground("assets/bg_level4.jpg");
        loadPlatformTexture("assets/plat_04.png");

        // Platforms
        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(0, 450, 140, 32);   // Far left low
        addPlatform(460, 450, 140, 32);   // Far right low
        addPlatform(150, 370, 140, 32);   // Left mid
        addPlatform(310, 370, 140, 32);   // Right mid
        addPlatform(0, 220, 160, 32);   // Top left
        addPlatform(440, 220, 160, 32);   // Top right

        // Enemies - Level8: 4 BlueBotom, 2 BlueFlyingFoogo, 1 BlueTornado
        addEnemy(new BlueBotom(80.f, 520.f));
        addEnemy(new BlueBotom(220.f, 520.f));
        addEnemy(new BlueBotom(360.f, 520.f));
        addEnemy(new BlueBotom(500.f, 380.f));

        addEnemy(new BlueBotom(500.f, 410.f));
        addEnemy(new BlueFlyingFoogo(200.f, 330.f));

        addEnemy(new BlueTornado(420.f, 90.f));

        // Level-specific music
        loadLevelMusic("assets/level_music_8.ogg");
    }
};
