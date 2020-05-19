#include "powerup.h"
#include "defines.h"

Powerup::Powerup(PowerupType powerupType_, Coordinate pos)
    : Base (powerupTexture[powerupType_], pos) {
    
    powerupType = powerupType_;
}

PowerupType Powerup::get_powerup_type() {return powerupType;}

void Powerup::update_pos() {
    update_pos_topleft({boundary.left, boundary.top + speedY});
}
