#include "Player.h"
#include "Graphics.h"

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.8f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player()
{
}

Player::Player(Graphics& graphics, float x, float y) :
	AnimatedSprite(graphics, "Sprites/marioSprite.png", 0, 0, 16, 16, x, y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false)
{
	graphics.loadImage("Sprites/marioSprite.png");

	this->setupAnimations();
	this->playAnimation("runLeft");
}

void Player::draw(Graphics& graphics, Rectangle& camera) 
{
	AnimatedSprite::draw(graphics, this->_x - camera.getLeft(), this->_y - camera.getTop());
	//AnimatedSprite::draw(graphics, this->_x - camera.x, this->_y - camera.y); add camera to parameter list
}

void Player::update(float elapsedTime)
{
	//Apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	this->_x += this->_dx * elapsedTime;

	//Move by dy
	this->_y += this->_dy * elapsedTime;
	AnimatedSprite::update(elapsedTime);
}

void Player::animationDone(std::string currentAnimation)
{
}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}

void Player::moveLeft() {
	this->_dx = -player_constants::WALK_SPEED;
	this->playAnimation("runLeft");
	this->_facing = LEFT;
}

void Player::moveRight() {
	this->_dx = player_constants::WALK_SPEED;
	this->playAnimation("runRight");
	this->_facing = RIGHT;
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	this->playAnimation(this->_facing == RIGHT ? "idleRight" : "idleLeft");
}

void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

//void handleTileCollisions
//Handles collisions with ALL tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle>& others) {
	//Figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				if (this->_grounded) {
					this->_dx = 0;
					this->_x -= this->_facing == RIGHT ? 1.0f : -1.0f;
				}

				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			}

		}
	}
}

void Player::setCamera(Rectangle& camera)
{
	//Center the camera over the dot

	int cameraOffset = 160;
	camera.setX((this->_boundingBox.getCenterX()) - globals::SCREEN_WIDTH / 2);
	camera.setY((this->_boundingBox.getCenterY()) - (globals::SCREEN_HEIGHT / 2) - cameraOffset);

	//Keep the camera in bounds
	if (camera.getLeft() < 0)
	{
		camera.setX(0);
	}
	if (camera.getTop() < 0)
	{
		camera.setY(0);
	}
	if (camera.getLeft() > globals::LEVEL_WIDTH - camera.getWidth()) //change in globals
	{
		camera.setX(globals::LEVEL_WIDTH - camera.getWidth());
	}
	if (camera.getTop() > globals::LEVEL_HEIGHT - camera.getHeight())
	{
		camera.setY(globals::LEVEL_HEIGHT - camera.getHeight());
	}
}

void Player::setupAnimations()
{
	this->addAnimation(1, 0, 24, "idleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 8, "idleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 24, "runLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 8, "runRight", 16, 16, Vector2(0, 0));
}
