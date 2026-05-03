#pragma once
#include "levels.h"
#include "ColorVarients.h"
#include "Gamakichi.h"

class Level10 : public Level
{
public:
    Level10()
    {
        loadBackground("assets/bg_level6.png");
        loadPlatformTexture("assets/plat_05.png");
        setEnemyTint(sf::Color(144, 120, 161, 255));

        // Platforms
        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(0, 470, 120, 32);   // Far left low
        addPlatform(480, 470, 120, 32);   // Far right low
        addPlatform(120, 390, 120, 32);   // Left mid low
        addPlatform(360, 390, 120, 32);   // Right mid low
        addPlatform(230, 310, 140, 32);   // Center mid
        addPlatform(0, 210, 130, 32);   // Left upper
        addPlatform(470, 210, 130, 32);   // Right upper
        addPlatform(200, 120, 200, 32);   // Top center

        // Enemies - Level10: 3 PurpleBotom, 3 PurpleFlyingFoogo, 3 PurpleTornado + Gamakichi boss top-right
        addEnemy(new BlueFlyingFoogo(180.f, 350.f));
        addEnemy(new BlueFlyingFoogo(320.f, 270.f));
        addEnemy(new BlueFlyingFoogo(460.f, 170.f));
        addEnemy(new BlueTornado(220.f, 80.f));
        addEnemy(new BlueTornado(340.f, 80.f));
        addEnemy(new BlueTornado(460.f, 80.f));
        addEnemy(new Gamakichi(260.f, 80.f));

        // Level-specific music
        loadLevelMusic("assets/level_music_10.ogg");
    } //Its better if you change this layout faiq -Cheema
};
