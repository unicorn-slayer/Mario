#pragma once

#include "Player.h"
#include "Level.h"
#include "Mushrooms.h"

class Graphics;

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics& graphics, Rectangle& camera);
	void update(float elapsedTime);

	Player _player;

	Level _level;
};

