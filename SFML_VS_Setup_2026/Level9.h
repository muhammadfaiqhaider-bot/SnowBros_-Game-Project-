#pragma once
#include "levels.h"
#include "ColorVarients.h"

class Level9 : public Level
{
public:
    Level9()
    {
        loadBackground("assets/bg_level6.png");
        setEnemyTint(sf::Color(144, 120, 161, 255));

        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(0, 460, 130, 32);   // Far left low
        addPlatform(470, 460, 130, 32);   // Far right low
        addPlatform(130, 380, 130, 32);   // Left mid
        addPlatform(340, 380, 130, 32);   // Right mid
        addPlatform(220, 280, 160, 32);   // Center
        addPlatform(0, 180, 140, 32);   // Top left
        addPlatform(460, 180, 140, 32);   // Top right

        addEnemy(new PurpleBotom(120.f, 520.f));
        addEnemy(new PurpleBotom(300.f, 520.f));
        addEnemy(new PurpleBotom(480.f, 420.f));
        addEnemy(new PurpleFlyingFoogo(200.f, 340.f));
        addEnemy(new PurpleFlyingFoogo(320.f, 240.f));
        addEnemy(new PurpleFlyingFoogo(420.f, 140.f));
        addEnemy(new PurpleTornado(220.f, 90.f));
        addEnemy(new PurpleTornado(460.f, 90.f));
    }
};
