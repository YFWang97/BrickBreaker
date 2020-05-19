#include "defines.h"
#include "helper.h"
#include "base.h"
#include "paddle.h"
#include "ball.h"
#include "brick.h"
#include "powerup.h"

int main (void) {

    if (initialize() || initialize_img()) {
		ERROR_MSG("PROG: Program quits. Initialization failed");
        return 0;
	}

	bool quit = false;

	SDL_Event e;
	
	bool clicked = false;
    bool pause = false;
    bool fallOut = false;

    int xLast = 0;
    int xDiff = 0;

	Paddle paddle;
	Ball mainBall;

    mainBall.set_lives(5);

    Brick* brickArr[BRICK_ROW][BRICK_COL];
    int breakTotal;

    vector<Ball> ballIndiVector;

    vector<Powerup> powerupVector;

    Base over(overTexture, {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, 1);
    Base levelup(levelupTexture, {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, 1);
    
    srand(SDL_GetTicks());


    /* Set up bricks */
    for (int i = 0; i < BRICK_ROW; i++) {
        for (int j = 0; j < BRICK_COL; j++) {
            breakTotal++;
            BrickType brType = BR_DEFAULT;
            if (rand() % 5 == 0) {
            brType = BR_BREAKMULT;
            } else if (rand() % 8 == 0) {
                brType = BR_UNBREAK;
                breakTotal--;
            }

            brickArr[i][j] = new Brick(brType, {BRICK_MARGIN + j * BRICK_WIDTH, BRICK_MARGIN + i * BRICK_HEIGHT});
        }
    }


    /* Set up Powerup on bricks */
    for (int i = 0; i < 10; i++) {
        PowerupType pType = POW_NONE;
        int randN = rand();
        if (randN % 3) {
            pType = POW_LONG;
        } else if (randN % 5) {
            pType = POW_SHORT;
        }
        brickArr[rand() % BRICK_ROW][rand() % BRICK_COL]->set_powerup_type(pType);
    }

    
    /* Set up the ball live indicator */
    for (int i = 0; i < mainBall.get_lives(); i++) {
        Ball ball;
        ball.update_width(ball.get_rect().w / 2);
        ball.update_height(ball.get_rect().h / 2);
        ball.update_pos_centre({10 + 30 * i, 780});
        ballIndiVector.push_back(ball);
    }


	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_MOUSEMOTION: {
					int x;
					SDL_GetMouseState(&x, NULL);

                    if ((x - xLast) != 0) {
                        xDiff = x - xLast;
                    }

                    xLast = x;

                    //cout << "Cur x: " << x << " Last x: " << xLast  << " Diff " << xDiff << endl;

					paddle.update_pos_centre_x(x);
					mainBall.update_pos_w_paddle(paddle.get_centre());

                    fallOut = false;
					break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    if (!fallOut) {
					    clicked = true;
                    }
					break;
                }
				case SDL_MOUSEBUTTONUP:
                    if (clicked) {
                        clicked = false;
                        mainBall.fire(10, -5);
                    }
					break;
                case SDL_KEYDOWN:
                    pause = !pause;
                    break;
                        
			}
		}

        if (pause) continue;


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

                    Coordinate brickIndex = {brickCol, brickRow};

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

            vector<Coordinate> breakList;

            for (auto& brick: edge_collision) {
                breakList.push_back(brick.first);
            }

            if (edge_collision.size() < 2) {
                for (auto& brick: corner_collision) {
                    breakList.push_back(brick.first);
                }
            }

            for (auto& brick: breakList) {

                switch (brickArr[brick.y][brick.x]->get_brick_type()) {
                    case BR_BREAKMULT:
                        brickArr[brick.y][brick.x]->set_brick_type(BR_DEFAULT);
                        break;
                    case BR_DEFAULT: {
                        PowerupType pType = brickArr[brick.y][brick.x]->get_powerup_type();
                        if (pType >= 0) {
                            powerupVector.push_back(Powerup(pType, brickArr[brick.y][brick.x]->get_centre()));
                        }
                        delete brickArr[brick.y][brick.x];
                        brickArr[brick.y][brick.x] = NULL;
                        breakTotal--;
                        break;
                    }
                    case BR_UNBREAK:
                        break;
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
        if (mainBall.get_boundary().top > SCREEN_HEIGHT - 1 && !fallOut) {
            mainBall.reset();
            fallOut = true;
            //If it fall outs, don't update with the paddle immediately
            if (ballIndiVector.size() > 0) {
                ballIndiVector.pop_back();
            }
            mainBall.set_lives(mainBall.get_lives() - 1);
        }

        if (mainBall.get_boundary().right >= SCREEN_WIDTH - 1 || mainBall.get_boundary().left <= 0) {
            mainBall.set_speed_x_neg();
        }

        if (mainBall.get_boundary().top <= 0) {
            mainBall.set_speed_y_neg();
        }	

        /* Check Collision and Fallouts of the powerups */
        for (auto it = powerupVector.begin(); it != powerupVector.end(); it++) {
            bool collision = check_collision(it->get_boundary(), paddle.get_boundary());

            if (collision) {
                switch (it->get_powerup_type()) {
                    case POW_LONG:
                        paddle.update_width(P_LONG);
                        //mainBall.set_speed(S_FAST);
                        break;

                    case POW_SHORT:
                        paddle.update_width(P_SHORT);
                        break;
                }


                powerupVector.erase(it);
                it--;
            } else if (it->get_boundary().top > SCREEN_HEIGHT - 1) {
                powerupVector.erase(it);
                it--;
            }
        }

        /* Position Update and Draw */
        

		SDL_RenderClear(gRenderer);
        
        //After setting the speed, update the new position
        mainBall.update_pos();
		mainBall.draw();


        if (mainBall.get_lives() == 0) {
            pause = true;
            over.draw();
        }

        if (breakTotal == 0) {
            pause = true;
            levelup.draw();
        }

		paddle.draw();


        for (int i = 0; i < BRICK_ROW; i++) {
            for (int j = 0; j < BRICK_COL; j++) {
                if (brickArr[i][j] == NULL) continue;
                brickArr[i][j]->draw();
            }
        }

        for (auto& powerup : powerupVector) {
            powerup.update_pos();
            powerup.draw();
        }

        for (auto& ball : ballIndiVector) {
            ball.draw();
        }

		SDL_RenderPresent(gRenderer);

        //SDL_Delay(200);
	}

	quit_game();

	return 0;
}
