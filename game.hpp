#pragma once
#include "gui.hpp"
#include <iostream>
#include <cmath>
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//Change base attributes here.
const int PLAYER_ONE_VEL_X = 0;
const int PLAYER_ONE_VEL_Y = 10;
const int PLAYER_ONE_WIDTH = SCREEN_WIDTH/30;
const int PLAYER_ONE_HEIGHT = SCREEN_HEIGHT/5;
const int PLAYER_ONE_POS_X = 10 + PLAYER_ONE_WIDTH/2;
const int PLAYER_ONE_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5 + PLAYER_ONE_HEIGHT/2;

const int PLAYER_TWO_VEL_X = 0;
const int PLAYER_TWO_VEL_Y = 10;
const int PLAYER_TWO_WIDTH = SCREEN_WIDTH/30;
const int PLAYER_TWO_HEIGHT = SCREEN_HEIGHT/5;
const int PLAYER_TWO_POS_X = SCREEN_WIDTH - 10 - SCREEN_WIDTH/30 + PLAYER_TWO_WIDTH/2;
const int PLAYER_TWO_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5 + PLAYER_TWO_HEIGHT/2;

const int BALL_MAX_VEL_Y = 15;
const int BALL_MIN_VEL_Y = 1;
const int BALL_VEL_X = 5;
const int BALL_VEL_Y = 5;
const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15; 
const int BALL_POS_X = SCREEN_WIDTH/2;
const int BALL_POS_Y = SCREEN_HEIGHT/2; 

//*******************************************************************************************//
class FPS_Timer
{
    public:
		//Initializes variables
		FPS_Timer();

		//The various clock actions
		void start();
		void capFPS();

		//Gets the timer's time
		Uint32 getTicks();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;
};

FPS_Timer::FPS_Timer()
{
    //Initialize the variables
    mStartTicks = 0;
}

void FPS_Timer::start()
{
    //Get the current clock time
    mStartTicks = SDL_GetTicks();
}

void FPS_Timer :: capFPS(){
	int frameTicks = getTicks();
	if( frameTicks < SCREEN_TICK_PER_FRAME ){
		SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
	}
}

Uint32 FPS_Timer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    time = SDL_GetTicks() - mStartTicks;

    return time;
}

//*******************************************************************************************//


//*******************************************************************************************//
enum axes
{
	X,
	Y
};

enum corners{
	TL, //top left corner
	TR, //top right corner
	BL, //bottom left
	BR //bottom right
};

class Particle
{
	public:
	//Member functions	
		Particle(int, int, int, int, int, int);
		void move();
		void renderToScreen();
		void reboundFromWall();
		void reboundFrom(Particle);
		void reset();

		bool collisionWith(Particle);
		bool collisionWithWall();
		bool outOfBounds();
		
		int getCorner(bool, int);
		int getNearestCorner(int, int, Particle);
		double slopeChange(int, int);
		int getCollisionPoint(bool, int, int, int);
		int whichPlayer();

	//Data members 	
	int score = 0;
	int vx; //velocity along x-axis
	int vy; //velocity along y-axis
	private:
		int x; //x coordinate of centre 
		int y; //y coordinates of centre
		int w; //width of particle
		int h; //height of particle
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Particle PLAYERONE(PLAYER_ONE_VEL_X, PLAYER_ONE_VEL_Y, PLAYER_ONE_POS_X, PLAYER_ONE_POS_Y, PLAYER_ONE_WIDTH, PLAYER_ONE_HEIGHT);
Particle PLAYERTWO(PLAYER_TWO_VEL_X, PLAYER_TWO_VEL_Y, PLAYER_TWO_POS_X, PLAYER_TWO_POS_Y, PLAYER_TWO_WIDTH, PLAYER_TWO_HEIGHT);
Particle BALL(BALL_VEL_X, BALL_VEL_Y, BALL_POS_X, BALL_POS_Y, BALL_WIDTH, BALL_HEIGHT);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Particle :: Particle(int a, int b, int c, int d, int e, int f){
	vx = a;
	vy = b;
	x = c;
	y = d;
	w = e;
	h = f;
	score = 0;	
}

void Particle :: move(){
	if(whichPlayer() == -1){
		x += vx;
		y += vy;
	}


	else if(whichPlayer() == 1){
		if(keyStates[SDL_SCANCODE_W]){
			vy = -PLAYER_ONE_VEL_Y;
			x += vx;
			y += vy;
			if(y  - h/2 < 0) y = h/2;
		}
		if(keyStates[SDL_SCANCODE_S]){
			vy = PLAYER_ONE_VEL_Y;
			x += vx;
			y += vy;
			if (y + h/2 > SCREEN_HEIGHT) y = SCREEN_HEIGHT - h/2;
		}
		vy = 0;
	}


	else if(whichPlayer() == 2){
		if(keyStates[SDL_SCANCODE_UP]){
			vy = -PLAYER_TWO_VEL_Y;
			x += vx;
			y += vy;
			if(y  - h/2 < 0) y = h/2;
		}
		if(keyStates[SDL_SCANCODE_DOWN]){
			vy = PLAYER_TWO_VEL_Y;
			x += vx;
			y += vy;
			if (y + h/2 > SCREEN_HEIGHT) y = SCREEN_HEIGHT - h/2;
		}
		vy = 0;
	}
}

void Particle :: renderToScreen(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_Rect particleRect = {getCorner(X, TL), getCorner(Y, TL), w, h};
	SDL_RenderFillRect( gRenderer,&particleRect);
}

void Particle :: reboundFromWall(){
	if (y - h/2 <= 0){
		x = getCollisionPoint(X, x, y, h/2);
		y = h/2;
		vy *= -1;
	}
	else if(y + h/2 >= SCREEN_HEIGHT){
		x = getCollisionPoint(X, x, y, SCREEN_HEIGHT - h/2);
		y = SCREEN_HEIGHT - h/2;
		vy *= -1;
	}
}

void Particle :: reboundFrom(Particle player){
	while(collisionWith(player)){
		y -= vy;
		x -= vx;
	}

	if(player.whichPlayer() == 1){
		if (getNearestCorner(TR, BR, player) == TR){
			if(vy < 0){
				if (getCorner(X, BR) <= player.getCorner(X, TR) && getCorner(Y, BR) <= player.getCorner(Y, TR)){
					
				}
				else{
				
				}
			}
			else{
				
			}
		}

		else{

		}
	}

	else{
		if (getNearestCorner(TL, BL, player) == TL){
			if(vy  < 0){
				if (getCorner(X, BL) <= player.getCorner(X, TL) && getCorner(Y, BL) <= player.getCorner(Y, TL)){
					
				}
				else{
				
				}
			}
			else{
				
			}

		}

		else{

		}

	}
	
	if(collisionWithWall()){
		if(y - h/2 < 0){
			y = h/2;
		}
		else{
			y = SCREEN_HEIGHT - h/2;
		}
		if(collisionWith(player)){
			if(vx < 0){
				x = player.getCorner(X, BR) + w/2;
			}
			else{
				x = player.getCorner(X, BL) - w/2;
			}
		}
	}
}

void Particle :: reset(){
	if (whichPlayer() == -1){
		if(vx < 0){
			vx = BALL_VEL_X;	
		}
		else if(vx > 0){
			vx = -BALL_VEL_X;
		}
		else{
			vx = BALL_VEL_X;
		}
		vy = BALL_VEL_Y;
		x = BALL_POS_X;
		y = BALL_POS_Y;
	}
	else if (whichPlayer() == 1){
		x = PLAYER_ONE_POS_X;
		y = PLAYER_ONE_POS_Y;
	}
	else if (whichPlayer() == 2){
		x = PLAYER_TWO_POS_X;
		y = PLAYER_TWO_POS_Y;
	}
}

bool Particle :: collisionWith(Particle player){
	if    (getCorner(X, TL)  >= player.getCorner(X, TL) 
		&& getCorner(X, TL)  <= player.getCorner(X, BR)
		&& getCorner(Y, TL)  >= player.getCorner(Y, TL)
		&& getCorner(Y, TL)  <= player.getCorner(Y, BR)) {
		return true;
	} 
	if (getCorner(X, TR)  >= player.getCorner(X, TL) && getCorner(X, TR)  <= player.getCorner(X, BR) && getCorner(Y, TR)  >= player.getCorner(Y, TL) && getCorner(Y, TR)  <= player.getCorner(Y, BR)){
		return true;	
	}
	if (getCorner(X, BL)  >= player.getCorner(X, TL) && getCorner(X, BL)  <= player.getCorner(X, BR) && getCorner(Y, BL)  >= player.getCorner(Y, TL) && getCorner(Y, BL)  <= player.getCorner(Y, BR)){
		return true;
	}
	if (getCorner(X, BR)  >= player.getCorner(X, TL) && getCorner(X, BR)  <= player.getCorner(X, BR) && getCorner(Y, BR)  >= player.getCorner(Y, TL) && getCorner(Y, BR)  <= player.getCorner(Y, BR)){
		return true;
	}
	return false;
}

bool Particle :: collisionWithWall(){
	return (y - h/2 <= 0 || y  + h/2 >= (int)(SCREEN_HEIGHT));
}

bool Particle :: outOfBounds(){
	return (x + w/2 <= 0 || x  - w/2 >= (int)(SCREEN_WIDTH));
}

double Particle :: slopeChange(int x1, int y1){
	float m1 = (float)((float)(y - y1)/(float)(x - x1));
	int y_copy = y;
	int x_copy = x;
	y -= vy;
	x -= vx;
	if(x - x1 == 0){
		y -= vy;
		x -= vx;
	}
	float m2 = (float)((float)(y - y1)/float((x - x1)));
	if((x1 - x_copy)*(x1 - x) < 0){
		return m2-m1;
	}
	return m1 - m2; 
}
int Particle :: getCorner(bool choice1, int choice2){
	if(choice1 == X){
		if(choice2 == TL){
			return x - w/2;
		}
		if(choice2 == TR){
			return x + w/2;
		}
		if(choice2 == BL){
			return x - w/2;
		}
		if(choice2 == BR){
			return x + w/2;
		}
	}
	else{
		if(choice2 == TL){
			return y - h/2;
		}
		if(choice2 == TR){
			return y - h/2;
		}
		if(choice2 == BL){
			return y + h/2;
		}
		if(choice2 == BR){
			return y + h/2;
		}
	}
}

int Particle :: getNearestCorner(int c1, int c2, Particle player){
	if (abs(abs(player.getCorner(Y, c1)) - abs(x)) <= abs(abs(player.getCorner(Y, c2)) - abs(x))){
		return c1;
	} 
	return c2; 
}

int Particle :: getCollisionPoint(bool choice, int x1, int y1, int coordinate){
	float m = (float)((float)(vy)/(float)(vx));
	if(choice == X){
		return x1 + (coordinate-y1)/m;
	}
	else{
		return y1 + m*(coordinate - x1);
	}
}

int Particle :: whichPlayer(){
	if (x == PLAYER_ONE_POS_X && h == PLAYER_ONE_HEIGHT) return 1;
	if (x == PLAYER_TWO_POS_X && h == PLAYER_TWO_HEIGHT) return 2;
	return -1;
}

//*******************************************************************************************//

//GAME FUNCTIONS*****************************************************************************//

void newGame();
void updateScore();
void render();


void newGame(){
	BALL.reset();
	PLAYERONE.reset();
	PLAYERONE.score = 0;
	PLAYERTWO.reset();
	PLAYERTWO.score = 0;	
}

void updateScore(){
	if(BALL.vx < 0){
		++PLAYERTWO.score;
	}
	else if(BALL.vx > 0){
		++PLAYERONE.score;
	}

}

void renderAll(){
		clearScreen();

		PLAYERONE.renderToScreen();
		PLAYERTWO.renderToScreen();
		BALL.renderToScreen();

		renderText(std::to_string(PLAYERONE.score) + " " + std::to_string(PLAYERTWO.score), SCREEN_WIDTH/2, 45, 0xFF, 0xFF, 0xFF);

		render();
}

//*******************************************************************************************//