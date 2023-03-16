#pragma once
#include "LTexture.h"
#include <iostream>

//Starts up SDL and creates window
bool init();


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The dimensions of the level
const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 720;

//Tile constants
const double SMOOTH = 0.6;
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILE_SPRITES = 5;

//The different tile sprites
const char TILE_SPACE = '-';
const char TILE_MOON = '+';
const char TILE_COOKIE = 'O';
const char TILE_SQUARE = 'S';
const char TILE_BODY = 'B';
const char TILE_PEAK = 'P';

extern float Layer1;
extern float Layer2;
extern float Layer3;


const char NULL_TILE = ' ';

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

