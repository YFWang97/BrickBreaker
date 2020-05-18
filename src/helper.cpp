#include "defines.h"
#include "helper.h"

SDL_Renderer* gRenderer;
SDL_Window* gWindow;

SDL_Texture* ballTexture;
SDL_Texture* paddleTexture;
SDL_Texture* brickTextures[5];
SDL_Texture* overTexture;

int initialize() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_ERROR_MSG("SDL: Failed to initialize");
		return -1;
	}

	int flags=IMG_INIT_JPG|IMG_INIT_PNG;
	if (IMG_Init(flags)&flags != flags) {
		SDL_ERROR_MSG("IMG INIT: Failed to initialize jpg and png support");
		return -1;
	}

	gWindow = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (gWindow == NULL) {
		SDL_ERROR_MSG("SDL: Failed to create window");
		return -1;	
	} 
	
	// Get the current display attributes 
	SDL_DisplayMode current;

	SDL_GetCurrentDisplayMode(0, &current);

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	if (gRenderer == NULL) {
		SDL_ERROR_MSG("SDL: Failed to create renderer");
		return -1;
	}

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0x00);

    return 0;
}

int initialize_img() {
    ballTexture = load_img("img/ball.png");
    if (ballTexture == NULL) {return -1;}

    paddleTexture = load_img("img/pad.png");
    //paddleTexture = load_img("img/padTest.png");
    if (paddleTexture == NULL) {return -1;}

    brickTextures[0] = load_img("img/brick0.png");
    if (brickTextures[0] == NULL) {return -1;}

    brickTextures[1] = load_img("img/brick1.png");
    if (brickTextures[1] == NULL) {return -1;}

    brickTextures[2] = load_img("img/brick2.png");
    if (brickTextures[2] == NULL) {return -1;}

    overTexture = load_img("img/clear.png");
    if (overTexture == NULL) {return -1;}

    return 0;
}

SDL_Texture* load_img(char* name) {
    SDL_Surface* surface = IMG_Load(name);

    if (surface == NULL) {
        SDL_ERROR_MSG("IMG: Failed to load image to surface");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);

    if (texture == NULL) {
        SDL_ERROR_MSG("SDL: Failed to crate texture from surface");
    }

    return texture;
}

bool check_collision(Bound objA, Bound objB, double* portion) {
    bool vert = false;
    bool hori = false;

    if (objA.top >= objB.top && objA.top <= objB.bottom) {
        vert = true;
    }

    if (objA.bottom >= objB.top && objA.bottom <= objB.bottom) {
        vert = true;
    }

    if (objA.left >= objB.left && objA.left <= objB.right) {
        hori = true;
    }

    if (objA.right >= objB.left && objA.left <= objB.right) {
        hori = true;
    }

    if (portion != NULL) {
        double midA = (objA.left + objA.right) / 2;
        double midB = (objB.left + objB.right) / 2;
        double halfB = (objB.right - objB.left) / 2;
        *portion = (midA - midB) / halfB;
    }

    return vert && hori;
}

bool check_collision_cir(Coordinate objACentre, int objARad, Bound objB, double* portion, int* edge) {
    int closestX, closestY;
    int hit_edge = 0;

    if (objACentre.x < objB.left) {
        closestX = objB.left;
        hit_edge += 1;
    } else if (objACentre.x > objB.right) {
        closestX = objB.right;
        hit_edge += 2;
    } else {
        closestX = objACentre.x;
    }

    if (objACentre.y < objB.top) {
        closestY = objB.top;
        hit_edge += 4;
    } else if (objACentre.y > objB.bottom) {
        closestY = objB.bottom;
        hit_edge += 8;
    } else {
        closestY = objACentre.y;
    }


    double diff = (closestX - objACentre.x) * (closestX - objACentre.x) +
                  (closestY - objACentre.y) * (closestY - objACentre.y) -
                  objARad * objARad;

    if (diff <= 0) {
    printf("CLosestX: %d, CLosestY: %d\n", closestX, closestY);
    printf("Circle Centre: %d %d\n", objACentre.x, objACentre.y);
    printf("Radius: %d\n", objARad);
    printf("Boundary: %d %d %d %d\n", objB.left, objB.right, objB.top, objB.bottom); 
    printf("Diff: %f\n", diff);
    printf("Edge: %d\n", hit_edge); 
    printf("\n");
    }

    if (diff > 0) {
        hit_edge = 0;
    }

    if (portion != NULL) {
        double midB = (objB.left + objB.right) / 2;
        double halfB = (objB.right - objB.left) / 2;
        *portion = (objACentre.x - midB) / halfB;
    }

    if (edge != NULL) {
        *edge = hit_edge;
    }

    return (diff <= 0);
}

bool inside(Coordinate point, Bound bound) {
    if(point.x > bound.left && point.x < bound.right &&
        point.y > bound.top && point.y < bound.bottom) {
        return true;
    }

    return false;
}

bool sign(int num) {
    return (num < 0);
}

void quit_game() {
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}
