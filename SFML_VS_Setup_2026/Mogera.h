#pragma once
#include "Enemy.h"
#include "MogeraChild.h"

class Mogera : public Enemy
{
private:
	int health;

	float spawnTimer;
	float childSpawnInterval;

	static const int numberOfChilds = 10;
	MogeraChild child[numberOfChilds];


public:
	Mogera(float posX, float posY) : Enemy(posX, posY, "Mogera")
	{
		x = posX;
		y = posY;
		health = 10;
		spawnTimer = 0;
		childSpawnInterval = 90;
	}

	void movementsUpdate()override
	{
		spawnTimer++;
		if (spawnTimer >= childSpawnInterval)
		{
			for (int i = 0;i < numberOfChilds; i++)
			{
				if (!child[i].isActive())
				{
					float playerx = 50;
					child[i].spawn(x, y, playerx);
					break;
				}
			}
			spawnTimer = 0;
		}

		for (int i = 0; i < numberOfChilds; i++)
		{
			child[i].movementsUpdate();
		}


	}

	






	void DisplayEnemy() override
	{
		// Cheema Mogera Draw here 



		// Draw children
		for (int i = 0; i < numberOfChilds; i++)
		{
			child[i].DisplayEnemy();
		}
	}




	void reduceHealth()override
	{
		health--;
		if (health <= 0)
		{
			// Boss defeated now powerups and all that shyts......
		}
	}




};