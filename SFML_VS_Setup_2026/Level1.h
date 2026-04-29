#pragma once
#include "levels.h"

class Level1 : public Level
{
public:
    Level1()
    {
        loadBackground("assets/bg_level1.png");

        // Platforms
        addPlatform(0, 560, 600, 20);   // Ground
        addPlatform(0, 420, 250, 20);   // Mid left
        addPlatform(350, 420, 250, 20);   // Mid right
        addPlatform(150, 280, 300, 20);   // Upper mid
        addPlatform(0, 140, 200, 20);   // Top left
        addPlatform(400, 140, 200, 20);   // Top right

        // Enemies
        // Level adjustment: 4 Botoms
        addEnemy(new Botom(100.f, 520.f));
        addEnemy(new Botom(250.f, 520.f));
        addEnemy(new Botom(400.f, 520.f));
        addEnemy(new Botom(500.f, 380.f));
    }
};