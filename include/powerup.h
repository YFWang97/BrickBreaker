#ifndef POWERUP_H
#define POWERUP_H

#include "base.h"

enum PowerupType {
    POW_NONE = -1,
    POW_FAST = 0,
    POW_LONG = 1,
    POW_SHORT = 2
};

class Powerup : public Base {
    private:
        PowerupType powerupType;
        int speedY = 3;
    public:
        Powerup(PowerupType powerupType_, Coordinate pos);
        PowerupType get_powerup_type();

        void update_pos();

};

#endif
