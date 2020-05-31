#ifndef DEFINE_H
#define DEFINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#define SDL_ERROR_MSG(...) cout << __VA_ARGS__ << "! " << "SDL_Error: " << SDL_GetError() << endl
#define ERROR_MSG(...) cout << __VA_ARGS__ << "! " << endl
#define DEBUG_MSG(...) cout << "DEBUG INFO: " << __VA_ARGS__ << "! " << endl

typedef struct coordinate {
	int x;
	int y;
} Coordinate;

typedef struct bound {
	int top;
	int bottom;
	int left;
	int right;
} Bound;

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 1000
#define BRICK_REGION_HEIGHT 600

#define PADDLE_HEIGHT 21
#define PADDLE_INITIAL_LENGTH 105
#define PADDLE_INITIAL_Y_POSITION (SCREEN_HEIGHT - 30 - PADDLE_HEIGHT / 2)
#define PADDLE_INITIAL_X_POSITION ((SCREEN_WIDTH - PADDLE_INITIAL_LENGTH) / 2)

#define BALL_INITIAL_RADIUS 8
#define BALL_INITIAL_Y_POSITION (PADDLE_INITIAL_Y_POSITION - PADDLE_HEIGHT / 2 - BALL_INITIAL_RADIUS)
#define BALL_INITIAL_X_POSITION PADDLE_INITIAL_X_POSITION

#define BRICK_HEIGHT 20
#define BRICK_WIDTH 60

#define BRICK_ROW 8
#define BRICK_COL 15

#define BRICK_MARGIN ((SCREEN_WIDTH - (BRICK_COL * BRICK_WIDTH)) / 2)

#define PI 3.1415926

extern SDL_Renderer* gRenderer;
extern SDL_Texture* ballTexture;
extern SDL_Texture* paddleTexture;
extern SDL_Texture* brickTextures[5]; 
extern SDL_Texture* overTexture;
extern SDL_Texture* levelupTexture;
extern SDL_Texture* powerupTexture[5];

#endif
