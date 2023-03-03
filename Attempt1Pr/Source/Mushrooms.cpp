#include "Mushrooms.h"

namespace mushroom_constants {
	const float MOVING_SPEED = 0.2f;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Mushroom::Mushroom(Graphics& graphics, int x, int y, int width, int height) :
	Sprite(graphics, "Sprites/items.png", 0, 8, width / 2, height / 2, x, y),
	_originalPositions(x, y),
	_levelPositions(x, y),
	_rect(x, y, width, height),
	_dx(0),
	_dy(0),
	_becomeNormal(false),
	_commenceCollisionDetection(false)
{
}

//do update function which changes updated positions
void Mushroom::update(float elapsedTime)
{
	if (_becomeNormal == false)
	{
		_levelPositions.y = _levelPositions.y - _dy;
	}

	if (_originalPositions.y - _levelPositions.y >= 32.0)
	{
		_becomeNormal = true;
		_dx = mushroom_constants::MOVING_SPEED;
	}	

	if (_becomeNormal == true)
	{
		//Apply gravity
		if (this->_dy <= mushroom_constants::GRAVITY_CAP) {
			this->_dy += mushroom_constants::GRAVITY * elapsedTime;
		}
		_levelPositions.y = _levelPositions.y + _dy;
		_levelPositions.x = _levelPositions.x + _dx;
	}

}

Rectangle Mushroom::getRect()
{
	return _rect;
}

void Mushroom::draw(Graphics& graphics)
{

	Sprite::draw(graphics, _updatedPositions.x, _updatedPositions.y);

}

void Mushroom::activate()
{
	_dy = mushroom_constants::MOVING_SPEED;
}

int Mushroom::getXPosition()
{
	return _levelPositions.x;
}


int Mushroom::getYPosition()
{
	return _levelPositions.y;
}

void Mushroom::setUpdatedXPosition(int position)
{
	_updatedPositions.x = position;
}

void Mushroom::setUpdatedYPosition(int position)
{
	_updatedPositions.y = position;
}


