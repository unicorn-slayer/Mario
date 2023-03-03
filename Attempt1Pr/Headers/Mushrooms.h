#pragma once
#include <iostream>
#include <string>
#include "AnimatedSprite.h"
#include "Globals.h"

class Mushroom : Sprite
{
public:
	Mushroom();
	Mushroom(Graphics& graphics, int x, int y, int width, int height);

	Rectangle getRect();
	int getXPosition();
	int getYPosition();
	void setUpdatedXPosition(int position);
	void setUpdatedYPosition(int position);
	void draw(Graphics& graphics);
	void activate();
	void update(float elapsedTime);


private:
	Vector2 _originalPositions;
	floatVector2 _levelPositions;
	
	Vector2 _updatedPositions;
	Rectangle _rect;
	float _dx;
	float _dy;
	bool _becomeNormal;
	bool _commenceCollisionDetection;
};