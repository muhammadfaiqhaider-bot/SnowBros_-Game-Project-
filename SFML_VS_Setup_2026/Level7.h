#pragma once
#include "levels.h"
#include "ColorVarients.h"

class Level7 : public Level
{
public:
    Level7()
    {
        loadBackground("assets/bg_level7.png");

        // Platforms
        addPlatform(0, 560, 600, 20);   // Ground
        addPlatform(0, 420, 250, 20);   // Mid left
        addPlatform(350, 420, 250, 20); // Mid right
        addPlatform(150, 280, 300, 20); // Upper mid
        addPlatform(0, 140, 200, 20);   // Top left
        addPlatform(400, 140, 200, 20); // Top right

        // Enemies - Level7: 4 GreenBotom, 3 GreenFlyingFoogo
        addEnemy(new GreenBotom(80.f, 520.f));
        addEnemy(new GreenBotom(220.f, 520.f));
        addEnemy(new GreenBotom(360.f, 520.f));
        addEnemy(new GreenBotom(500.f, 380.f));

        addEnemy(new GreenFlyingFoogo(200.f, 240.f));
        addEnemy(new GreenFlyingFoogo(320.f, 180.f));
        addEnemy(new GreenFlyingFoogo(430.f, 120.f));
    }
};
