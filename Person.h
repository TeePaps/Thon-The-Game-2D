#pragma once

#include "GameObject.h"
#include "mappy_A5.h"
#include <list>
#include <string>

class Person : public GameObject
{
private:
	std::string Name;
	int lives;
	int score;
	int surfaceLevel;
	int groundLevel;

	int animationRow;
	int startHeightOfWalkingFrame;
	int startWidthOfWalkingFrame;

	bool leftKey;
	bool rightKey;

public:
	BLKSTR *topLeftBlock , *topRightBlock, 
		*bottomLeftBlock, *bottomRightBlock;
	std::list<BLKSTR*> cornerBlocks;

	Person();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image = NULL);
	void Update();
	void Render();

	void Jump();
	void MoveLeft();
	void MoveRight();
	void ResetAnimation(int position);

	void ReadName(std::string name) {Name = name;}
	std::string GetName() {return Name;}
	int GetLives() {return lives;}
	int GetScore() {return score;}

	void LoseLife() {lives--;}
	void AddPoint() {score += 10000;}

	void Collided(int objectID);

	int BlockType();
	
	void Gravity();
	bool OffGround() {return y < surfaceLevel;}
	//bool OffSurface() {return y < surfaceLevel;}

	int GetRightOfBoundX() {return x + boundX / 2;}
	int GetBottomOfBoundY() {return y + boundY / 2;}

	std::list<BLKSTR*> GetCornerBlocks();

};