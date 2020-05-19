#ifndef PADDLE_H
#define PADDLE_H

#include "defines.h"
#include "base.h"

enum PaddleWidth {
    P_NORMAL = 2,
    P_LONG = 4,
    P_SHORT = 1   
};

class Paddle : public Base {
    private:
        PaddleWidth paddleWidth;

	public:
		Paddle();
		void update_pos_centre_x(int x_);
        void update_width(PaddleWidth paddleWidth_);
};



#endif
