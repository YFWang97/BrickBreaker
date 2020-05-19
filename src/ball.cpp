#include "ball.h"

Ball::Ball()
	: Base(ballTexture, {BALL_INITIAL_X_POSITION, BALL_INITIAL_Y_POSITION}, 1) {
	speedX = 0;
	speedY = 0;
	radius = R_NORMAL;
	speed = S_NORMAL;
	fired = 0;
}

int Ball::get_speed_x() {return speedX;}

int Ball::get_speed_y() {return speedY;}

BallSpeed Ball::get_speed() {return speed;}

int Ball::get_radius() {return (int)radius;}

void Ball::set_speed(BallSpeed speed_) {
    speedX *= (speed_ / speed);
    speedY *= (speed_ / speed);
    speed = speed_;
}

void Ball::set_speed_x(int speedX_) {speedX = speedX_;}

void Ball::set_speed_y(int speedY_) {speedY = speedY_;}

void Ball::set_speed_x_neg() {speedX = 0 - speedX;}

void Ball::set_speed_y_neg() {speedY = 0 - speedY;}

void Ball::set_radius(BallRadius radius_) {
    radius = radius_;
    //FIXME Set radius should change the SDL_Rect as well
}

void Ball::reset() {
    speedX = 0;
    speedY = 0;
    radius = R_NORMAL;
    speed = S_NORMAL;
    fired = 0;
}

void Ball::update_pos() {

	centre.x += speedX;
	centre.y += speedY;

	update_pos_centre(centre);
}

void Ball::update_pos_w_paddle(Coordinate paddleCentre) {
	//Only updates with paddle when the ball is not fired
	if (!fired) {
		centre.x = paddleCentre.x;
		centre.y = paddleCentre.y - PADDLE_HEIGHT / 2 - radius;

		update_pos_centre(centre);
	}
}

void Ball::fire(int speedX_, int speedY_) {
	//Only fire when not in flight
	if (!fired) {
		fired = true;
		speedX = speedX_;
		speedY = speedY_;
	}
}
