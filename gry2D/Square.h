#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

bool checkCollision(SDL_Rect a, SDL_Rect b);
bool touchesWall(SDL_Rect box, std::vector<class Tile>& tiles);
bool touchesTop(SDL_Rect box, std::vector<class Tile>& tiles);

class Square
{
	SDL_Rect mSquare;
	
	//Velocity 
	float mVelX, mVelY;

	//Jump
	float accelerationX, accelerationY;
	float startingPositionX, startingPositionY;
	Uint64 LAST, NOW = SDL_GetPerformanceCounter();

public:
	static const int SQUARE_SIZE = 40;
	static const int SQUARE_VEL = 2;

	//Jump
	float jump_height = 150;
	float jump_distance = 350;
	float jump_velocity;
	float gravity;

	//Jump - deltaTime
	float deltaTime;
	long time;
	int jumpCount = 0;

	Square();

	void handleEvent(SDL_Event& e);
	void move(std::vector<Tile>& tiles);
	void setCamera(SDL_Rect& camera);
	void render(SDL_Rect& camera);

	int getX(), getY();
	void setX(int x), setY(int y);

	SDL_Rect getSquare();

	//jump
	void calculate();
	void jump(std::vector<Tile>& tiles);
};

