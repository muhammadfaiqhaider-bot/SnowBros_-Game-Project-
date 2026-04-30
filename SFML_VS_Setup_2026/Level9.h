#pragma once
#include "levels.h"
#include "ColorVarients.h"

class Level9 : public Level
{
public:
    Level9()
    {
        loadBackground("assets/bg_level9.png");

        // Platforms
        addPlatform(0, 560, 600, 20);   // Ground
        addPlatform(0, 420, 250, 20);   // Mid left
        addPlatform(350, 420, 250, 20); // Mid right
        addPlatform(150, 280, 300, 20); // Upper mid
        addPlatform(0, 140, 200, 20);   // Top left
        addPlatform(400, 140, 200, 20); // Top right

        // Enemies - Level9: 3 PurpleBotom, 3 PurpleFlyingFoogo, 2 PurpleTornado
        addEnemy(new PurpleBotom(120.f, 520.f));
        addEnemy(new PurpleBotom(300.f, 520.f));
        addEnemy(new PurpleBotom(480.f, 380.f));

        addEnemy(new PurpleFlyingFoogo(200.f, 240.f));
        addEnemy(new PurpleFlyingFoogo(320.f, 180.f));
        addEnemy(new PurpleFlyingFoogo(420.f, 120.f));

        addEnemy(new PurpleTornado(220.f, 100.f));
        addEnemy(new PurpleTornado(460.f, 60.f));
    }
};
