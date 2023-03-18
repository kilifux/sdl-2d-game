#pragma once

#include "Globals.h"

//Scene textures
extern LTexture gSquareTexture;
extern LTexture gTileTexture;
extern SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	void render(SDL_Rect& camera, float a, float b, float c);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};

