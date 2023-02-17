
#include "tile.h"
#include "graphics.h"

#include <SDL.h>

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2(position.x* globals::SPRITE_SCALE, position.y* globals::SPRITE_SCALE)),
	_rect(position.x* globals::SPRITE_SCALE, position.y* globals::SPRITE_SCALE,
		size.x* globals::SPRITE_SCALE, size.y* globals::SPRITE_SCALE),
	_updatedPosition(Vector2(0, 0))
{}

void Tile::update(int elapsedTime) {}

void Tile::draw(Graphics& graphics) {
	SDL_Rect destRect = { this->_updatedPosition.x, this->_updatedPosition.y,
			this->_size.x * globals::SPRITE_SCALE, this->_size.y * globals::SPRITE_SCALE };
	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };

	graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
}

Rectangle Tile::getRect()
{
	return _rect;
}

void Tile::setXPosition(int position)
{
	_position.x = position;
}

void Tile::setYPosition(int position)
{
	_position.y = position;
}

double Tile::getXPosition()
{
	return _position.x;
}

double Tile::getYPosition()
{
	return _position.y;
}

void Tile::setUpdatedXPosition(double position)
{
	_updatedPosition.x = position;
}

void Tile::setUpdatedYPosition(double position)
{
	_updatedPosition.y = position;
}