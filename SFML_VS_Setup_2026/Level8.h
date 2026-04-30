#pragma once
#include "levels.h"
#include "ColorVarients.h"

class Level8 : public Level
{
public:
    Level8()
    {
        loadBackground("assets/bg_level8.png");

        // Platforms
        addPlatform(0, 560, 600, 20);   // Ground
        addPlatform(0, 420, 250, 20);   // Mid left
        addPlatform(350, 420, 250, 20); // Mid right
        addPlatform(150, 280, 300, 20); // Upper mid
        addPlatform(0, 140, 200, 20);   // Top left
        addPlatform(400, 140, 200, 20); // Top right

        // Enemies - Level8: 4 BlueBotom, 2 BlueFlyingFoogo, 1 BlueTornado
        addEnemy(new BlueBotom(80.f, 520.f));
        addEnemy(new BlueBotom(220.f, 520.f));
        addEnemy(new BlueBotom(360.f, 520.f));
        addEnemy(new BlueBotom(500.f, 380.f));

        addEnemy(new BlueFlyingFoogo(200.f, 240.f));
        addEnemy(new BlueFlyingFoogo(320.f, 180.f));

        addEnemy(new BlueTornado(420.f, 100.f));
    }
};
