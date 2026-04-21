#pragma once
#include "Botom.h"
#include "FlyingFoogo.h"


// During level 6-7 Botom powers increased like 
// 1- speed increase by 25% 
// 2- and also health increase means 1 more hit required to shot him down 
class GreenBotom : public Botom
{
private:

public:
	GreenBotom(float posX, float posY) :Botom(posX, posY)
	{
		velocityX = velocityX * 1.25f;              // Speed Increased by 25%
		health = health + 1;
		name = "Green Botom";
	}


	void DisplayEnemy(sf::RenderWindow& window)override
	{
		sf::RectangleShape shape(sf::Vector2f(40.0f, 40.0f));
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
private:

public:
	BlueBotom(float posX, float posY) :Botom(posX, posY)
	{
		name = "Blue Botom";
		velocityX = velocityX * 1.50f;
		health = health + 2;
	}


	void DisplayEnemy(sf::RenderWindow& window)override
	{
		sf::RectangleShape shape(sf::Vector2f(40.0f, 40.0f));
		shape.setFillColor(sf::Color::Blue);
		shape.setPosition(x, y);
		window.draw(shape);
	}

};










// During level 10 Botom powers increased like 
// 1- speed increase by 75% 
// 2- and also health increase means 3 more hit required to shot him down 
class PurpleBotom : public Botom
{
private:

public:
	PurpleBotom(float posX, float posY) :Botom(posX, posY)
	{
		name = "Purple Botom";
		velocityX = velocityX * 1.75f; 
		health = health + 3;
	}


	void DisplayEnemy(sf::RenderWindow& window)override
	{
		sf::RectangleShape shape(sf::Vector2f(40.0f, 40.0f));
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
private:

public:
	GreenFlyingFoogo(float posX, float posY) : FlyingFoogaFoog(posX, posY)
	{
		name = "Green Flying Fooga Foog";
		velocityX = velocityX + 1.25f;
		velocityY = velocityY + 1.25f;
		health = health + 1;
	}


	void DisplayEnemy(sf::RenderWindow& window)
	{
		sf::RectangleShape shape(sf::Vector2f(40.0f, 40.0f));
		shape.setFillColor(sf::Color::Green);
		shape.setPosition(x, y);
		window.draw(shape);
	}
};





class BlueFlyingFoogo : public FlyingFoogaFoog
{
private:

public:
	BlueFlyingFoogo(float posX, float posY) : FlyingFoogaFoog(posX, posY)
	{
		velocityX = velocityX * 1.50f;
		velocityY = velocityY * 1.50f;
		health = health + 2;
		name = "Blue Flying Fooga Foog";
	}

	void DisplayEnemy(sf::RenderWindow& window)
	{
		sf::RectangleShape shape(sf::Vector2f(40.0f, 40.0f));
		shape.setFillColor(sf::Color::Blue);
		shape.setPosition(x, y);
		window.draw(shape);
	}
};




// During level 10 FlyingFoogaFOOG powers increased like 
// 1- speed increase by 75% 
// 2- and also health increase means 3 more hit required to shot him down 
class PurpleFlyingFoogo : public FlyingFoogaFoog
{
private:

public:
	PurpleFlyingFoogo(float posX, float posY) : FlyingFoogaFoog(posX, posY)
	{
		velocityX = velocityX * 1.75f;
		velocityY = velocityY * 1.75f;
		health = health + 3;
		name = "Purple Flying Fooga Foog";
	}

	void DisplayEnemy(sf::RenderWindow& window)
	{
		sf::RectangleShape shape(sf::Vector2f(40.0f, 40.0f));
		shape.setFillColor(sf::Color(128, 0, 128));
		shape.setPosition(x, y);
		window.draw(shape);
	}
};




























