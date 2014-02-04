#include "Projectile.h"

Projectile::Projectile(float x, float y, ALLEGRO_BITMAP *image)
{
	GameObject::Init(x, y, 3, 0, 1, 0, 0, 0);

	SetID(PROJECTILE);

	if(image != NULL)
		Projectile::image = image;
}

void Projectile::Destroy()
{
	GameObject::Destroy();
}

void Projectile::Update()
{
	GameObject::Update();

	if(x > WIDTH)
		Collided(BORDER);
}

void Projectile::Render()
{
	//al_draw_filled_rounded_rectangle(x-2, y-1, x+4, y+2, 4, 2, al_map_rgb(0, 0, 0));
	al_draw_bitmap(image, x, y - 15, 0);
}

void Projectile::Collided(int objectID)
{
	if(objectID == ENEMY || objectID == BORDER)
		SetAlive(false);	
}