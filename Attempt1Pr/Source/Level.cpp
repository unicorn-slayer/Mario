#include "Level.h"
#include "Graphics.h"
#include "SDL.h"
#include "Globals.h";
#include "tinyxml2.h";
#include <sstream>;
#include <algorithm>;
#include <cmath>;

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics& graphics, Rectangle& camera) :
	_mapName(mapName),
	_spawnPoint(spawnPoint),
	_size(Vector2(0, 0))
{
	this->loadMap(mapName, graphics, camera);
}

Level::~Level() {

}

void Level::loadMap(std::string mapName, Graphics& graphics, Rectangle& camera)
{
	XMLDocument doc;
	std::stringstream ss;
	ss << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	//Get the width and the height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	//Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	//Loading the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->Attribute("altsource");
			std::string ss = source;
			//char* path;
			//std::stringstream ss;
			//ss << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	//Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//Build each individual tile here
							//If gid is 0, no tile should be drawn. Continue loop
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							//Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {

										tls = this->_tilesets.at(i);
										break;
								}
							}

							if (tls.FirstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);							
							Vector2 finalTilePosition = Vector2(xx, yy);

							//calculate position of the tile in the tileset
							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) - 1;
							tsxx *= tileWidth;
							int tsyy = 0;
							int amt = (gid / (tilesetWidth / tileWidth));
							tsyy = tileHeight * amt;
							Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							tileCounter++;


							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	//Parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL)
	{
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
				pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
			}
			if (ss.str() == "mushrooms")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_mushroomList.push_back(Mushroom(
							graphics,
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
				pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
			}
		}
	}

}

void Level::update(int elapsedTime)
{
	if (this->_mushroomList.size() > 0)
	{
		for (int i = 0; i < this->_mushroomList.size(); i++)
		{
			this->_mushroomList.at(i).update(elapsedTime);
		}
	}



}

void Level::draw(Graphics& graphics, Rectangle& camera) 
{
	for (int i = 0; i < this->_tileList.size(); i++)
	{		
		if (this->_tileList.at(i).getRect().collidesWith(camera))
		{					
			this->_tileList.at(i).setUpdatedXPosition(this->_tileList.at(i).getXPosition() - camera.getLeft());
			this->_tileList.at(i).setUpdatedYPosition(this->_tileList.at(i).getYPosition() - camera.getTop());
			
			this->_tileList.at(i).draw(graphics);
		}		
	}

	for (int i = 0; i < this->_mushroomList.size(); i++)
	{
		if (this->_mushroomList.at(i).getRect().collidesWith(camera))
		{
			this->_mushroomList.at(i).setUpdatedXPosition(this->_mushroomList.at(i).getXPosition() - camera.getLeft());
			this->_mushroomList.at(i).setUpdatedYPosition(this->_mushroomList.at(i).getYPosition() - camera.getTop());

			this->_mushroomList.at(i).draw(graphics);
		}
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle& other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

void Level::checkMushroomCollisions(const Rectangle& other)
{
	for (int i = 0; i < this->_mushroomList.size(); i++)
	{
		if (this->_mushroomList.at(i).getRect().collidesWith(other))
		{
			this->_mushroomList.at(i).activate();
		}
	}
}


