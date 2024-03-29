#ifndef BALL_H
#define BALL_H

#include "defines.h"
#include "base.h"

enum BallRadius {
	R_NORMAL = 8,
	R_LARGE = 10,
	R_SMALL = 5
};

enum BallSpeed {
	S_NORMAL = 6,
	S_FAST = 12,
	S_SLOW = 3
};

class Ball : public Base {
	private:
		int speedX;
		int speedY;	

		BallSpeed speed;
		BallRadius radius;

		bool fired;
        int lives;

	public:
		Ball();
		int get_speed_x();
		int get_speed_y();
		BallSpeed get_speed();
        int get_radius();

        int updateCnt;

        bool throughBall;
    
        void set_speed(BallSpeed speed_);

		void set_speed_x(int speedX_);
		void set_speed_y(int speedY_);

		void set_speed_x_neg();
		void set_speed_y_neg();

		void set_radius(BallRadius radius_);

        void reset();

		void update_pos();

		/**
		 * Update the position of the ball along with the paddle when not fired
		 * Check the firing status of ball
		 *
		 * @param
		 * @return
		 */
		void update_pos_w_paddle(Coordinate paddleCentre);

		void fire(int speedX_, int speedY_);

        int get_lives();
        void set_lives(int lives_);
};



#endif
