#include "PickUp.h"

PickUp::PickUp(ALLEGRO_BITMAP *image)
{
	GameObject::Init(500, 0, 0, 0, 0, 0, 0, 0);

	PickUp::image = image;

	NumberOfPickUps = 0;
}

void PickUp::Destroy()
{
	GameObject::Destroy();
}

void PickUp::Render()
{
	GameObject::Render();

	for (int i = 0; i < NumberOfPickUps; i++)
	{
		al_draw_bitmap(image, x + (i * 30), y, 0);
	}
}