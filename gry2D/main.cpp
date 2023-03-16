/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and file streams

#include <fstream>
#include <vector>
#include <iostream>
#include "Square.h"
#include "Tile.h"
#include "Globals.h"
#include "LTexture.h"

//Loads media
bool loadMedia(std::vector<Tile>& tiles);

//Frees media and shuts down SDL
void close(std::vector<Tile>& tiles);

//Sets tiles from tile map
bool setTiles(std::vector<Tile>& tiles, std::string path);


int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		std::vector<Tile> tileSet;

		//Load media
		if (!loadMedia(tileSet))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			srand(time(0));
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Square square;
			square.calculate();
			double distance = 0;

			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					square.handleEvent(e);
				}

					square.move(tileSet);
					square.setCamera(camera);
					square.jump(tileSet);

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					for (auto Tile : tileSet)
					{
						Tile.render(camera, Layer1, Layer2, Layer3);
					}

					//Render dot
					square.render(camera);

				SDL_RenderPresent(gRenderer);
			}

		}

		//Free resources and close SDL
		close(tileSet);
	}

	return 0;
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
				tiles.push_back(Tile(x * 80 , y * 80, Character));
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