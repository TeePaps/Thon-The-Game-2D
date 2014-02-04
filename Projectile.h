#pragma once

#include "GameObject.h"

class Projectile : public GameObject
{
private:


public:
	Projectile(float x, float y, ALLEGRO_BITMAP *image);
	void Destroy();

	void Update();
	void Render();

	void Collided(int objectID);
};