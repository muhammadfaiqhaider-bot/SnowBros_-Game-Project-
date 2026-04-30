#pragma once
#include "levels.h"
#include "Mogera.h"

class Level6 : public Level
{
public:
    Level6()
    {
        loadBackground("assets/bg_level6.png");

        // Platforms
        addPlatform(0, 560, 600, 20);   // Ground
        addPlatform(0, 420, 250, 20);   // Mid left
        addPlatform(350, 420, 250, 20); // Mid rightda
        addPlatform(150, 280, 300, 20); // Upper mid
        addPlatform(0, 140, 200, 20);   // Top left
        addPlatform(400, 140, 200, 20); // Top right

        // Enemies - Level6: Boss Mogera (top right) + 4 Botoms + 3 FlyingFooga
        addEnemy(new Botom(100.f, 520.f));
        addEnemy(new Botom(220.f, 520.f));
        addEnemy(new Botom(360.f, 520.f));
        addEnemy(new Botom(480.f, 380.f));

        addEnemy(new FlyingFoogaFoog(200.f, 240.f));
        addEnemy(new FlyingFoogaFoog(340.f, 100.f));
        addEnemy(new FlyingFoogaFoog(480.f, 200.f));

        // Boss on top-right platform
        addEnemy(new Mogera(420.f, 20.f));
    }
};

