#pragma once
#include "levels.h"

class Level2 : public Level
{
public:
    Level2()
    {
        loadBackground("assets/bg_level2.png");

        // Platforms
        addPlatform(0, 560, 600, 20);
        addPlatform(0, 420, 250, 20);
        addPlatform(350, 420, 250, 20);
        addPlatform(150, 280, 300, 20);
        addPlatform(0, 140, 200, 20);
        addPlatform(400, 140, 200, 20);

        // Enemies
        addEnemy(new Botom(100.f, 520.f));
        addEnemy(new Botom(250.f, 520.f));
        addEnemy(new Botom(400.f, 520.f));
        addEnemy(new Botom(150.f, 380.f));
        addEnemy(new Botom(450.f, 380.f));
        addEnemy(new FlyingFoogaFoog(200.f, 240.f));
        addEnemy(new FlyingFoogaFoog(400.f, 100.f));
        addEnemy(new FlyingFoogaFoog(100.f, 100.f));
    }
};