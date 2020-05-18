#include "brick.h"
#include "defines.h"

Brick::Brick(BrickType brickType_, Coordinate pos) 
    : Base(brickTextures[brickType_], pos) {
    brickType = brickType_;
}

BrickType Brick::get_brick_type() {return brickType;}

void Brick::set_brick_type(BrickType brickType_) {brickType = brickType_;}
