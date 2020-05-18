#ifndef PADDLE_H
#define PADDLE_H

#include "defines.h"
#include "base.h"

class Paddle : public Base {
	public:
		Paddle();
		void update_pos_centre_x(int x_);
};



#endif
