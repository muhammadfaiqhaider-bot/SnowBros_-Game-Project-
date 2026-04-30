#pragma once
#include "levels.h"
#include "Tornado.h"

class Level4 : public Level
{
public:
    Level4()
    {
        loadBackground("assets/bg_level4.png");

        // Platforms
        addPlatform(0, 560, 600, 20);   // Ground
        addPlatform(0, 420, 250, 20);   // Mid left
        addPlatform(350, 420, 250, 20); // Mid right
        addPlatform(150, 280, 300, 20); // Upper mid
        addPlatform(0, 140, 200, 20);   // Top left
        addPlatform(400, 140, 200, 20); // Top right

        // Enemies - Level4 = 3 Botoms + 2 FlyingFooga + 2 Tornado
        addEnemy(new Botom(120.f, 520.f));
        addEnemy(new Botom(300.f, 520.f));
        addEnemy(new Botom(480.f, 380.f));

        addEnemy(new FlyingFoogaFoog(200.f, 200.f));
        addEnemy(new FlyingFoogaFoog(380.f, 100.f));

        addEnemy(new Tornado(220.f, 100.f));
        addEnemy(new Tornado(460.f, 60.f));
    }
};
