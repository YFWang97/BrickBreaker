#include "defines.h"
#include "helper.h"
#include "base.h"
#include "paddle.h"
#include "ball.h"
#include "brick.h"

int main (void) {

    if (initialize() || initialize_img()) {
		ERROR_MSG("PROG: Program quits. Initialization failed");
        return 0;
	}

	bool quit = false;

	SDL_Event e;
	
	bool clicked = false;
    bool pause = false;

    int xLast = 0;
    int xDiff = 0;

	unsigned int startTick = SDL_GetTicks();

	Paddle paddle;
	Ball mainBall;

    Brick* brickArr[BRICK_ROW][BRICK_COL];

    for (int i = 0; i < BRICK_ROW; i++) {
        for (int j = 0; j < BRICK_COL; j++) {
            brickArr[i][j] = new Brick(BR_DEFAULT, {BRICK_MARGIN + j * BRICK_WIDTH, BRICK_MARGIN + i * BRICK_HEIGHT});
        }
    }


	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_MOUSEMOTION:
					int x;
					SDL_GetMouseState(&x, NULL);

                    if ((x - xLast) != 0) {
                        xDiff = x - xLast;
                    }

                    xLast = x;

                    //cout << "Cur x: " << x << " Last x: " << xLast  << " Diff " << xDiff << endl;

					paddle.update_pos_centre_x(x);
					mainBall.update_pos_w_paddle(paddle.get_centre());
					break;
				case SDL_MOUSEBUTTONDOWN:
					clicked = true;
					break;
				case SDL_MOUSEBUTTONUP:
					clicked = false;
					mainBall.fire(10, -5);
					break;
                case SDL_KEYDOWN:
                    pause = !pause;
                    break;
                        
			}
		}

        if (pause) continue;

		if (SDL_GetTicks() - startTick > 2) {
			startTick = SDL_GetTicks();

            //Set new speed base on any collision

            // 1. Check Brick Collision 
			// 2. Check Ball Paddle Collision
            //    2a. Normal Collision from Outside / Low Speed Mouse Motion
            //    2b. Ball Resides inside the Paddle (Caused by Fast Speed Mouse Motion)
			// 3. Check Ball Wall Bouncing

            /* Brick Collision Check */
            if (mainBall.get_centre().y < BRICK_REGION_HEIGHT) {
                int curBallCol = (mainBall.get_centre().x - BRICK_MARGIN) / BRICK_WIDTH;
                int curBallRow = (mainBall.get_centre().y - BRICK_MARGIN) / BRICK_HEIGHT;


                vector<pair<Coordinate, int>> corner_collision;
                vector<pair<Coordinate, int>> edge_collision;

                for (int row = -1; row <= 1; row++) {
                    int brickRow = row + curBallRow;

                    if (brickRow < 0 || brickRow >= BRICK_ROW) {
                        continue;
                    }

                    for (int col = -1; col <= 1; col++) {
                        int brickCol = col + curBallCol;

                        if (brickCol < 0 || brickCol >= BRICK_COL) {
                            continue;
                        }

                        if (brickArr[brickRow][brickCol] == NULL) continue;

                        int edge;

                        bool collision = check_collision_cir(mainBall.get_centre(), mainBall.get_radius(), brickArr[brickRow][brickCol]->get_boundary(), NULL, &edge);

                        Coordinate brickIndex = {brickRow, brickCol};

                        if (edge) {
                            if ((row + col) & 0x1) { //Sum is odd
                                edge_collision.push_back(make_pair(brickIndex, edge));
                            } else {
                                corner_collision.push_back(make_pair(brickIndex, edge));
                            }

                        }
                    }
                }

                if (edge_collision.size() > 2) {
                    ERROR_MSG("Colliding more than two edges");
                }

                if (edge_collision.size() > 1) {
                    ERROR_MSG("Colliding more then two corners");
                } 

                int effEdge = 0;

                if (edge_collision.size() > 0) {
                    for (auto& brick: edge_collision) {
                        effEdge += brick.second;
                    }
                } else {
                    if (corner_collision.size() > 0) {
                        effEdge = corner_collision[0].second;
                    }
                }

                if (effEdge & 0x1 && mainBall.get_speed_x() > 0) {
                    mainBall.set_speed_x_neg();
                }

                if (effEdge & 0x2 && mainBall.get_speed_x() < 0) {
                    mainBall.set_speed_x_neg();
                }

                if (effEdge & 0x4 && mainBall.get_speed_y() > 0) {
                    mainBall.set_speed_y_neg();
                }

                if (effEdge & 0x8 && mainBall.get_speed_y() < 0) {
                    mainBall.set_speed_y_neg();
                }

                for (auto& brick: edge_collision) {
                    delete brickArr[brick.first.x][brick.first.y];
                    brickArr[brick.first.x][brick.first.y] = NULL;
                }

                if (edge_collision.size() < 2) {
                    for (auto& brick: corner_collision) {
                        delete brickArr[brick.first.x][brick.first.y];
                        brickArr[brick.first.x][brick.first.y] = NULL;
                    }
                }
            }

            /* Paddle Collision Check */
			if (mainBall.get_speed_y() > 0 && 
                mainBall.get_centre().y > BRICK_REGION_HEIGHT &&
                mainBall.get_centre().y < paddle.get_boundary().bottom) {

			    double ballPaddlePortion;
                int edge;
                bool collision = check_collision_cir(mainBall.get_centre(), mainBall.get_radius(), paddle.get_boundary(), &ballPaddlePortion, &edge);

                if (edge) { //Collision happens
                    if (edge & 0x4) {
                        double angle = (1 - abs(ballPaddlePortion)) * 90 * PI / 180; 

                        BallSpeed speed = mainBall.get_speed();
                        mainBall.set_speed_x((int)(cos(angle) * speed));
                        mainBall.set_speed_y((int)(sin(angle) * speed));
                        
                        if (ballPaddlePortion < 0) {
                            mainBall.set_speed_x_neg();
                        }

                        //Y should be set as negative when bouncing from the paddle
                        mainBall.set_speed_y_neg();
                        if (mainBall.get_speed_y() == 0) {
                            mainBall.set_speed_y(-1);
                        }
                        cout << "Collision with Paddle from Top" << endl << endl;
                    } else if (edge & 0x3) { //& with 0b11
                        cout << "signdiff: " << sign(xDiff) << " xdiffval: " << xDiff << " speed: " << mainBall.get_speed_x() << endl;

                        bool upperHalf = mainBall.get_centre().y < paddle.get_centre().y;
                        
                        if (edge & 0x1) { //Hit the left edge
                            if (upperHalf) mainBall.set_speed_y_neg();
                            if (mainBall.get_speed_x() > 0) mainBall.set_speed_x_neg();
                        } else if (edge & 0x2) { //Hit the right edge
                            if (upperHalf) mainBall.set_speed_y_neg();
                            if (mainBall.get_speed_x() < 0) mainBall.set_speed_x_neg();
                        }
                        
                        cout << "Collision with Paddle from Side" << endl << endl;
                    }
                }
			}
            
            /* Mouse motion fast and paddle moves into ball without colliding
             * In this case, bounce ball base xDiff(mouse moving direction)
             * Speed forced to be {ball.get_speed(), 1}. Change speedX based on xDiff
             */
            if (mainBall.get_speed_y() > 0 && 
                inside(mainBall.get_centre(), paddle.get_boundary())) {

                /* Move the centre of the ball out of the paddle first
                 * Set to the left or right boundary
                 * Set the new speed
                 */

                //Hit only the upper half will results in a boost
                //bool upperHalf = mainBall.get_centre().y < paddle.get_centre().y;
                //if (upperHalf) mainBall.set_speed_y(-1);
                //else mainBall.set_speed_y(2);

                mainBall.set_speed_y(-1);

                //Ball is moving vertically
                //Now the boost force will depend on

                cout << "Collision from Inside" << endl << endl;

                if (xDiff > 0) {// Paddle moves to the right fast
                    mainBall.update_pos_centre({paddle.get_boundary().right + mainBall.get_radius(), mainBall.get_centre().y});
                    mainBall.set_speed_x((int)S_FAST);
                } else {
                    mainBall.update_pos_centre({paddle.get_boundary().left - mainBall.get_radius(), mainBall.get_centre().y});
                    mainBall.set_speed_x(-(int)S_FAST);
                }
            }

            /* Bouncing from the Wall Check */
            //The ball falls out of the world, reset everything
            if (mainBall.get_boundary().top >= SCREEN_HEIGHT - 1) {
                mainBall.reset();
                //If it fall outs, don't update with the paddle immediately
            }

            if (mainBall.get_boundary().right >= SCREEN_WIDTH - 1 || mainBall.get_boundary().left <= 0) {
                mainBall.set_speed_x_neg();
            }

            if (mainBall.get_boundary().top <= 0) {
                mainBall.set_speed_y_neg();
            }	
            

            //After setting the speed, update the new position
            mainBall.update_pos();
		}

		SDL_RenderClear(gRenderer);

		paddle.draw();
		mainBall.draw();

        for (int i = 0; i < BRICK_ROW; i++) {
            for (int j = 0; j < BRICK_COL; j++) {
                if (brickArr[i][j] == NULL) continue;
                brickArr[i][j]->draw();
            }
        }

		SDL_RenderPresent(gRenderer);

        //SDL_Delay(200);
	}

	quit_game();

	return 0;
}
