#include "defines.h"
#include "base.h"

Base::Base(SDL_Texture* texture_, Coordinate initPos, bool type) {
    if (texture_ == NULL) {
        ERROR_MSG("BASE: Invalid texture input");
    }
    
	texture = texture_;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	
	rect.w = width;
	rect.h = height;

	if (type == 0) { //Inital pos using topleft corner
		update_pos_topleft(initPos);
	} else { //Initial pos using centre
		update_pos_centre(initPos);
	}
}

void Base::set_texture(SDL_Texture* texture_) {texture = texture_;}

Coordinate Base::get_centre() {return centre;}

Bound Base::get_boundary() {return boundary;}

SDL_Rect Base::get_rect() {return rect;}

void Base::update_pos_centre(Coordinate centre_) {

    int top = centre_.y - height / 2;

    if (top <= 0) {
        boundary.top = 0;
    } else {
	    boundary.top = top;
    }

    boundary.bottom = boundary.top + height - 1;

    int left = centre_.x - width / 2;
    int right = centre_.x + width / 2;

    if (left <= 0) {
        boundary.left = 0;
        boundary.right = width - 1;
    } else if (right >= SCREEN_WIDTH - 1) {
        boundary.right = SCREEN_WIDTH - 1;
        boundary.left = boundary.right - width + 1;
    } else {
        boundary.left = left;
        boundary.right = right;
    }
	
	rect.x = boundary.left;
	rect.y = boundary.top;

    centre.x = (boundary.left + boundary.right) / 2;
    centre.y = (boundary.top + boundary.bottom) / 2;

}

void Base::update_pos_topleft(Coordinate topleft_) {
    if(topleft_.y <= 0) {
        boundary.top = 0;
    } else {
        boundary.top = topleft_.y;
    }

    boundary.bottom = boundary.top + height - 1; 

	int right = topleft_.x + width - 1;
    if (topleft_.x <= 0) {
        boundary.left = 0;
        boundary.right = width - 1;
    } else if (right >= SCREEN_WIDTH - 1) {
        boundary.right = SCREEN_WIDTH - 1;
        boundary.left = boundary.right - width + 1;
    } else {
        boundary.left = topleft_.x;
        boundary.right = right;
    }
	
	rect.x = boundary.left;
	rect.y = boundary.top;

	centre.x = (boundary.right + boundary.left) / 2;
	centre.y = (boundary.bottom + boundary.top) / 2;
}

//FIXME Width update should be wrt the centre
void Base::update_width(int width_) {
	width = width_;

	boundary.right = boundary.left + width - 1;

	centre.x = (boundary.right + boundary.left) / 2;
}

void Base::update_height(int height_) {
	height = height_;

	boundary.bottom = boundary.top + height - 1;

	centre.y = (boundary.bottom + boundary.top) / 2;
}

void Base::draw() {
	if (SDL_RenderCopy(gRenderer, texture, NULL, &rect)) {
		SDL_ERROR_MSG("SDL: Failed to copy texture to renderer");
	}
}
