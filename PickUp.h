#pragma once

#include "GameObject.h"

class PickUp : public GameObject
{
private:
	int NumberOfPickUps;

public:
	PickUp(ALLEGRO_BITMAP *image);
	void Destroy();

	void PickUp::AddItem() {NumberOfPickUps++;}

	void Render();
};