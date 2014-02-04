#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
private:
	int lives;

public:
	Enemy(ALLEGRO_BITMAP *image);
	void Destroy();

	void Update();
	void Render();

	int GetLives() {return lives;}

	void Collided(int objectID);
};