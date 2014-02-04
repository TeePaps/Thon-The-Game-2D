#include "Person.h"

Person::Person()
{}

void Person::Destroy()
{
	GameObject::Destroy();
}

void Person::Init(ALLEGRO_BITMAP *image)
{
	GameObject::Init(200, 200, 0, 10, 0, 0, 16, 16);

	SetID(PLAYER);
	SetAlive(true);

	lives = 2;
	score = 0;
	groundLevel= HEIGHT - 10;
	surfaceLevel = groundLevel;

	maxFrame = 10;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 5;
	frameWidth = 35;
	frameHeight = 50;
	animationColumns = 10;
	animationDirection = 1;

	startWidthOfWalkingFrame = 45;
	startHeightOfWalkingFrame = 20;

	animationRow = 0;

	leftKey = false;
	rightKey = false;

	if(image != NULL)
		Person::image = image;
}


void Person::Update()
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

	if(x < 0)
		x = 0;
	else if(x > WIDTH)
		x = WIDTH;

	if(y < 0)
		y = 0;
	if(y > surfaceLevel)
		y = surfaceLevel;
}

void Person::Render()
{
	GameObject::Render();

	int fx, fy;

	if(leftKey || rightKey)
	{
		fx = startWidthOfWalkingFrame + (curFrame % animationColumns) * frameWidth;
		fy = startHeightOfWalkingFrame + animationRow * frameHeight;
	}
	else
	{
		fx = 0;
		fy = 20;
	}

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight,
		x - frameWidth / 2, y - frameHeight / 2, leftKey);			//Left key acts as flag to invert image when needed
}

void Person::ResetAnimation(int position)
{
	if(position == 1)
	{
		if (OffGround() && BlockType() == 0)
		{
			Gravity();
		}
		else if (OffGround() && BlockType() == 1)
		{
			surfaceLevel = y - boundY;
			velX = 0;
		}
		else
		{
			dirY = 0;
		}
	}
	else
	{
		curFrame = 0;

		rightKey = false;
		leftKey = false;

		if (y == surfaceLevel && BlockType() == 1)
		{
			velX = 0;
			dirX = 0;
			x -= 16;
		}
		else if (BlockType() == 0)
		{
			velX = 0;
		}
	}
}

void Person::Jump()
{
	if (dirY == 0)
	{
		velY = 10;
		dirY = -1;
	}

	else ResetAnimation(1);
}
void Person::MoveLeft()
{
	leftKey = true;

	if (BlockType() == 0)
		dirX = -1;
}
void Person::MoveRight()
{
	rightKey = true;

	animationRow = 0;
	if (BlockType() == 0)
		dirX = 1;
}

void Person::Collided(int objectID)
{
	if(objectID == ENEMY)
	{
		if ((dirY == 0) || (dirY == -1))
			lives--;
	}
	if(BlockType() == 1)
	{
		surfaceLevel = y;
	}
}

int Person::BlockType() 
{
	if (dirX == 1 && (dirY == 0 || dirY == 1))
		return bottomRightBlock->user1;
	else if (dirX == -1 && (dirY == 0 || dirY == 1))
		return bottomLeftBlock->user1;
	else if (dirX == 1 && dirY == -1)
		return topRightBlock->user1;
	else if (dirX == -1 && dirY == -1)
		return topLeftBlock->user1;
	else
		return bottomRightBlock->user1;
}

std::list<BLKSTR *> Person::GetCornerBlocks()
{
	cornerBlocks.clear();	// resets block for each frame

	cornerBlocks.push_back(topLeftBlock);
	cornerBlocks.push_back(topRightBlock);
	cornerBlocks.push_back(bottomLeftBlock);
	cornerBlocks.push_back(bottomRightBlock);

	return cornerBlocks;
}

void Person::Gravity()
{
	surfaceLevel = HEIGHT - boundY;	

	if (velY != 0 && dirY == -1)
		velY--;
	else
	{
		dirY = 1;
		velY++;
	}
}