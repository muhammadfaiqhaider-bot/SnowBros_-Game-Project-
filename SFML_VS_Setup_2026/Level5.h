#pragma once
#include "levels.h"

class Level5 : public Level
{
public:
    Level5()
    {
        loadBackground("assets/bg_level3.png");
        loadPlatformTexture("assets/plat_03.png");
        setEnemyTint(sf::Color(255, 177, 153, 255));

        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(0, 390, 220, 32);   // Left mid
        addPlatform(380, 390, 220, 32);   // Right mid - big gap in middle
        addPlatform(180, 220, 240, 32);   // Upper mid - narrow

        addEnemy(new Botom(100.f, 520.f));
        addEnemy(new Botom(250.f, 520.f));
        addEnemy(new Botom(400.f, 520.f));
        addEnemy(new Botom(500.f, 350.f));
        addEnemy(new FlyingFoogaFoog(220.f, 180.f));
        addEnemy(new FlyingFoogaFoog(420.f, 120.f));
    }
};
