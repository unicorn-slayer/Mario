#pragma once

#include <iostream>
#include <string>
#include "AnimatedSprite.h"
#include "Globals.h"


class Graphics;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics& graphics, float x, float y);
	void draw(Graphics& graphics, Rectangle& camera);
	void update(float elapsedTime);

	void moveLeft();
	void moveRight();
	void stopMoving();
	void jump();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	void handleTileCollisions(std::vector<Rectangle>& others);
	void setCamera(Rectangle& camera);

	const float getX() const;
	const float getY() const;
private:
	float _dx, _dy;
	Direction _facing;

	bool _grounded;
};