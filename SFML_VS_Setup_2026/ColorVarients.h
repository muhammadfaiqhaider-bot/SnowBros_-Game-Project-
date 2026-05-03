#pragma once
#include "Botom.h"
#include "FlyingFoogo.h"
#include "Tornado.h"


// During level 6-7 Botom powers increased like 
// 1- speed increase by 25% 
// 2- and also health increase means 1 more hit required to shot him down 
class GreenBotom : public Botom
{
public:
    GreenBotom(float posX, float posY) : Botom(posX, posY)
    {
        velocityX = velocityX * 1.25f;
        health = health + 1;
        hitCount = 0;
        name = "GreenBotom";

        // Override texture with orange sprite sheet
        // Everything else (animation logic, frame sizes) inherited from Botom
        if (enemyTexture.loadFromFile("assets/Botom_Orange.png"))
        {
            textureLoaded = true;
            enemySprite.setTexture(enemyTexture);
            enemySprite.setTextureRect(sf::IntRect(0, 136, 88, 88));
            enemySprite.setScale(40.f / 88.f, 40.f / 88.f);
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        // All snow/rolling/encasing display inherited from Botom
        // Just call parent's DisplayEnemy
        Botom::DisplayEnemy(window);
    }
};


// During level 8-9 Botom powers increased like 
// 1- speed increase by 50% 
// 2- and also health increase means 2 more hit required to shot him down 
class BlueBotom : public Botom
{
public:
    BlueBotom(float posX, float posY) : Botom(posX, posY)
    {
        velocityX = velocityX * 1.50f;
        health = health + 2;
        name = "BlueBotom";

        // Override with Pink sprite sheet
        if (enemyTexture.loadFromFile("assets/Botom_Pink.png"))
        {
            textureLoaded = true;
            enemySprite.setTexture(enemyTexture);
            enemySprite.setTextureRect(sf::IntRect(0, 136, 88, 88));
            enemySprite.setScale(40.f / 88.f, 40.f / 88.f);
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        Botom::DisplayEnemy(window);    // All animation inherited from Botom
    }
};



// During level 6-7 FlyingFoogoFOOG powers increased like 
// 1- speed increase by 25% 
// 2- and also health increase means 1 more hit required to shot him down 
class GreenFlyingFoogo : public FlyingFoogaFoog
{
public:
    GreenFlyingFoogo(float posX, float posY) : FlyingFoogaFoog(posX, posY)
    {
        velocityX = velocityX * 1.25f;
        health = health + 1;
        name = "GreenFlyingFoogo";

        // Override with Orange sprite sheet
        if (foogaTexture.loadFromFile("assets/FlyingFoogaFoog_Orange.png"))
        {
            foogaTextureLoaded = true;
            foogaSprite.setTexture(foogaTexture);
            foogaSprite.setTextureRect(sf::IntRect(WALK_X, WALK_Y, WALK_W, WALK_H));
            foogaSprite.setScale(40.f / WALK_W, 40.f / WALK_H);
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        FlyingFoogaFoog::DisplayEnemy(window);  // All animation inherited
    }
};

class BlueFlyingFoogo : public FlyingFoogaFoog
{
public:
    BlueFlyingFoogo(float posX, float posY) : FlyingFoogaFoog(posX, posY)
    {
        velocityX = velocityX * 1.50f;
        health = health + 2;
        name = "BlueFlyingFoogo";

        // Override with Pink sprite sheet
        if (foogaTexture.loadFromFile("assets/FlyingFoogaFoog_Red.png"))
        {
            foogaTextureLoaded = true;
            foogaSprite.setTexture(foogaTexture);
            foogaSprite.setTextureRect(sf::IntRect(WALK_X, WALK_Y, WALK_W, WALK_H));
            foogaSprite.setScale(40.f / WALK_W, 40.f / WALK_H);
        }
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        FlyingFoogaFoog::DisplayEnemy(window);  // All animation inherited
    }
};











// Level 6-7: Speed +25%, Health +1
class GreenTornado : public Tornado
{
public:
    GreenTornado(float posX, float posY) : Tornado(posX, posY)
    {
        velocityX = velocityX * 1.25f;
        health = health + 1;
        name = "GreenTornado";

    
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        Tornado::DisplayEnemy(window);  // All animation + knife + snow inherited
    }
};

// Level 7-8: Speed +50%, Health +2
class BlueTornado : public Tornado
{
public:
    BlueTornado(float posX, float posY) : Tornado(posX, posY)
    {
        velocityX = velocityX * 1.50f;
        health = health + 2;
        name = "BlueTornado";

    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        Tornado::DisplayEnemy(window);  // All animation + knife + snow inherited
    }
};



