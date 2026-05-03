#pragma once
#include "levels.h"
#include "Tornado.h"

class Level4 : public Level
{
public:
    Level4()
    
    {
        loadBackground("assets/bg_level2.png");
        loadPlatformTexture("assets/plat_02.png");

        // Platforms
        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(0, 440, 180, 32);   // Left low
        addPlatform(200, 360, 180, 32);   // Mid stair
        addPlatform(400, 280, 180, 32);   // Right stair
        addPlatform(150, 180, 200, 32);   // Top mid
        loadLevelMusic("assets/level_music_4.wav");

        // Enemies - Level4 = 3 Botoms + 2 FlyingFooga + 2 Tornado
        addEnemy(new Botom(120.f, 520.f));
        addEnemy(new Botom(300.f, 520.f));
        addEnemy(new Botom(480.f, 240.f));
        addEnemy(new FlyingFoogaFoog(200.f, 140.f));
        addEnemy(new FlyingFoogaFoog(380.f, 140.f));
        addEnemy(new Tornado(220.f, 320.f));
        addEnemy(new Tornado(460.f, 240.f));
        // spawn 4 stars on random platforms in this level (project requirement)
        for (int i = 0; i < 4; ++i) spawnStarOnRandomPlatform();
    }

    // Star feature removed
};
