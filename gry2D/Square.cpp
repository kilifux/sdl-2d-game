#include "Square.h"
#include "Globals.h"


bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool touchesWall(SDL_Rect box, std::vector<Tile>& tiles)
{
	//Go through the tiles
	for (int i = 0; i < tiles.size(); ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i].getType() == TILE_MOON))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i].getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}


bool touchesTop(SDL_Rect box, std::vector<Tile>& tiles)
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		SDL_Rect tBox = tiles[i].getBox();
		if (tiles[i].getType() == TILE_MOON && (box.y + box.h == tBox.y) &&
			((box.x > tBox.x && box.x < tBox.x + tBox.w) || (box.x + box.w > tBox.x && box.x + box.w < tBox.x + tBox.w)))
		{
			return true;
		}
	}
	return false;
}

Square::Square()
{
	mSquare.w = SQUARE_SIZE;
	mSquare.h = SQUARE_SIZE;

	mSquare.x = 10;
	mSquare.y = 600;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	//Jump
	startingPositionX = mSquare.x;
	startingPositionY = mSquare.y;
	accelerationX = 0;
}

void Square::calculate()
{
	jump_velocity = -(2 * jump_height * SQUARE_VEL / jump_distance);
	gravity = ((2 * jump_height * SQUARE_VEL * SQUARE_VEL) / (jump_distance * jump_distance)) - 0.006;
	accelerationY = gravity;
}

void Square::jump(std::vector<Tile>& tiles)
{
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();
	deltaTime = ((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

	float sY = mSquare.y;
	float sX = mSquare.x;
	float vel_timeX = mVelX * deltaTime;
	float vel_timeY = mVelY * deltaTime;

	float acc_timeX = accelerationX * deltaTime * deltaTime * 0.5;
	float acc_timeY = accelerationY * deltaTime * deltaTime * 0.5;

	mSquare.x += (vel_timeX + acc_timeX);
	mSquare.y += (vel_timeY + acc_timeY);

	mVelX += accelerationX * deltaTime;
	mVelY += accelerationY * deltaTime;

	if (mSquare.y >= startingPositionY) {
		mVelY = 0;
		mSquare.y = startingPositionY;

		//std::cout << "startingPositionY: " << startingPositionY << std::endl;
	}

	if ((mSquare.y < 0) || (mSquare.y + SQUARE_SIZE > LEVEL_HEIGHT) || touchesWall(mSquare, tiles))
	{
		mSquare.y = sY;
		mVelY = 0;
		if ((mSquare.y + SQUARE_SIZE > LEVEL_HEIGHT) || touchesTop(mSquare, tiles));
		jumpCount = 0;
	}

	if ((mSquare.x < 0) || (mSquare.x + SQUARE_SIZE > LEVEL_WIDTH) || touchesWall(mSquare, tiles))
	{
		mSquare.x = sX;
		mVelX = 0;
		if ((mSquare.y + SQUARE_SIZE > LEVEL_WIDTH) || touchesTop(mSquare, tiles));
		jumpCount = 0;
	}

	if (!touchesWall(mSquare, tiles)) {
		startingPositionY += (vel_timeY + acc_timeY);
	}


}

void Square::move(std::vector<Tile>& tiles)
{
	//Move the dot left or right
	mSquare.x += mVelX;

	//If the dot went too far to the left or right or touched a wall
	if ((mSquare.x < 0) || (mSquare.x + SQUARE_SIZE > LEVEL_WIDTH) || touchesWall(mSquare, tiles))
	{
		//move back
		mSquare.x -= mVelX;
	}

	//Move the dot up or down
	mSquare.y += mVelY;

	//If the dot went too far up or down or touched a wall
	if ((mSquare.y < 0) || (mSquare.y + SQUARE_SIZE > LEVEL_HEIGHT) || touchesWall(mSquare, tiles))
	{
		//move back
		mSquare.y -= mVelY;
	}

}

int Square::getX()
{
	return mSquare.x;
}

int Square::getY()
{
	return mSquare.y;
}

void Square::setX(int x)
{
	mSquare.x = x;
}

void Square::setY(int y)
{
	mSquare.y = y;
}

void Square::render(SDL_Rect& camera)
{
	gSquareTexture.render(mSquare.x - camera.x, mSquare.y - camera.y);
}

void Square::setCamera(SDL_Rect& camera)
{
	//Center the camera over the dot
	camera.x = (mSquare.x + SQUARE_SIZE / 2) - SCREEN_WIDTH / 2;
	camera.y = (mSquare.y + SQUARE_SIZE / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Square::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			if (jumpCount < 2)
			{
				jumpCount++;
				startingPositionX = mSquare.x;
				startingPositionY = mSquare.y;
				mVelY = jump_velocity;
			}

			break;
		case SDLK_a:
			mVelX -= SQUARE_VEL * (1 - SMOOTH) + mVelX * SMOOTH;
			break;
		case SDLK_d:
			mVelX += SQUARE_VEL * (1 - SMOOTH) + mVelX * SMOOTH;
			break;
		case SDLK_1:
			Layer1 += 0.1;
			std::cout << "Current Layer1: " << Layer1 << std::endl;
			break;
		case SDLK_2:
			Layer1 -= 0.1;
			std::cout << "Current Layer1: " << Layer1 << std::endl;
			break;
		case SDLK_3:
			Layer2 += 0.1;
			std::cout << "Current Layer2: " << Layer2 << std::endl;
			break;
		case SDLK_4:
			Layer2 -= 0.1;
			std::cout << "Current Layer2: " << Layer2 << std::endl;
			break;
		case SDLK_5:
			Layer3 += 0.1;
			std::cout << "Current Layer3: " << Layer3 << std::endl;
			break;
		case SDLK_6:
			Layer3 -= 0.1;
			std::cout << "Current Layer3: " << Layer3 << std::endl;
			break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			/*case SDLK_w:
				mVelY = 0;
				break;
			case SDLK_s:
				mVelY = 0;
				break;*/
		case SDLK_a:
			mVelX = 0;
			break;
		case SDLK_d:
			mVelX = 0;
			break;
		}
	}
}

SDL_Rect Square::getSquare()
{
	return mSquare;
}
