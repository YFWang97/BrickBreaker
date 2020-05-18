#ifndef HELPER_H
#define HELPER_H

#include "defines.h"

int initialize();

int initialize_img();

SDL_Texture* load_img(char* name);

/**
 * Check the collision status
 * @param   portion: returns the hitting portion wrt 1/2 objB width
 * @return  true: colliding; false: not colliding
 */
bool check_collision(Bound objA, Bound objB, double* portion = NULL);

/**
 * Check the collision status between a circle and a rectangle
 * @param   objACentre: centre coordinate of objA
 *          obARad: radius of objA
 *          objB: boundary of objB
 *          portion: percentage of the hitting position wrt 1/2 objB width
 *          edge: return the colliding edge: 1 left, 2 right, 4 top, 8 bottom 
 *              
 * @return  0: no collision; 1: collision
 */
bool check_collision_cir(Coordinate objACentre, int objARad, Bound objB, double* portion = NULL, int* edge = NULL);

bool inside(Coordinate point, Bound bound);


/**
 * Check the sign of a number. Can also be used as the direction
 * @param
 * @return 0 for positive or 0, 1 for negative
 */
bool sign(int num);

void quit_game();
#endif
