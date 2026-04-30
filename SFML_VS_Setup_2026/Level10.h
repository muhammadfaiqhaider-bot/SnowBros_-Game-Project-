#pragma once
#include "levels.h"
#include "ColorVarients.h"
#include "Gamakichi.h"

class Level10 : public Level
{
public:
    Level10()
    {
        loadBackground("assets/bg_boss2.png");

        // Platforms
        addPlatform(0, 560, 600, 20);   // Ground
        addPlatform(0, 420, 250, 20);   // Mid left
        addPlatform(350, 420, 250, 20); // Mid right
        addPlatform(150, 280, 300, 20); // Upper mid
        addPlatform(0, 140, 200, 20);   // Top left
        addPlatform(400, 140, 200, 20); // Top right

        // Enemies - Level10: 3 PurpleBotom, 3 PurpleFlyingFoogo, 3 PurpleTornado + Gamakichi boss top-right
        addEnemy(new PurpleBotom(100.f, 520.f));
        addEnemy(new PurpleBotom(300.f, 520.f));
        addEnemy(new PurpleBotom(500.f, 520.f));

        addEnemy(new PurpleFlyingFoogo(180.f, 240.f));
        addEnemy(new PurpleFlyingFoogo(320.f, 180.f));
        addEnemy(new PurpleFlyingFoogo(460.f, 120.f));

        addEnemy(new PurpleTornado(220.f, 100.f));
        addEnemy(new PurpleTornado(340.f, 60.f));
        addEnemy(new PurpleTornado(460.f, 100.f));

        addEnemy(new Gamakichi(420.f, 20.f));
    }
};
