#include "brick.h"
#include "defines.h"
#include "powerup.h"

Brick::Brick(BrickType brickType_, Coordinate pos) 
    : Base(brickTextures[brickType_], pos) {
    brickType = brickType_;
    powerupType = POW_NONE;
}

BrickType Brick::get_brick_type() {return brickType;}

PowerupType Brick::get_powerup_type() {return powerupType;}

void Brick::set_brick_type(BrickType brickType_) {
    brickType = brickType_;
    set_texture(brickTextures[brickType_]);
}

void Brick::set_powerup_type(PowerupType powerupType_) {
    powerupType = powerupType_;
}
