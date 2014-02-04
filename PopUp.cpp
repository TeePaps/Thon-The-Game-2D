#include "PopUp.h"

PopUp::PopUp(ALLEGRO_BITMAP *image)
{
	GameObject::Init(70, 35, 0, 0, 0, 0, 0, 0);

	PopUp::image = image;
}

void PopUp::Destroy()
{
	GameObject::Destroy();
}

void PopUp::Render()
{
	GameObject::Render();

	al_draw_bitmap(image, x, y, 0);
}