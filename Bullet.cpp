#include "Bullet.h"

Bullet::Bullet(float x, float y, void(*ScorePoint)(void))
{
	GameObject::Init(x, y, 10, 0, 1, 0, 0, 0);

	SetID(BULLET);

	Bullet::ScorePoint = ScorePoint;
}

void Bullet::Destroy()
{
	GameObject::Destroy();
}

void Bullet::Update()
{
	GameObject::Update();

	if(x > WIDTH)
		Collided(BORDER);
}

void Bullet::Render()
{
	al_draw_filled_rounded_rectangle(x-2, y-1, x+4, y+2, 4, 2, al_map_rgb(255, 255, 255));
}

void Bullet::Collided(int objectID)
{
	if(objectID == ENEMY || objectID == BORDER)
		SetAlive(false);

	if(objectID == ENEMY)
		ScorePoint();
}