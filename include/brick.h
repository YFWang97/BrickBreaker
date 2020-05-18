#ifndef BRICK_H
#define BRICK_H

#include "base.h"


enum BrickType {
    BR_DEFAULT = 0,
    BR_UNBREAK = 1,
    BR_BREAKMULT = 2
};

class Brick : public Base {
    private:
        BrickType brickType;
    public:
        Brick(BrickType brickType_, Coordinate pos);
        BrickType get_brick_type();
        void set_brick_type(BrickType brickType_);
};

#endif
