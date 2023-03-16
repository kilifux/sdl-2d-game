#include "Tile.h"
#include "Globals.h"
#include "Square.h"

//Scene textures
LTexture gSquareTexture;
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
}

void Tile::render(SDL_Rect& camera, float a, float b, float c)
{
	//If the tile is on screen
	if (checkCollision(camera, mBox) && getType() == TILE_MOON)
	{
		//Show the tile
		gTileTexture.render(mBox.x - camera.x , mBox.y - camera.y , &gTileClips[mType]);
	}
	
	if (getType() == TILE_SPACE)
	{
		gTileTexture.render(mBox.x - camera.x * a, mBox.y - camera.y * 0.1, &gTileClips[mType]);
	}

	if (getType() == TILE_COOKIE || getType() == TILE_SQUARE)
	{
		gTileTexture.render(mBox.x - camera.x * b, mBox.y - camera.y * 0.3, &gTileClips[mType]);
	}

	if (getType() == TILE_BODY || getType() == TILE_PEAK)
	{
		gTileTexture.render(mBox.x - camera.x * c, mBox.y - camera.y * 0.6, &gTileClips[mType]);
	}
}

int Tile::getType()
{
	return mType;
}

SDL_Rect Tile::getBox()
{
	return mBox;
}
