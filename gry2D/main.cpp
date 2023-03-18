/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and file streams

#include <fstream>
#include "Square.h"
#include "Tile.h"

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