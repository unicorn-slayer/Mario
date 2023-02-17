#pragma once
#include "globals.h";
#include "Rectangle.h"

struct SDL_Texture;
class Graphics;

class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	void update(int elapsedTime);
	void draw(Graphics& graphics);
	Rectangle getRect();
	void setXPosition(int position);
	void setYPosition(int position);
	double getXPosition();
	double getYPosition();

	void setUpdatedXPosition(double position);
	void setUpdatedYPosition(double position);

protected:
	SDL_Texture* _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;
	Vector2 _updatedPosition;
	Rectangle _rect;
};
