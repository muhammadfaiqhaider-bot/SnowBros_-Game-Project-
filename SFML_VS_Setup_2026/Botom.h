#pragma once

#include "Enemy.h"

class Botom :public Enemy
{
protected:
	bool hitWall;
public:
	Botom(float xPos, float yPos) : Enemy(xPos, yPos, "Botom")
	{
		health = 1;
		hitWall = false;
		velocityX = rand() % 3 - 1;
		if (velocityX == 0)
		{
			velocityX = 1;
		}
		velocityY = 0;

	}


	void movementsUpdate() override
	{
		if (hitWall)
		{
			velocityX = -velocityX;

			if (velocityX == 0)
			{
				velocityX = 1;
			}
			hitWall = false;
		}

		x += velocityX;
	}

	void DisplayEnemy()override
	{
		//Display krana hai enemy koo Cheema
	}



	//Note: Cheema ab is mei U have to make 1 more function  as well something like checkCollision 
	// In which when the position of enemy and some hard object like wall coolide soo hitwall gets truee and direction change 
	// logic is already up there.....



};