#pragma once
#include "levels.h"
#include "ColorVarients.h"


class Level6 : public Level
{
public:
    Level6()
    {
        loadBackground("assets/bg_level3.png");
        loadPlatformTexture("assets/plat_03.png");
        setEnemyTint(sf::Color(255, 177, 153, 255)); //The shyt that makes sprites a bit darker in places with darker backgrounds
        //SetTint is a virt func in enemy, override it in other monsters to set their sprite to an sf::color as i did in fooga

        // Platforms
        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(50, 400, 160, 32);   // Left small
        addPlatform(390, 400, 160, 32);   // Right small - large gap
        addPlatform(200, 230, 200, 32);   // Upper mid - narrow

        // Enemies - Level6: Boss Mogera (top right) + 4 Botoms + 3 FlyingFooga
        addEnemy(new Botom(100.f, 520.f));
        addEnemy(new GreenBotom(220.f, 520.f));
        addEnemy(new Botom(360.f, 520.f));
        addEnemy(new GreenBotom(480.f, 360.f));
        addEnemy(new FlyingFoogaFoog(200.f, 190.f));
        addEnemy(new FlyingFoogaFoog(340.f, 100.f));
        addEnemy(new FlyingFoogaFoog(480.f, 200.f));

        // Level-specific music
        loadLevelMusic("assets/level_music_1.wav");

    }
};

