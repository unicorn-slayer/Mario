#include "Game.h"
#include "graphics.h"
#include "Input.h"

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}

Game::~Game() {

}

void Game::gameLoop()
{
	Graphics graphics;
	Input input;
	SDL_Event event;

	Rectangle camera = { 0, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT }; // get screen width and height from globals

	this->_player = Player(graphics, 280, 100);
	this->_level = Level("Maps/level11", Vector2(100, 100), graphics, camera); //this is not in while loop so won't update


	int LAST_UPDATE_TIME = SDL_GetTicks();
	while (true)
	{
		input.beginNewFrame();



		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			return;
		}
		if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
			this->_player.moveLeft();
		}
		if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
			this->_player.moveRight();
		}
		if (input.wasKeyPressed(SDL_SCANCODE_Z) == true) {
			this->_player.jump();
		}
		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_player.stopMoving();
		}

		this->draw(graphics, camera);

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->_player.setCamera(camera); //go into tiling sln and implement similar func in player class to get camera
		//centred over player. Add limits.


	}
}

void Game::draw(Graphics& graphics, Rectangle& camera) //add camera object into parameter list
{
	graphics.clear();
	this->_level.draw(graphics, camera); //set camera in while loop, add into parameter list here and in _player below
	//this->_level.draw(graphics, camera);
	this->_player.draw(graphics, camera);
	//this->_player.draw(graphics, camera);
	graphics.flip();
}

void Game::update(float elapsedTime)
{
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime);

	//Check tile collisions
	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		//Player collided with at least one tile. Handle it.
		this->_player.handleTileCollisions(others);	}

	this->_level.checkMushroomCollisions(this->_player.getBoundingBox());



}