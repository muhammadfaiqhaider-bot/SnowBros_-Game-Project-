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
        name = "GreenBotom";
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
        shape.setFillColor(sf::Color(0, 200, 0));
        shape.setPosition(x, y);
        window.draw(shape);
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
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
        shape.setFillColor(sf::Color(0, 0, 200));
        shape.setPosition(x, y);
        window.draw(shape);
    }
};

// During level 10 Botom powers increased like 
// 1- speed increase by 75% 
// 2- and also health increase means 3 more hit required to shot him down 
class PurpleBotom : public Botom
{
public:
    PurpleBotom(float posX, float posY) : Botom(posX, posY)
    {
        velocityX = velocityX * 1.75f;
        health = health + 3;
        name = "PurpleBotom";
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
        shape.setFillColor(sf::Color(128, 0, 128));
        shape.setPosition(x, y);
        window.draw(shape);
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
        velocityX = velocityX * 1.25f;  // Fixed - multiply not add
        health = health + 1;
        name = "GreenFlyingFoogo";
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (getIsFlying())
        {
            // Darker green when flying
            sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
            shape.setFillColor(sf::Color(0, 180, 0));
            shape.setPosition(x, y);
            window.draw(shape);
        }
        else
        {
            // Lighter green when walking
            sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
            shape.setFillColor(sf::Color(0, 230, 0));
            shape.setPosition(x, y);
            window.draw(shape);
        }
    }
};

// During level 8-9 FlyingFoogaFOOG powers increased like 
// 1- speed increase by 50% 
// 2- and also health increase means 2 more hit required to shot him down 
class BlueFlyingFoogo : public FlyingFoogaFoog
{
public:
    BlueFlyingFoogo(float posX, float posY) : FlyingFoogaFoog(posX, posY)
    {
        velocityX = velocityX * 1.50f;
        health = health + 2;
        name = "BlueFlyingFoogo";
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (getIsFlying())
        {
            sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
            shape.setFillColor(sf::Color(0, 0, 180));
            shape.setPosition(x, y);
            window.draw(shape);
        }
        else
        {
            sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
            shape.setFillColor(sf::Color(0, 0, 230));
            shape.setPosition(x, y);
            window.draw(shape);
        }
    }
};



// During level 10 FlyingFoogaFOOG powers increased like 
// 1- speed increase by 75% 
// 2- and also health increase means 3 more hit required to shot him down 
class PurpleFlyingFoogo : public FlyingFoogaFoog
{
public:
    PurpleFlyingFoogo(float posX, float posY) : FlyingFoogaFoog(posX, posY)
    {
        velocityX = velocityX * 1.75f;
        health = health + 3;
        name = "PurpleFlyingFoogo";
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        if (getIsFlying())
        {
            sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
            shape.setFillColor(sf::Color(100, 0, 150));
            shape.setPosition(x, y);
            window.draw(shape);
        }
        else
        {
            sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
            shape.setFillColor(sf::Color(150, 0, 200));
            shape.setPosition(x, y);
            window.draw(shape);
        }
    }
};







// Level 6-7: Speed +25%, Health +1, Faster knife
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
        sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
        shape.setFillColor(sf::Color(0, 200, 150));     // Teal green
        shape.setPosition(x, y);
        window.draw(shape);
    }
};



// Level 8-9: Speed +50%, Health +2
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
        sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
        shape.setFillColor(sf::Color(0, 150, 255));     // Light blue
        shape.setPosition(x, y);
        window.draw(shape);
    }
};


// Level 10: Speed +75%, Health +3
class PurpleTornado : public Tornado
{
public:
    PurpleTornado(float posX, float posY) : Tornado(posX, posY)
    {
        velocityX = velocityX * 1.75f;
        health = health + 3;
        name = "PurpleTornado";
    }

    void DisplayEnemy(sf::RenderWindow& window) override
    {
        sf::RectangleShape shape(sf::Vector2f(40.f, 40.f));
        shape.setFillColor(sf::Color(180, 0, 255));     // Purple
        shape.setPosition(x, y);
        window.draw(shape);
    }
};