#pragma once
#include <string>
#include <vector>
#include "Globals.h"
#include "Tile.h"
#include "Rectangle.h"
#include "Mushrooms.h"

class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct SDL_Tileset;

struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;
	Tileset() {
		this->FirstGid = -1;
	}
	Tileset(SDL_Texture* texture, int firstGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};

class Level {
public:
	Level();
	Level(std::string mapName, Vector2 spawnPoint, Graphics& graphics, Rectangle& camera);
	~Level();
	void update(int elapsedTime);
	void draw(Graphics& graphics, Rectangle& camera);

	std::vector<Rectangle> checkTileCollisions(const Rectangle& other);
	void checkMushroomCollisions(const Rectangle& other);



private:
	std::string _mapName;
	Vector2 _spawnPoint;
	Vector2 _size;
	Vector2 _tileSize;
	SDL_Texture* _backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;

	std::vector<Mushroom> _mushroomList;

	void loadMap(std::string mapName, Graphics& graphics, Rectangle& camera);

};





