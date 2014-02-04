#include "Enemy.h"

Enemy::Enemy(ALLEGRO_BITMAP *image)
{
	GameObject::Init(500, 290, 1, 0, -1, 0, 16, 16);

	SetID(ENEMY);
	SetAlive(true);
	lives = 3;

	maxFrame = 143;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 2;
	frameWidth = 96;
	frameHeight = 96;
	animationColumns = 21;

	if( rand() % 2)
		animationDirection = 1;
	else
		animationDirection = -1;

	if(image != NULL)
		Enemy::image = image;
}

void Enemy::Destroy()
{
	GameObject::Destroy();
}

void Enemy::Update()
{
	GameObject::Update();

	if(++frameCount >= frameDelay)
	{
		curFrame += animationDirection;
		if(curFrame >= maxFrame)
			curFrame = 0;
		else if(curFrame <= 0)
			curFrame = maxFrame;

		frameCount = 0;
	}

	if(x + frameWidth < 0)
		Collided(BORDER);
}

void Enemy::Render()
{
	GameObject::Render();
	/*
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, 
		x - frameWidth / 2, y - frameHeight / 2, 0);*/
	//al_draw_bitmap(image, x, y, 0);

	al_draw_filled_circle(x, y, boundX, al_map_rgb(200, 0, 0));

}

void Enemy::Collided(int objectID)
{
	if(objectID != ENEMY)
		lives--;
}