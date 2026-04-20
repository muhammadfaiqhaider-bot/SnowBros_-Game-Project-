#pragma once

#include "Botom.h"
#include <cstdlib>

class FlyingFoogaFoog : public Botom
{
private:
	bool isFlying;

	int flyingTimer;
	int flyingDuration;
	int walkingTimer;
	int walkingDuration;

public:
	FlyingFoogaFoog(float xPos, float yPos) : Botom(xPos, yPos)
	{
		isFlying = false;
		flyingTimer = 0;
		flyingDuration = 6;
		walkingTimer = 0;
		walkingDuration = 6;
	}

	void movementsUpdate()override
	{
		if (isFlying)
		{
			x = x + velocityX; // (x is a poistion and velocity tells where to move per pixel)
			y = y + velocityY;

			flyingTimer++;

			if (flyingTimer > flyingDuration)
			{
				isFlying = false;
				flyingTimer = 0;
			}

		}
		else
		{
			Botom::movementsUpdate(); // Reusing mvements of botom bcz have the same ground movemnets attributes

			walkingTimer++;
			if (walkingTimer > walkingDuration)
			{
				isFlying = true;
				walkingTimer = 0;

				velocityX = (rand() % 3) - 1;
				velocityY = (rand() % 3) - 1;

				if (velocityX == 0 && velocityY == 0)         // Check if coordinates gets 0 so make it 1 to keep flying continue
				{
					velocityX == 1;
					velocityY == 1;
				}
			}
		}		

	}
	void DisplayEnemy() override
	{
		// Cheema Sfml display logic here
	}
};