#include <fstream>
#include <iostream>
#include "Globals.h"
#include "Tile.h"

SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

float Layer1 = 0.1;
float Layer2 = 0.3;
float Layer3 = 0.6;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Platformer Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(std::vector<Tile>& tiles)
{
	//Loading success flag
	bool success = true;

	//Load square texture
	if (!gSquareTexture.loadFromFile("square.png"))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	//Load tile texture
	if (!gTileTexture.loadFromFile("TileMap.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles, "background.map"))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles, "planets.map"))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles, "mountains.map"))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}


	//Load tile map
	if (!setTiles(tiles, "lazy.map"))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	return success;
}

void close(std::vector<Tile>& tiles)
{
	tiles.clear();

	gSquareTexture.free();
	gTileTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool setTiles(std::vector<Tile>& tiles, std::string path)
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map(path);

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		std::string Line;
		while (std::getline(map, Line)) {
			for (char Character : Line)
			{
				tiles.push_back(Tile(x * 80, y * 80, Character));
				x++;
			}
			y++;
			x = 0;
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			gTileClips[TILE_MOON].x = 0;
			gTileClips[TILE_MOON].y = 0;
			gTileClips[TILE_MOON].w = TILE_WIDTH;
			gTileClips[TILE_MOON].h = TILE_HEIGHT;

			gTileClips[TILE_SPACE].x = 0;
			gTileClips[TILE_SPACE].y = 80;
			gTileClips[TILE_SPACE].w = TILE_WIDTH;
			gTileClips[TILE_SPACE].h = TILE_HEIGHT;

			gTileClips[TILE_COOKIE].x = 0;
			gTileClips[TILE_COOKIE].y = 160;
			gTileClips[TILE_COOKIE].w = TILE_WIDTH;
			gTileClips[TILE_COOKIE].h = TILE_HEIGHT;

			gTileClips[TILE_SQUARE].x = 0;
			gTileClips[TILE_SQUARE].y = 240;
			gTileClips[TILE_SQUARE].w = TILE_WIDTH;
			gTileClips[TILE_SQUARE].h = TILE_HEIGHT;

			gTileClips[TILE_BODY].x = 0;
			gTileClips[TILE_BODY].y = 320;
			gTileClips[TILE_BODY].w = TILE_WIDTH;
			gTileClips[TILE_BODY].h = TILE_HEIGHT;

			gTileClips[TILE_PEAK].x = 0;
			gTileClips[TILE_PEAK].y = 400;
			gTileClips[TILE_PEAK].w = TILE_WIDTH;
			gTileClips[TILE_PEAK].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}