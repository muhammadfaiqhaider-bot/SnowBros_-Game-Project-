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
        setEnemyTint(sf::Color(144, 120, 161, 255));

        addPlatform(0, 560, 600, 32);   // Ground
        addPlatform(0, 470, 120, 32);   // Far left low
        addPlatform(480, 470, 120, 32);   // Far right low
        addPlatform(120, 390, 120, 32);   // Left mid low
        addPlatform(360, 390, 120, 32);   // Right mid low
        addPlatform(230, 310, 140, 32);   // Center mid
        addPlatform(0, 210, 130, 32);   // Left upper
        addPlatform(470, 210, 130, 32);   // Right upper
        addPlatform(200, 120, 200, 32);   // Top center

        addEnemy(new PurpleBotom(100.f, 520.f));
        addEnemy(new PurpleBotom(300.f, 520.f));
        addEnemy(new PurpleBotom(500.f, 520.f));
        addEnemy(new PurpleFlyingFoogo(180.f, 350.f));
        addEnemy(new PurpleFlyingFoogo(320.f, 270.f));
        addEnemy(new PurpleFlyingFoogo(460.f, 170.f));
        addEnemy(new PurpleTornado(220.f, 80.f));
        addEnemy(new PurpleTornado(340.f, 80.f));
        addEnemy(new PurpleTornado(460.f, 80.f));
        addEnemy(new Gamakichi(260.f, 80.f));
    } //Its better if you change this layout faiq -Cheema
};
