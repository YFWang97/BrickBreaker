#include "defines.h"
#include <paddle.h>

Paddle::Paddle() 
	: Base(paddleTexture, {PADDLE_INITIAL_X_POSITION, PADDLE_INITIAL_Y_POSITION}, 1) {

    paddleWidth = P_NORMAL;
}

void Paddle::update_pos_centre_x(int x_) {

    update_pos_centre({x_, centre.y});
}

void Paddle::update_width(PaddleWidth paddleWidth_) {
    Base::update_width((int)ceil((double)paddleWidth_ / paddleWidth * width));
    paddleWidth = paddleWidth_;

}
