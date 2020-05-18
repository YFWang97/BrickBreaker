#include "defines.h"
#include <paddle.h>

Paddle::Paddle() 
	: Base(paddleTexture, {PADDLE_INITIAL_X_POSITION, PADDLE_INITIAL_Y_POSITION}, 1) {
}

void Paddle::update_pos_centre_x(int x_) {

    update_pos_centre({x_, centre.y});
}
