#ifndef BASE_H
#define BASE_H

#include "defines.h"

class Base {
	protected:
		SDL_Texture* texture;
		Coordinate centre;
		Bound boundary;
		SDL_Rect rect;
		int width;
		int height;

	public:
        /**
         * Constructor for Base image class
         *
         * @param type: Default using topleft corner as the initial position
         * @return
         */
		Base(SDL_Texture* texture_, Coordinate initPos, bool type = 0);

		Coordinate get_centre();
		Bound get_boundary();
		SDL_Rect get_rect();

		//Use the centre as the pivot to move
		void update_pos_centre(Coordinate centre_);
		//Use the top-left corner as the pivot to move
		void update_pos_topleft(Coordinate topleft_);

		//Change width & height will use top-left corner as the pivot to extend
		void update_width(int width_);
		void update_height(int height_);

		void draw();		
};

#endif
