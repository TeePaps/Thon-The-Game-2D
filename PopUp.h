#pragma once

#include "GameObject.h"

class PopUp : public GameObject
{
private:

public:
	PopUp(ALLEGRO_BITMAP *image);
	void Destroy();

	void Render();
};